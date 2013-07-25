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

const QLatin1String	kCoreIsPluginLoaded	=	QLatin1String(__Core_IsPluginLoaded_service);
//const QLatin1String	kCoreGetPluginInterfaces = QLatin1String(__Core_GetPluginInterfaces_service);

QDir*						PluginLoader::pluginsDir_;
QMap<QString, Plugin>*		PluginLoader::plugins_ = NULL;
//QMap<QUuid, QString>*		PluginLoader::interfaces_ = NULL;
QSet<int>*				PluginLoader::loadedPluginsTypes_ = NULL;

int PluginLoader::loadPluginLoader()
{
	pluginsDir_ = Folders::getPluginsDir();
	if (!loadedPluginsTypes_) {
		loadedPluginsTypes_ = new QSet<int>();
		//multiplyImplementingInterfaces_->insert(__UUID_TestPlugin);
	}

	core->hookEvent(&kOptionsShow_event, &PluginLoaderOptions::createLoaderOptionsPage);
	core->createServiceFunction(&kCoreIsPluginLoaded, &PluginLoader::isPluginLoaded);
	//core->createServiceFunction(&kCoreGetPluginInterfaces, &PluginLoader::getPluginInterfaces);

	return 0;
}

int PluginLoader::unloadPluginLoader()
{
	delete pluginsDir_;
	delete loadedPluginsTypes_;
	loadedPluginsTypes_ = NULL;
	core->unhookEvent(&kOptionsShow_event, &PluginLoaderOptions::createLoaderOptionsPage);
	core->destroyServiceFunction(&kCoreIsPluginLoaded);
	//core->destroyServiceFunction(&kCoreGetPluginInterfaces);

	return 0;
}

QJsonObject* PluginLoader::getPluginInfo(const QString &pluginModuleName)
{
	if (!confirmPluginModule(pluginModuleName))
		return NULL;

	QJsonObject* metaData = new QJsonObject();
	QPluginLoader loader;
	loader.setFileName(pluginsDir_->absoluteFilePath(pluginModuleName));
	*metaData = loader.metaData();

	if (metaData->empty())
		return 0;

	QJsonValue val = metaData->value("name");
	if (val.isUndefined())
		return 0;

	val = metaData->value("type");
	if (val.isUndefined())
		return 0;

	return metaData;
}

/*intptr_t PluginLoader::getElisePluginInterfaces(intptr_t id, intptr_t)
{
	//-- Note: We can get interfaces only for loaded plugins.

	QUuid* pluginUuid = reinterpret_cast<QUuid*>(id);
	IPlugin* pluginInterface;

	QMap<QString, Plugin>::const_iterator iteratorPlugins = plugins_->constBegin();
	QMap<QString, Plugin>::const_iterator pluginsEnd = plugins_->constEnd();

	bool found = false;

	while (iteratorPlugins != pluginsEnd)
	{
		if (iteratorPlugins.value().loaded == true
				&& *(iteratorPlugins.value().uuid) == *pluginUuid)
		{
			pluginInterface = qobject_cast<IPlugin*>(iteratorPlugins.value().instance);
			found = true;
			break;
		}
		++iteratorPlugins;
	}

	if (!found)
		return 0;

	QSet<QUuid>* interfaces = new QSet<QUuid>();

	*interfaces = *pluginInterface->ElisePluginInterfaces();

	return reinterpret_cast<intptr_t>(interfaces);
}*/

const QMap<QString, Plugin>* PluginLoader::getAvailablePlugins()
{
	if (plugins_ == 0)
		plugins_ = new QMap<QString, Plugin>();

	QPluginLoader loader;

	Plugin plugin;
	plugin.type = -1;
	plugin.instance = 0;

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
			loader.setFileName(pluginsDir_->absoluteFilePath(*i));
			QJsonObject metaData = loader.metaData();

			if (!metaData.empty()) {
				QJsonValue val = metaData.value("name");
				if (!val.isUndefined()) {
					val = metaData.value("id");
					if (!val.isUndefined()) {
						//-- FIX ME: there is no method QJsonValue::toInt() now.
						plugin.type = metaData.value("type").toDouble();
						plugins_->insert(*i, plugin);
					}
				}
			}

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
		result = core->callService(&kDBWriteSetting_service, reinterpret_cast<intptr_t>(set), 0);
	} else {
		//-- Remove breaker from the profile
		result = core->callService(&kDBDellSetting_service, reinterpret_cast<intptr_t>(set), 0);
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
	if (!core->callService(&kDBReadSetting_service, reinterpret_cast<intptr_t>(set), 0)) {
		result = static_cast<bool>(set->var->intValue);
	}
	delete set->var;
	delete set;
	return result;
}

intptr_t PluginLoader::isPluginLoaded(intptr_t name, intptr_t id)
{
	//-- Check a specific plugin or just plugin of specific type
	if (name) {
		QString* pluginModuleName = reinterpret_cast<QString*>(name);
		return isPluginLoaded(*pluginModuleName);
	} else {
		return isPluginLoaded(id);
	}
}

bool PluginLoader::isPluginLoaded(const QString& pluginModuleName)
{
	if (!confirmPluginModule(pluginModuleName))
		return false;

	QPluginLoader loader;
	loader.setFileName(pluginsDir_->absoluteFilePath(pluginModuleName));

	if (loader.isLoaded())
		return true;

	return false;
}

bool PluginLoader::isPluginLoaded(int id)
{
	if (loadedPluginsTypes_->contains(id))
		return true;

	return false;
}

bool PluginLoader::isPluginLoadable(const QString& pluginModuleName)
{
	QPluginLoader loader;
	loader.setFileName(pluginsDir_->absoluteFilePath(pluginModuleName));
	if (loader.isLoaded())
		return false;

	QJsonObject* pluginInfo = getPluginInfo(pluginModuleName);
	if (!pluginInfo)
		return false;

	int pluginID = pluginInfo->value("type").toDouble();

	//-- Check already loaded plugin types
	if (loadedPluginsTypes_->contains(pluginID))
		return false;

	return true;
}

bool PluginLoader::isPluginUnloadable(const QString& pluginModuleName)
{
	//-- We can't unload the DBPlugin
	if (plugins_->value(pluginModuleName).type == 1)
		return false;

	return true;
}

/*IDBPlugin* PluginLoader::loadDBPlugin(const QString& pluginModuleName)
{
	QMap<QString, IDBPlugin*>::const_iterator iteratorPlugins = name->constBegin();
	QMap<QString, IDBPlugin*>::const_iterator pluginsEnd = name->constEnd();
	QPluginLoader loader;

	while (iteratorPlugins != pluginsEnd) {
		loader.setFileName(pluginsDir_.absoluteFilePath(pluginModuleName));

		if (iteratorPlugins.key() == pluginModuleName)
			loadPlugin(pluginModuleName);
		else
			loader.unload();

		++iteratorPlugins;
	}

	return 0;
}*/

IPlugin* PluginLoader::loadPlugin(const QString& pluginModuleName)
{
	QPluginLoader loader;
	loader.setFileName(pluginsDir_->absoluteFilePath(pluginModuleName));

	Plugin* plugin = &(*plugins_)[pluginModuleName];
	plugin->instance = loader.instance();

	IPlugin* pluginInterface = qobject_cast<IPlugin*>(plugin->instance);
	if (!pluginInterface) {
		return 0;
	}

	//-- Load Elise plugin
	if (pluginInterface->Load(core))
		return 0;

	if (plugin->type)
		loadedPluginsTypes_->insert(plugin->type);

	return pluginInterface;
}

int PluginLoader::unloadPlugin(const QString& pluginModuleName)
{
	QPluginLoader loader;
	loader.setFileName(pluginsDir_->absoluteFilePath(pluginModuleName));
	Plugin* plugin = &(*plugins_)[pluginModuleName];

	if (!loader.isLoaded())
		return 0;

	IPlugin* pluginInterface = qobject_cast<IPlugin*>(plugin->instance);

	//-- Call Elise plugin api unload
	if (pluginInterface->Unload())
		QMessageBox::critical(0, QStringLiteral("unloadPlugin error"),
							  QStringLiteral("Error while unloading plugin ") + pluginModuleName,
							  QMessageBox::Ok);

	//-- Remove plugin type from the list
	loadedPluginsTypes_->remove(plugin->type);

	//-- Free plugin
	loader.unload();

	plugin->instance = 0;
	return 0;
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
	return 0;
}

int PluginLoader::unloadPlugins()
{
    //-- Launching app
	if (plugins_ == 0)
        return 0;

	//-- NOTE: we are not destroying QMap plugins here because we need it for new loading and
	//-- it will be automatically destroyed on fully exit
	QMap<QString, Plugin>::iterator iteratorPlugins = plugins_->begin();
	QMap<QString, Plugin>::iterator pluginsEnd = plugins_->end();
	while (iteratorPlugins != pluginsEnd) {
		unloadPlugin(iteratorPlugins.key());
		++iteratorPlugins;
	} //while
	plugins_->clear();

	return 0;
}

const QStringList PluginLoader::getDBPlugins()
{
	QStringList result;
	QMap<QString, Plugin>::iterator iteratorPlugins = plugins_->begin();
	QMap<QString, Plugin>::iterator pluginsEnd = plugins_->end();
	while (iteratorPlugins != pluginsEnd) {
		if (iteratorPlugins.value().type == 1)
			result.insert(result.size(), iteratorPlugins.key());
		++iteratorPlugins;
	}

	return result;
}
