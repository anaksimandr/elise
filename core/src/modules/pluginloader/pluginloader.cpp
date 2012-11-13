
#include "../../../../api/e_pluginapi.h"
#include "../../../../api/e_dbpluginapi.h"
#include "../../../../api/e_options.h"
#include "coreapi.h"
#include "pluginloader.h"
#include "../../services.h"
#include "pluginloaderoptions.h"

CoreAPI						PluginLoader::coreAPI;
QMap<QString, IPlugin*>*	PluginLoader::plugins = 0;
//QMap<QUuid, QString>*		PluginLoader::interfaces = 0;
LoadedDBPlugin				PluginLoader::loadedDBPlugin;

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

int PluginLoader::getAvailablePlugins(QMap<QString, IDBPlugin*>* dbPlugins,
									  QMap<QString, IPlugin*>* loadablePlugins)
{
	QDir pluginsDir = getPluginsDir();
	QPluginLoader loader;
	QObject* plugin = NULL;
	//-- Trying to load each file in dir as plugin
	foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
		loader.setFileName(pluginsDir.absoluteFilePath(fileName));
		plugin = loader.instance();
		if (plugin) {
			IPlugin* validPlugin = qobject_cast<IPlugin*>(plugin);
			//-- Elise plugin
			if (validPlugin) {
				//if (validPlugin->ElisePluginInfo(Core::g_eliseVersion) != NULL)
				loadablePlugins->insert(fileName, validPlugin);
			}
			//-- Elise DB plugin
			else {
				IDBPlugin* validDBPlugin = qobject_cast<IDBPlugin*>(plugin);
				if (validDBPlugin) {
					//if (validDBPlugin->ElisePluginInfo(Core::g_eliseVersion) != NULL)
					dbPlugins->insert(fileName, validDBPlugin);
				}
			}
		} //if plugin
	} //foreach
	return 0;
}

int PluginLoader::loadDBPlugin(QString pluginName, IDBPlugin* dbPlugin)
{
	loadedDBPlugin.name = pluginName;
	loadedDBPlugin.plugin = dbPlugin;
	//QMessageBox::information(0, "Debug", loadedDBPlugin.name, QMessageBox::Ok);
	return dbPlugin->Load(&coreAPI);
}

int PluginLoader::loadPlugins(QMap<QString, IPlugin*>* loadablePlugins)
{
	if (plugins == 0)
		plugins = new QMap<QString, IPlugin*>();
	QMapIterator<QString, IPlugin*> iter(*loadablePlugins);
	IPlugin* p;
	while (iter.hasNext()) {
		iter.next();
		p = iter.value();
		if (p->Load(&coreAPI))
			return 1;
		plugins->insert(iter.key(), p);
	}
	return 0;
}

int PluginLoader::unloadPlugins()
{
    //-- Launching app
    if (plugins == 0)
        return 0;

	//-- NOTE: we are not destroying QMap plugins here because we need it for new loading and
	//-- it will be automatically destroyed on fully exit
	QDir pluginsDir = getPluginsDir();
	QPluginLoader loader;
	QMapIterator<QString, IPlugin*> iter(*plugins);
	while (iter.hasNext()) {
		iter.next();
		//-- Call Elise plugin api unload
		if (iter.value()->Unload())
			QMessageBox::critical(0, QStringLiteral("unloadPlugins error"),
								  QStringLiteral("Error while unloading plugin ") + iter.key(),
								  QMessageBox::Ok);
		loader.setFileName(pluginsDir.absoluteFilePath(iter.key()));
		//-- Free plugin
		loader.unload();
		//-- Remove it from list
		plugins->remove(iter.key());
	} //while
	//-- Unload DB plugin
	if (loadedDBPlugin.plugin->Unload())
		QMessageBox::critical(0, QStringLiteral("unloadPlugins error"),
							 QStringLiteral("Error while unloading DB plugin") + loadedDBPlugin.name,
							  QMessageBox::Ok);
	loader.setFileName(pluginsDir.absoluteFilePath(loadedDBPlugin.name));
	loader.unload();
	return 0;
}
