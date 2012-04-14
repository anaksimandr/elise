#include "commonheaders.h"

//-- Temporary module for tests
#include "tests.h"

extern int shutDown(intptr_t, intptr_t);
int ChangeAccount(intptr_t, intptr_t);

const EVersion eliseVersion = {0,0,0,1};
const PLUGINLINK pluginLink = {
		&CreateHookableEvent,
		&DestroyHookableEvent,
		&NotifyEventHooks,
		&HookEvent,
		&UnhookEvent,
		&CreateServiceFunction,
		&DestroyServiceFunction,
		&CallService,
		&ServiceExists
};

QMap<QString, IPlugin*>*	PluginLoader::plugins = 0;

LOADEDDBPLUGIN PluginLoader::loadedDBPlugin;

int LoadSystemModule()
{
	if (CreateServiceFunction(&SHUTDOWN_SERVICE, (ELISESERVICE)shutDown))
		return 1;
	if (CreateServiceFunction(&CHANGEACC_SERVICE, (ELISESERVICE)ChangeAccount))
		return 1;
	//if (CreateHookableEvent(&hkevName))
		//return 1;

	return 0;
}


int LoadDefaultModules()
{
	if (LoadSystemModule())
		return 1;
	if (LoadTrayModule())
		return 1;
	if (LoadOptionsModule())
		return 1;

	//-- Now we will load the profile, do it befor loading plugins, because we must know which
	//-- plugins must be loaded for this profile.
	QMap<QString, IPlugin*>* loadablePlugins = new QMap<QString, IPlugin*>();
	if (PluginLoader::callLoginWindow(loadablePlugins, 1)) {
		loadablePlugins->~QMap();
		return 1;
	}

	//-- Loading plugins.
	int res = PluginLoader::loadPlugins(loadablePlugins);

	loadablePlugins->~QMap();

	if (res)
		return 1;

	return 0;
}

int PluginLoader::callLoginWindow(QMap<QString, IPlugin*>* loadablePlugins, bool first)
{
	//-- First, we must get list of all available plugins to know which db plugins are exist.
	QMap<QString, IDBPlugin*>* dbPlugins	= new QMap<QString, IDBPlugin*>();
	if (PluginLoader::getAvailablePlugins(dbPlugins, loadablePlugins)
			|| loadablePlugins == 0
			|| loadablePlugins->count() == 0)
	{
		QMessageBox::critical(0, "getAvailablePlugins error",
							  "Failed to get plugins list or no one plugin was found.",
							  QMessageBox::Cancel);
		dbPlugins->~QMap();
		return 1;
	}
	//-- Check for DB plugins
	if (dbPlugins == 0 || dbPlugins->count() == 0) {
		QMessageBox::critical(0, "Error",
							  "No one DB plugin was found.",
							  QMessageBox::Cancel);
		dbPlugins->~QMap();
		return 1;
	}
	AccountManager* manager =  new AccountManager(dbPlugins);
	if (first) {
		//-- Startup
		if (manager->loadDefault()) {
			//-- If load default fails then show login window
			if (manager->exec()) {
				dbPlugins->~QMap();
				return 1;
			}
		}
	}
	else
		//-- Change account
		if (manager->exec()) {
			dbPlugins->~QMap();
			return 1;
		}

	dbPlugins->~QMap();
	return 0;
}

int ChangeAccount(intptr_t, intptr_t)
{
	//-- First, unload all plugins
	PluginLoader::unloadPlugins();

	//-- New login
	QMap<QString, IPlugin*>* loadablePlugins = new QMap<QString, IPlugin*>();
	if (PluginLoader::callLoginWindow(loadablePlugins, 0)) {
		loadablePlugins->~QMap();
		return shutDown(-1, 0);
	}

	//-- Loading plugins.
	int res = PluginLoader::loadPlugins(loadablePlugins);

	loadablePlugins->~QMap();

	if (res)
		return shutDown(-1, 0);

	//-- For test
	new QTestWindow();
	return 0;
}

int UnloadDefaultModules()
{

	//-- Hide tray icon
	UnloadTrayModule();

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
				if (validPlugin->ElisePluginInfo(eliseVersion) != NULL)
					loadablePlugins->insert(fileName, validPlugin);
			}
			//-- Elise DB plugin
			else {
				IDBPlugin* validDBPlugin = qobject_cast<IDBPlugin*>(plugin);
				if (validDBPlugin) {
					if (validDBPlugin->ElisePluginInfo(eliseVersion) != NULL)
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
	return dbPlugin->Load(&pluginLink);
}

int PluginLoader::loadPlugins(QMap<QString, IPlugin *>* loadablePlugins)
{
	if (plugins == 0)
		plugins = new QMap<QString, IPlugin*>();
	QMapIterator<QString, IPlugin*> iter(*loadablePlugins);
	IPlugin* p;
	while (iter.hasNext()) {
		iter.next();
		p = iter.value();
		if (p->Load(&pluginLink))
			return 1;
		plugins->insert(iter.key(), p);
	}
	return 0;
}

int PluginLoader::unloadPlugins()
{
	//-- NOTE: we are not destroying QMap plugins here because we need it for new loading and
	//-- it will be automatically destroyed on fully exit
	QDir pluginsDir = getPluginsDir();
	QPluginLoader loader;
	QMapIterator<QString, IPlugin*> iter(*plugins);
	while (iter.hasNext()) {
		iter.next();
		//-- Call Elise plugin api unload
		if (iter.value()->Unload())
			QMessageBox::critical(0, "unloadPlugins error",
								  "Error while unloading plugin " + iter.key() + ".",
								  QMessageBox::Ok);
		loader.setFileName(pluginsDir.absoluteFilePath(iter.key()));
		//-- Free plugin
		loader.unload();
		//-- Remove it from list
		plugins->remove(iter.key());
	} //while
	//-- Unload DB plugin
	if (loadedDBPlugin.plugin->Unload())
		QMessageBox::critical(0, "unloadPlugins error",
							 "Error while unloading DB plugin" + loadedDBPlugin.name + ".",
							  QMessageBox::Ok);
	loader.setFileName(pluginsDir.absoluteFilePath(loadedDBPlugin.name));
	loader.unload();
	return 0;
}
