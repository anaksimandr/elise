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
#include "coreapi.h"
#include "pluginloader.h"
#include "../../core.h"
#include "pluginloaderoptions.h"

const QLatin1String	kCoreIsPluginLoaded	=	QLatin1String(__Core_IsPluginLoaded_service);
const QLatin1String	kCoreGetPluginInterfaces = QLatin1String(__Core_GetPluginInterfaces_service);

QDir						PluginLoader::pluginsDir_;
QMap<QString, Plugin>*		PluginLoader::plugins_ = 0;
QMap<QUuid, QString>*		PluginLoader::interfaces_ = 0;

int PluginLoader::loadPluginLoader()
{
	pluginsDir_ = getPluginsDir();
	core->hookEvent(&kOptionsShow_event, &PluginLoaderOptions::createLoaderOptionsPage);
	core->createServiceFunction(&kCoreIsPluginLoaded, &PluginLoader::isPluginLoaded);
	core->createServiceFunction(&kCoreGetPluginInterfaces, &PluginLoader::getElisePluginInterfaces);

	return 0;
}

int PluginLoader::unloadPluginLoader()
{
	core->unhookEvent();
	core->destroyServiceFunction(&kCoreIsPluginLoaded);
	core->destroyServiceFunction(&kCoreGetPluginInterfaces);

	return 0;
}

QDir PluginLoader::getPluginsDir()
{
	QDir pluginsDir = QDir(qApp->applicationDirPath());

	//-- Really not shure that this is needed
	//#if defined(Q_OS_WIN)
	//if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
	//	pluginsDir.cdUp();
	//#elif defined(Q_OS_MAC)
	//if (pluginsDir.dirName() == "MacOS") {
	//	pluginsDir.cdUp();
	//	pluginsDir.cdUp();
	//	pluginsDir.cdUp();
	//}
	//#endif

	pluginsDir.cd("Plugins");

	return pluginsDir;
}

PluginInfo* PluginLoader::getElisePluginInfo(const QString &pluginModuleName)
{
	PluginInfo* result = new PluginInfo;
	QPluginLoader loader;
	loader.setFileName(pluginsDir_.absoluteFilePath(pluginModuleName));
	bool doNotUnload = loader.isLoaded();
	IPlugin* pluginInterface = qobject_cast<IPlugin*>(loader.instance());
	const PluginInfo* buf = pluginInterface->ElisePluginInfo();
	result->name = buf->name;
	result->version[0] = buf->version[0];
	result->version[1] = buf->version[1];
	result->version[2] = buf->version[2];
	result->version[3] = buf->version[3];
	result->description = buf->description;
	result->author = buf->author;
	result->authorEmail = buf->authorEmail;
	result->copyright = buf->copyright;
	result->homepage = buf->homepage;
	result->uuid = buf->uuid;
	if (!doNotUnload)
		loader.unload();
	return result;
}

intptr_t PluginLoader::getElisePluginInterfaces(intptr_t id, intptr_t)
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
}

const QMap<QString, Plugin>* PluginLoader::getAvailablePlugins()
{
	if (plugins_ == 0)
		plugins_ = new QMap<QString, Plugin>();

	if (interfaces_ == 0)
		interfaces_ = new QMap<QUuid, QString>();

	QPluginLoader loader;
	QObject* pluginInstance = 0;
	Plugin plugin;
	plugin.loaded = false;
	plugin.instance = 0;
	plugin.uuid = 0;

	//-- First of all validate current list of plugins
	QMap<QString, Plugin>::iterator iteratorPlugins = plugins_->begin();
	QMap<QString, Plugin>::iterator pluginsEnd = plugins_->end();
	while (iteratorPlugins != pluginsEnd) {
		if (!pluginsDir_.exists(iteratorPlugins.key()))
			iteratorPlugins = plugins_->erase(iteratorPlugins);
		else
			++iteratorPlugins;
	}

	//-- Update the list of plugins
	QStringList files = pluginsDir_.entryList(QDir::Files);
	QStringList::const_iterator i = files.constBegin();
	QStringList::const_iterator iEnd = files.constEnd();
	while (i != iEnd) {
		if (!plugins_->contains(*i)) {
			loader.setFileName(pluginsDir_.absoluteFilePath(*i));
			pluginInstance = loader.instance();
			if (pluginInstance) {
				IPlugin* validPlugin = qobject_cast<IPlugin*>(pluginInstance);
				//-- Note: qobject_cast<IPlugin*>(pluginObject) does not work for plugins that
				//-- inherited from interfaces which inherits IPlugin.
				if (validPlugin)
					plugins_->insert(*i, plugin);
			}
			loader.unload();
		}
		++i;
	}

	if (plugins_->isEmpty())
		return 0;

	return plugins_;
}

int PluginLoader::savePluginStateOrDelete(const QString &pluginModuleName, bool disableOrDelete)
{
	if (!pluginsDir_.exists(pluginModuleName)) {
		QMessageBox::critical(0, "Error", "Plugin module is not found.\nPerhaps it was deleted.",
							  QMessageBox::Ok);
		return -1;
	}

	QString qsUuid;
	if (disableOrDelete) {
		const Plugin* plugin = &plugins_->value(pluginModuleName);
		qsUuid = plugin->uuid->toString();
	}
	else {
		PluginInfo* pluginInfo = getElisePluginInfo(pluginModuleName);
		qsUuid = pluginInfo->uuid.toString();
		delete pluginInfo;
	}

	int result = 0;
	QString module = QStringLiteral("core");
	QString setting = QStringLiteral("pluginDisabled_") + qsUuid;
	Setting* set = new Setting;
	set->contact = 0;
	set->qsModule = &module;
	set->qsSetting = &setting;
	set->var = new DBVariant;
	set->var->type = 0; //-- int
	if (disableOrDelete) {
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

bool PluginLoader::isLoadingPluginDisabled(const QString& pluginModuleName)
{
	PluginInfo* pluginInfo = getElisePluginInfo(pluginModuleName);

	QString module = QStringLiteral("core");
	QString setting = QStringLiteral("pluginDisabled_") + pluginInfo->uuid.toString();
	bool result = 0;
	Setting* set = new Setting;
	set->contact = 0;
	set->qsModule = &module;
	set->qsSetting = &setting;
	set->var = new DBVariant;
	set->var->type = 0; //-- int
	if (!core->callService(&kDBReadSetting_service, reinterpret_cast<intptr_t>(set), 0)) {
		result = static_cast<bool>(set->var->intValue);
	}
	delete set->var;
	delete set;
	delete pluginInfo;
	return result;
}

intptr_t PluginLoader::isPluginLoaded(intptr_t id, intptr_t)
{
	QUuid* pluginUuid = reinterpret_cast<QUuid*>(id);

	QMap<QString, Plugin>::const_iterator iteratorPlugins = plugins_->constBegin();
	QMap<QString, Plugin>::const_iterator pluginsEnd = plugins_->constEnd();

	while (iteratorPlugins != pluginsEnd) {
		if (iteratorPlugins.value().loaded == true
				&& *(iteratorPlugins.value().uuid) == *pluginUuid)
			return 1;
		++iteratorPlugins;
	}

	return 0;
}

bool PluginLoader::isPluginLoaded(const QString& pluginModuleName)
{
	//if (plugins->contains(pluginModuleName)) {
	const Plugin* p = &plugins_->value(pluginModuleName);
	return p->loaded;
	//}
	//return false;
}

bool PluginLoader::isPluginLoadable(const QString& pluginModuleName)
{
	//if (pluginModuleName.isEmpty())
		//return false;

	const Plugin* p = &plugins_->value(pluginModuleName);
	if (p->loaded)
		return false;

	//-- hmmm... plugin is not loaded and QUuid* == 0
	//if (isPluginLoaded(reinterpret_cast<intptr_t>(p->uuid), 0))
		//return false;

	QPluginLoader loader;
	loader.setFileName(pluginsDir_.absoluteFilePath(pluginModuleName));
	//bool doNotUnload = loader.isLoaded();
	IPlugin* pluginInterface = qobject_cast<IPlugin*>(loader.instance());

	//-- Iterate all interfaces of current plugin
	const QSet<QUuid>* pluginInterfaces = pluginInterface->ElisePluginInterfaces();
	QSet<QUuid>::const_iterator i = pluginInterfaces->constBegin();
	QSet<QUuid>::const_iterator iEnd = pluginInterfaces->constEnd();
	bool result = true;
	while (i != iEnd) {
		//-- if one of the interfaces is already registered - break
		if (interfaces_->contains(*i)) {
			result = false;
			break;
		}
		++i;
	}
	//if (!doNotUnload)
	loader.unload();

	return result;
}

bool PluginLoader::isPluginUnloadable(const QString& pluginModuleName)
{
	//if (pluginModuleName.isEmpty())
		//return false;

	const Plugin* p = &plugins_->value(pluginModuleName);

	//-- Moved to unloadPlugin
	//if (!p->loaded)
		//return false;

	//-- We can't unload the DBPlugin
	IDBPlugin* pluginInterface = qobject_cast<IDBPlugin*>(p->instance);

	if (pluginInterface)
		return false;

	return true;
}

int PluginLoader::loadDBPlugin(const QString& pluginModuleName,
							   const QMap<QString, IDBPlugin*> *dbPlugins)
{
	QMap<QString, IDBPlugin*>::const_iterator iteratorPlugins = dbPlugins->constBegin();
	QMap<QString, IDBPlugin*>::const_iterator pluginsEnd = dbPlugins->constEnd();
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
}

int PluginLoader::loadPlugin(const QString& pluginModuleName)
{
	Plugin* plugin = &(*plugins_)[pluginModuleName];
	QObject* pluginInstance = plugin->instance;

	if (!pluginInstance) {
		QPluginLoader loader;
		//QMessageBox::information(0, "Debug loadPlugin",
		//						 "Plugin interface already indicates to a valid(?) instance\n"
		//						 "Memory leak?\n" + pluginModuleName,
		//						 QMessageBox::Ok);

		loader.setFileName(pluginsDir_.absoluteFilePath(pluginModuleName));
		pluginInstance = loader.instance();
	}
	//-- Elise plugin
	IPlugin* pluginInterface = qobject_cast<IPlugin*>(pluginInstance);
	if (!pluginInterface) {
		return 1;
	}

	//-- Load Elise plugin
	if (pluginInterface->Load(core))
		return 1;

	const PluginInfo* pluginInfo = pluginInterface->ElisePluginInfo();

	plugin->loaded = true;
	plugin->instance = pluginInstance;
	plugin->uuid = const_cast<QUuid*>(&(pluginInfo->uuid));

	//-- Register loaded interfaces
	const QSet<QUuid>* pluginInterfaces = pluginInterface->ElisePluginInterfaces();
	QSet<QUuid>::const_iterator i = pluginInterfaces->constBegin();
	QSet<QUuid>::const_iterator iEnd = pluginInterfaces->constEnd();
	while (i != iEnd) {
		interfaces_->insert(*i, pluginModuleName);
		++i;
	}

	//QMessageBox::information(0, "Debug", "loadPlugin " + pluginModuleName, QMessageBox::Ok);
	return 0;
}

int PluginLoader::unloadPlugin(const QString& pluginModuleName)
{
	QPluginLoader loader;
	Plugin* plugin = &(*plugins_)[pluginModuleName];

	if (!plugin->loaded)
		return 0;

	IPlugin* pluginInterface = qobject_cast<IPlugin*>(plugin->instance);

	//-- Call Elise plugin api unload
	if (pluginInterface->Unload())
		QMessageBox::critical(0, QStringLiteral("unloadPlugin error"),
							  QStringLiteral("Error while unloading plugin ") + pluginModuleName,
							  QMessageBox::Ok);

	//-- Remove the interfaces from the list
	const QSet<QUuid>* pluginInterfaces = pluginInterface->ElisePluginInterfaces();
	QSet<QUuid>::const_iterator i = pluginInterfaces->constBegin();
	QSet<QUuid>::const_iterator iEnd = pluginInterfaces->constEnd();
	while (i != iEnd) {
		interfaces_->take(*i);
		++i;
	}

	loader.setFileName(pluginsDir_.absoluteFilePath(pluginModuleName));
	//-- Free plugin
	loader.unload();

	plugin->instance = 0;
	plugin->loaded = false;
	plugin->uuid = 0;

	//QMessageBox::information(0, "Debug", "unloadPlugin " + pluginModuleName, QMessageBox::Ok);
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
		if (!isLoadingPluginDisabled(pluginModuleName)) {

			//-- Check existing interfaces
			if (isPluginLoadable(pluginModuleName))
				loadPlugin(pluginModuleName);
		}
		++iteratorPlugins;
	}
	return 0;
}

int PluginLoader::unloadAllPlugins()
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
	interfaces_->clear();

	return 0;
}

const QMap<QString, IDBPlugin*>* PluginLoader::getDBPlugins()
{
	QPluginLoader loader;
	QMap<QString, IDBPlugin*>* dbPlugins = new QMap<QString, IDBPlugin*>();
	QMap<QString, Plugin>::iterator iteratorPlugins = plugins_->begin();
	QMap<QString, Plugin>::iterator pluginsEnd = plugins_->end();
	IDBPlugin* pluginInterface = 0;

	while (iteratorPlugins != pluginsEnd) {
		if (!iteratorPlugins.value().loaded) {
			loader.setFileName(pluginsDir_.absoluteFilePath(iteratorPlugins.key()));
			pluginInterface = qobject_cast<IDBPlugin*>(loader.instance());
		} else {
			pluginInterface = qobject_cast<IDBPlugin*>(iteratorPlugins.value().instance);
		}
		if (pluginInterface)
			dbPlugins->insert(iteratorPlugins.key(), pluginInterface);
		else
			loader.unload();
		++iteratorPlugins;
	}

	if (dbPlugins->isEmpty()) {
		delete dbPlugins;
		return 0;
	}

	return dbPlugins;
}
