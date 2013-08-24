/*  Elise IM, free crossplatform IM client
	Copyright (C) 2012  Elise project

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../../../../api/e_pluginapi.h"
#include "../../../../api/e_dbpluginapi.h"
#include "../../../../api/e_options.h"
#include "../../../../api/e_database.h"
#include "../options/options.h"
#include "pluginloader.h"
#include "../../core.h"
#include "pluginloaderoptions.h"
#include "../folders/folders.h"

const QLatin1String	kPluginLoaderPluginLoaded		= QLatin1String(__PluginLoader_PluginLoaded);
const QLatin1String	kPluginLoaderPluginUnloaded		= QLatin1String(__PluginLoader_PluginUnloaded);
const QLatin1String	kPluginLoaderPluginsLoaded		= QLatin1String(__PluginLoader_PluginsLoaded);
//const QLatin1String	kPluginLoaderPluginsUnloaded	= QLatin1String(__PluginLoader_PluginsUnloaded);
const QLatin1String	kPluginLoaderIsPluginLoaded		= QLatin1String(__PluginLoader_IsPluginLoaded_service);

QDir*						PluginLoader::pluginsDir_;
QMap<QString, Plugin>*		PluginLoader::plugins_ = 0;
QSet<int>*					PluginLoader::loadedPluginsTypes_ = 0;

int PluginLoader::loadPluginLoader()
{
	pluginsDir_ = Folders::getPluginsDir();
	if (!loadedPluginsTypes_) {
		loadedPluginsTypes_ = new QSet<int>();
	}

	g_core->hookEvent(&kOptionsShow_event, &PluginLoaderOptions::createLoaderOptionsPage);
	g_core->createServiceFunction(&kPluginLoaderIsPluginLoaded, &PluginLoader::isPluginLoaded);
	g_core->createHookableEvent(&kPluginLoaderPluginLoaded);
	g_core->createHookableEvent(&kPluginLoaderPluginUnloaded);
	g_core->createHookableEvent(&kPluginLoaderPluginsLoaded);
	//core->createHookableEvent(&kPluginLoaderPluginsUnloaded);

	return 0;
}

int PluginLoader::unloadPluginLoader()
{
	unloadPlugins();

	delete pluginsDir_;
	delete loadedPluginsTypes_;
	loadedPluginsTypes_ = 0;
	g_core->unhookEvent(&kOptionsShow_event, &PluginLoaderOptions::createLoaderOptionsPage);
	g_core->destroyServiceFunction(&kPluginLoaderIsPluginLoaded);
	g_core->destroyHookableEvent(&kPluginLoaderPluginLoaded);
	g_core->destroyHookableEvent(&kPluginLoaderPluginUnloaded);
	g_core->destroyHookableEvent(&kPluginLoaderPluginsLoaded);
	//core->destroyHookableEvent(&kPluginLoaderPluginsUnloaded);

	return 0;
}

QJsonObject* PluginLoader::getPluginInfo(const QString &pluginModuleName)
{
	if (!confirmPluginModule(pluginModuleName))
		return 0;

	QJsonObject* metaData = new QJsonObject();
	QPluginLoader* loader = plugins_->value(pluginModuleName).loader;
	*metaData = loader->metaData().value("MetaData").toObject();

	if (metaData->empty()) {
		delete metaData;
		return 0;
	}

	QJsonValue val = metaData->value("name");
	if (val.isUndefined()) {
		delete metaData;
		return 0;
	}

	val = metaData->value("type");
	if (val.isUndefined()) {
		delete metaData;
		return 0;
	}

	return metaData;
}

const QMap<QString, Plugin>* PluginLoader::getAvailablePlugins()
{
	if (plugins_ == 0)
		plugins_ = new QMap<QString, Plugin>();

	Plugin plugin;
	plugin.type = -1;
	plugin.loader = 0;

	//-- First of all validate current list of plugins
	QMap<QString, Plugin>::iterator iteratorPlugins = plugins_->begin();
	QMap<QString, Plugin>::iterator pluginsEnd = plugins_->end();
	while (iteratorPlugins != pluginsEnd) {
		if (!pluginsDir_->exists(iteratorPlugins.key()))
			iteratorPlugins = plugins_->erase(iteratorPlugins);
		else
			++iteratorPlugins;
	}

	//-- Update the list of plugins
	QStringList files = pluginsDir_->entryList(QDir::Files);
	QStringList::const_iterator i = files.constBegin();
	QStringList::const_iterator iEnd = files.constEnd();
	while (i != iEnd) {
		if (!plugins_->contains(*i)) {
			plugin.loader = new QPluginLoader(pluginsDir_->absoluteFilePath(*i));
			QJsonObject metaData = plugin.loader->metaData().value("MetaData").toObject();
			if (!metaData.isEmpty()) {
				QJsonValue val = metaData.value("name");
				if (!val.isUndefined()) {
					val = metaData.value("type");
					if (!val.isUndefined()) {
						//-- FIX ME: there is no method QJsonValue::toInt() now.
						plugin.type = val.toDouble();
						plugins_->insert(*i, plugin);
						++i;
						continue;
					}
				}
			}
			//-- Delete wrong instances
			delete plugin.loader;
			plugin.loader = 0;
		}
		++i;
	}

	if (plugins_->isEmpty())
		return 0;

	return plugins_;
}

int PluginLoader::updatePluginState(const QString &pluginModuleName, bool disable)
{
	if (!confirmPluginModule(pluginModuleName))
		return -1;

	int result = 0;
	QString module = QStringLiteral("core");
	QString setting = QStringLiteral("pluginDisabled_") + pluginModuleName;
	Setting* set = new Setting;
	set->contact = 0;
	set->qsModule = &module;
	set->qsSetting = &setting;
	set->var = new DBVariant;
	set->var->type = __Int_Type;
	if (disable) {
		//-- Disable plugin
		set->var->intValue = 1;
		result = g_core->callService(&g_kDBWriteSetting_service, reinterpret_cast<intptr_t>(set), 0);
	} else {
		//-- Remove breaker from the profile
		result = g_core->callService(&g_kDBDellSetting_service, reinterpret_cast<intptr_t>(set), 0);
	}

	delete set->var;
	delete set;
	return result;
}

bool PluginLoader::isPluginDisabled(const QString& pluginModuleName)
{
	QString module = QStringLiteral("core");
	QString setting = QStringLiteral("pluginDisabled_") + pluginModuleName;
	bool result = 0;
	Setting* set = new Setting;
	set->contact = 0;
	set->qsModule = &module;
	set->qsSetting = &setting;
	set->var = new DBVariant;
	set->var->type = __Int_Type;
	if (!g_core->callService(&g_kDBReadSetting_service, reinterpret_cast<intptr_t>(set), 0)) {
		result = static_cast<bool>(set->var->intValue);
	}
	delete set->var;
	delete set;
	return result;
}

intptr_t PluginLoader::isPluginLoaded(intptr_t name, intptr_t type)
{
	//-- Check a specific plugin or just plugin of specific type
	if (name) {
		QString* pluginModuleName = reinterpret_cast<QString*>(name);
		return isPluginLoaded(*pluginModuleName);
	} else {
		return isPluginLoaded(type);
	}
}

bool PluginLoader::isPluginLoaded(const QString& pluginModuleName)
{
	if (!confirmPluginModule(pluginModuleName))
		return false;

	if (!plugins_->contains(pluginModuleName))
		return false;

	return plugins_->value(pluginModuleName).loader->isLoaded();
}

bool PluginLoader::isPluginLoadable(const QString& pluginModuleName)
{
	QPluginLoader* loader = plugins_->value(pluginModuleName).loader;

	if (loader->isLoaded())
		return false;

	QJsonObject* pluginInfo = getPluginInfo(pluginModuleName);
	if (!pluginInfo)
		return false;

	int pluginID = pluginInfo->value("type").toDouble();

	//-- Check already loaded plugin types
	return !(loadedPluginsTypes_->contains(pluginID));
}

bool PluginLoader::isPluginUnloadable(const QString& pluginModuleName)
{
	//-- We can't unload the DBPlugin
	if (plugins_->value(pluginModuleName).type == __Plugins_DBPlugin)
		return false;

	return true;
}

QObject* PluginLoader::loadPlugin(const QString& pluginModuleName, bool manualyLoad)
{
	Plugin* plugin = &(*plugins_)[pluginModuleName];

	IPlugin* pluginInterface = qobject_cast<IPlugin*>(plugin->loader->instance());
	if (!pluginInterface) {
		plugin->loader->unload();
		return 0;
	}

	//-- Load Elise plugin
	if (pluginInterface->Load(g_core)) {
		plugin->loader->unload();
		return 0;
	}

	if (plugin->type)
		loadedPluginsTypes_->insert(plugin->type);

	if (manualyLoad)
		g_core->notifyEventHooks(&kPluginLoaderPluginLoaded,
								 reinterpret_cast<intptr_t>(&pluginModuleName), 0);

	return plugin->loader->instance();
}

int PluginLoader::unloadPlugin(const QString& pluginModuleName, bool manualyUnload)
{
	Plugin* plugin = &(*plugins_)[pluginModuleName];

	if (!(plugin->loader->isLoaded()))
		return 0;

	IPlugin* pluginInterface = qobject_cast<IPlugin*>(plugin->loader->instance());

	//-- Call Elise plugin api unload
	if (pluginInterface->Unload())
		QMessageBox::critical(0, QStringLiteral("unloadPlugin error"),
							  QStringLiteral("Error while unloading plugin ") + pluginModuleName,
							  QMessageBox::Ok);

	//-- Remove plugin type from the list
	if (plugin->type)
		loadedPluginsTypes_->remove(plugin->type);

	//-- Free plugin
	if (!plugin->loader->unload())
		QMessageBox::critical(0, QStringLiteral("unloadPlugin error"),
							  QStringLiteral("Can't unload plugin ") + pluginModuleName,
							  QMessageBox::Ok);

	if (manualyUnload)
		g_core->notifyEventHooks(&kPluginLoaderPluginUnloaded,
								 reinterpret_cast<intptr_t>(&pluginModuleName), 0);

	return 0;
}

int PluginLoader::unloadPlugin(int pluginType)
{
	QMap<QString, Plugin>::iterator iteratorPlugins = plugins_->begin();
	QMap<QString, Plugin>::iterator pluginsEnd = plugins_->end();
	while (iteratorPlugins != pluginsEnd) {
		if (iteratorPlugins.value().type == pluginType)
			return unloadPlugin(iteratorPlugins.key(), true);
		++iteratorPlugins;
	}

	return 1;
}

int PluginLoader::loadPlugins()
{
	if (plugins_ == 0)
		return 1;

	QString pluginModuleName;
	QMap<QString, Plugin>::const_iterator iteratorPlugins = plugins_->constBegin();
	QMap<QString, Plugin>::const_iterator pluginsEnd = plugins_->constEnd();

	while (iteratorPlugins != pluginsEnd) {
		pluginModuleName = iteratorPlugins.key();

		//-- If plugin disabled by user earlier - break
		if (!isPluginDisabled(pluginModuleName)) {

			//-- Check existing interfaces
			if (isPluginLoadable(pluginModuleName))
				loadPlugin(pluginModuleName);
		}
		++iteratorPlugins;
	}

	g_core->notifyEventHooks(&kPluginLoaderPluginsLoaded, 0, 0);

	return 0;
}

int PluginLoader::unloadPlugins()
{
	//-- If launching app - nothing to unload
	if (plugins_ == 0)
        return 0;

	//-- NOTE: we are not destroying QMap plugins here because we need it for new loading and
	//-- it will be automatically destroyed on fully exit
	QMap<QString, Plugin>::iterator iteratorPlugins = plugins_->begin();
	QMap<QString, Plugin>::iterator pluginsEnd = plugins_->end();
	while (iteratorPlugins != pluginsEnd) {
		if (iteratorPlugins.value().type != __Plugins_DBPlugin)
			unloadPlugin(iteratorPlugins.key());
		++iteratorPlugins;
	} //while

	//core->notifyEventHooks(&kPluginLoaderPluginsUnloaded, 0, 0);

	return 0;
}

const QStringList PluginLoader::getDBPlugins()
{
	QStringList result;
	QMap<QString, Plugin>::iterator iteratorPlugins = plugins_->begin();
	QMap<QString, Plugin>::iterator pluginsEnd = plugins_->end();
	while (iteratorPlugins != pluginsEnd) {
		if (iteratorPlugins.value().type == __Plugins_DBPlugin)
			result.insert(result.size(), iteratorPlugins.key());
		++iteratorPlugins;
	}

	return result;
}
