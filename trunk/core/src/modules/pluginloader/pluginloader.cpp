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
QMap<QString, Plugin>*		PluginLoader::plugins = 0;
QMap<QUuid, QString>*		PluginLoader::interfaces = 0;
//LoadedDBPlugin				PluginLoader::loadedDBPlugin;

int PluginLoader::LoadPluginLoader()
{
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

int PluginLoader::getAvailablePlugins()
{
	if (plugins == 0)
		plugins = new QMap<QString, Plugin>();

	QDir pluginsDir = getPluginsDir();
	QPluginLoader loader;
	QObject* pluginObject = 0;
	Plugin plugin;
	plugin.loaded = false;
	plugin.loadable = true;
	//-- Trying to load each file in dir as plugin
	foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
		loader.setFileName(pluginsDir.absoluteFilePath(fileName));
		pluginObject = loader.instance();
		if (pluginObject) {
			IPlugin* validPlugin = qobject_cast<IPlugin*>(pluginObject);
			//-- Note: qobject_cast<IPlugin*>(pluginObject) does not work for plugins that
			//-- inherited from interfaces which inherits IPlugin.
			//-- Elise plugin
			if (validPlugin) {
				plugin.pluginInterface = validPlugin;
				plugins->insert(fileName, plugin);
			} else {
				IDBPlugin* validDBPlugin = qobject_cast<IDBPlugin*>(pluginObject);
				if (validDBPlugin) {
					plugin.pluginInterface = dynamic_cast<IPlugin*>(validDBPlugin);
					plugins->insert(fileName, plugin);
				}
			}
		} //if plugin
	} //foreach

	if (plugins->isEmpty())
		return 1;

	return 0;
}

bool PluginLoader::loadingPluginDisabled(const QString& plugin)
{
	QString module = QStringLiteral("core");
	QString setting = QStringLiteral("pluginDisabled") + plugin;
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
}

int PluginLoader::loadDBPlugin(QString pluginName)
{
	//loadedDBPlugin.name = pluginName;
	//loadedDBPlugin.plugin = dbPlugin;
	//QMessageBox::information(0, "Debug", loadedDBPlugin.name, QMessageBox::Ok);
	Plugin* plugin = &(*plugins)[pluginName];
	if (plugin->pluginInterface->Load(&coreAPI))
		return 1;
	plugin->loaded = true;
	return 0;
}

int PluginLoader::loadPlugins()
{
	if (plugins == 0)
		return 1;

	if (interfaces == 0)
		interfaces = new QMap<QUuid, QString>();

	const Plugin* p;
	const QSet<QUuid>* pluginInterfaces;

	QMap<QString, Plugin>::const_iterator iterPlugins = plugins->constBegin();
	QMap<QString, Plugin>::const_iterator pluginsEnd = plugins->constEnd();

	while (iterPlugins != pluginsEnd) {
		p = &iterPlugins.value();

		//-- If plugin disabled by user earlier - break
		if (loadingPluginDisabled(iterPlugins.key()))
			continue;

		pluginInterfaces = p->pluginInterface->ElisePluginInterfaces();


		//-- Iterate all interfaces of current plugin
		QSet<QUuid>::const_iterator i = pluginInterfaces->constBegin();
		QSet<QUuid>::const_iterator iEnd = pluginInterfaces->constEnd();
		bool dontLoadPlugin = false;
		while (i != iEnd) {
			//-- if one of the interfaces is already registered - break
			if (interfaces->contains(*i))
				dontLoadPlugin = true;
			++i;
		}
		if (dontLoadPlugin)
			continue;

		if (p->pluginInterface->Load(&coreAPI))
			return 1;

		//-- Register loaded interfaces
		i = pluginInterfaces->constBegin();
		while (i != iEnd) {
			interfaces->insert(*i, iterPlugins.key());
			//QMessageBox::information(0, iterPlugins.key(), (*i).toString(), QMessageBox::Ok);
			++i;
		}
	++iterPlugins;
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
	QDir pluginsDir = getPluginsDir();
	QPluginLoader loader;
	QMapIterator<QString, Plugin> iter(*plugins);
	while (iter.hasNext()) {
		iter.next();
		//-- Call Elise plugin api unload
		if (iter.value().pluginInterface->Unload())
			QMessageBox::critical(0, QStringLiteral("unloadPlugins error"),
								  QStringLiteral("Error while unloading plugin ") + iter.key(),
								  QMessageBox::Ok);
		loader.setFileName(pluginsDir.absoluteFilePath(iter.key()));
		//-- Free plugin
		loader.unload();
		//-- Remove it from list
		//plugins->remove(iter.key());
	} //while
	plugins->clear();
	interfaces->clear();
	//-- Unload DB plugin
	//if (0)//loadedDBPlugin.plugin->Unload())
	//	QMessageBox::critical(0, QStringLiteral("unloadPlugins error"),
	//						QStringLiteral("Error while unloading DB plugin") + loadedDBPlugin.name,
	//						QMessageBox::Ok);
	//loader.setFileName(pluginsDir.absoluteFilePath(loadedDBPlugin.name));
	//loader.unload();
	return 0;
}

const QMap<QString, IDBPlugin*>* PluginLoader::getDBPlugins()
{
	QMap<QString, IDBPlugin*>* dbPlugins = new QMap<QString, IDBPlugin*>();
	QMapIterator<QString, Plugin> iter(*plugins);
	while (iter.hasNext()) {
		iter.next();
		IDBPlugin* pl = dynamic_cast<IDBPlugin*>(iter.value().pluginInterface);
		if (pl)
			dbPlugins->insert(iter.key(), pl);
	}
	if (dbPlugins->isEmpty()) {
		delete dbPlugins;
		return 0;
	}

	return dbPlugins;
}
