#include "../../../../api/e_pluginapi.h"
#include "../../../../api/e_dbpluginapi.h"
#include "../../../../api/e_options.h"
#include "../../../../api/e_database.h"
#include "../options/options.h"
#include "coreapi.h"
#include "pluginloader.h"
#include "../../services.h"
#include "pluginloaderoptions.h"

CoreAPI						PluginLoader::coreAPI;
QDir						PluginLoader::pluginsDir;
QMap<QString, Plugin>*		PluginLoader::plugins = 0;
QMap<QUuid, QString>*		PluginLoader::interfaces = 0;

int PluginLoader::loadPluginLoader()
{
	pluginsDir = getPluginsDir();
	core::HookEvent(&OPTIONS_SHOW, &PluginLoaderOptions::createLoaderOptionsPage);

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
	loader.setFileName(pluginsDir.absoluteFilePath(pluginModuleName));
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
	if (!doNotUnload)
		loader.unload();
	return result;
}

const QMap<QString, Plugin>* PluginLoader::getAvailablePlugins()
{
	if (plugins == 0)
		plugins = new QMap<QString, Plugin>();

	if (interfaces == 0)
		interfaces = new QMap<QUuid, QString>();

	QPluginLoader loader;
	QObject* pluginInstance = 0;
	Plugin plugin;
	plugin.loaded = false;
	plugin.instance = 0;

	//-- First of all validate current list of plugins
	QMap<QString, Plugin>::iterator iteratorPlugins = plugins->begin();
	QMap<QString, Plugin>::iterator pluginsEnd = plugins->end();
	while (iteratorPlugins != pluginsEnd) {
		if (!pluginsDir.exists(iteratorPlugins.key()))
			iteratorPlugins = plugins->erase(iteratorPlugins);
		else
			++iteratorPlugins;
	}

	//-- Update the list of plugins
	QStringList files = pluginsDir.entryList(QDir::Files);
	QStringList::const_iterator i = files.constBegin();
	QStringList::const_iterator iEnd = files.constEnd();
	while (i != iEnd) {
		if (!plugins->contains(*i)) {
			loader.setFileName(pluginsDir.absoluteFilePath(*i));
			pluginInstance = loader.instance();
			if (pluginInstance) {
				IPlugin* validPlugin = qobject_cast<IPlugin*>(pluginInstance);
				//-- Note: qobject_cast<IPlugin*>(pluginObject) does not work for plugins that
				//-- inherited from interfaces which inherits IPlugin.
				if (validPlugin)
					plugins->insert(*i, plugin);
			}
			loader.unload();
		}
		++i;
	}

	if (plugins->isEmpty())
		return 0;

	return plugins;
}

bool PluginLoader::isLoadingPluginDisabled(const QString& pluginModuleName)
{
	QString module = QStringLiteral("core");
	QString setting = QStringLiteral("pluginDisabled") + pluginModuleName;
	bool result = 0;
	Setting* set = new Setting;
	set->contact = 0;
	set->qsModule = &module;
	set->qsSetting = &setting;
	set->var = new DBVariant;
	set->var->type = 0;
	if (!core::CallService(&DB_READSETTING, reinterpret_cast<intptr_t>(set), 0)) {
		result = static_cast<bool>(set->var->intValue);
	}
	delete set->var;
	delete set;
	return result;
	return 0;
}

bool PluginLoader::isPluginLoaded(const QString& pluginModuleName)
{
	//if (plugins->contains(pluginModuleName)) {
	const Plugin* p = &plugins->value(pluginModuleName);
	return p->loaded;
	//}
	//return false;
}

bool PluginLoader::isPluginLoadable(const QString& pluginModuleName)
{
	//if (pluginModuleName.isEmpty())
		//return false;

	const Plugin* p = &plugins->value(pluginModuleName);
	if (p->loaded)
		return false;

	QPluginLoader loader;
	loader.setFileName(pluginsDir.absoluteFilePath(pluginModuleName));
	//bool doNotUnload = loader.isLoaded();
	IPlugin* pluginInterface = qobject_cast<IPlugin*>(loader.instance());

	//-- Iterate all interfaces of current plugin
	const QSet<QUuid>* pluginInterfaces = pluginInterface->ElisePluginInterfaces();
	QSet<QUuid>::const_iterator i = pluginInterfaces->constBegin();
	QSet<QUuid>::const_iterator iEnd = pluginInterfaces->constEnd();
	bool result = true;
	while (i != iEnd) {
		//-- if one of the interfaces is already registered - break
		if (interfaces->contains(*i)) {
			result = false;
			break;
		}
		++i;
	}
	//if (!doNotUnload)
	loader.unload();

	return result;
}

int PluginLoader::loadDBPlugin(const QString& pluginModuleName,
							   const QMap<QString, IDBPlugin*> *dbPlugins)
{
	QMap<QString, IDBPlugin*>::const_iterator iteratorPlugins = dbPlugins->constBegin();
	QMap<QString, IDBPlugin*>::const_iterator pluginsEnd = dbPlugins->constEnd();
	QPluginLoader loader;

	while (iteratorPlugins != pluginsEnd) {
		loader.setFileName(pluginsDir.absoluteFilePath(pluginModuleName));

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
	Plugin* plugin = &(*plugins)[pluginModuleName];
	QObject* pluginInstance = plugin->instance;

	if (!pluginInstance) {
		QPluginLoader loader;
		//QMessageBox::information(0, "Debug loadPlugin",
		//						 "Plugin interface already indicates to a valid(?) instance\n"
		//						 "Memory leak?\n" + pluginModuleName,
		//						 QMessageBox::Ok);

		loader.setFileName(pluginsDir.absoluteFilePath(pluginModuleName));
		pluginInstance = loader.instance();
	}
	//-- Elise plugin
	IPlugin* pluginInterface = qobject_cast<IPlugin*>(pluginInstance);
	if (!pluginInterface) {
		return 1;
	}

	//-- Load Elise plugin
	if (pluginInterface->Load(&coreAPI))
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
		interfaces->insert(*i, pluginModuleName);
		++i;
	}

	//QMessageBox::information(0, "Debug", "loadPlugin " + pluginModuleName, QMessageBox::Ok);
	return 0;
}

int PluginLoader::unloadPlugin(const QString& pluginModuleName)
{
	QPluginLoader loader;
	Plugin* plugin = &(*plugins)[pluginModuleName];

#ifndef NDEBUG
	if (!plugin->instance)
		QMessageBox::information(0, "Debug unloadPlugin",
								 "Plugin interface does not indicates to a valid instance\n"
								 + pluginModuleName,
								 QMessageBox::Ok);
#endif

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
		interfaces->take(*i);
		++i;
	}

	loader.setFileName(pluginsDir.absoluteFilePath(pluginModuleName));
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
	if (plugins == 0)
		return 1;

	QString pluginModuleName;
	QMap<QString, Plugin>::iterator iteratorPlugins = plugins->begin();
	QMap<QString, Plugin>::iterator pluginsEnd = plugins->end();

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
    if (plugins == 0)
        return 0;

	//-- NOTE: we are not destroying QMap plugins here because we need it for new loading and
	//-- it will be automatically destroyed on fully exit
	QMap<QString, Plugin>::iterator iteratorPlugins = plugins->begin();
	QMap<QString, Plugin>::iterator pluginsEnd = plugins->end();
	while (iteratorPlugins != pluginsEnd) {
		unloadPlugin(iteratorPlugins.key());
		++iteratorPlugins;
	} //while
	plugins->clear();
	interfaces->clear();

	return 0;
}

const QMap<QString, IDBPlugin*>* PluginLoader::getDBPlugins()
{
	QPluginLoader loader;
	QMap<QString, IDBPlugin*>* dbPlugins = new QMap<QString, IDBPlugin*>();
	QMap<QString, Plugin>::iterator iteratorPlugins = plugins->begin();
	QMap<QString, Plugin>::iterator pluginsEnd = plugins->end();
	IDBPlugin* pluginInterface = 0;

	while (iteratorPlugins != pluginsEnd) {
		if (!iteratorPlugins.value().loaded) {
			loader.setFileName(pluginsDir.absoluteFilePath(iteratorPlugins.key()));
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
