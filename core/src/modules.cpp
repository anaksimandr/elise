#include "commonheaders.h"

extern int shutDown(intptr_t, intptr_t);

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

//QMap<QString, IPlugin*>* PluginLoader::loadablePlugins;
//QMap<QString, IDBPlugin*>*	PluginLoader::dbPlugins;
QMap<QString, IPlugin*>*	PluginLoader::plugins;

int LoadSystemModule()
{
	if (CreateServiceFunction(&SHUTDOWN_SERVICE, (ELISESERVICE)shutDown))
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

	//-- Now we will load the profile, do it befor loading plugins, because we must know which
	//-- plugins must be loaded for this profile.
	//-- First, we must get list of all available plugins to know which db plugins are exist.
	QMap<QString, IPlugin*>* loadablePlugins = new QMap<QString, IPlugin*>();
	QMap<QString, IDBPlugin*>* dbPlugins	= new QMap<QString, IDBPlugin*>();
	if (PluginLoader::getPluginsList(dbPlugins, loadablePlugins)) {
		QMessageBox qmes;
		qmes.setWindowTitle("getPluginsList error");
		qmes.setText("Failed to get plugins list or no one plugin was found.");
		qmes.exec();
		return 1;
	}
	AccountManager* manager =  new AccountManager(dbPlugins);
	if (manager->exec()) {
		loadablePlugins->~QMap();
		dbPlugins->~QMap();
		return 1;
	}


	//-- Loading plugins.
	//-- Note: trying access loadPlugins() without getPluginsList() cause a crash.

	//-- Try to load DB plugin
	if (PluginLoader::loadDBPlugin())
		return 1;
	return PluginLoader::loadPlugins(loadablePlugins);

	//-- Destroy temporary elements
	loadablePlugins->~QMap();
	dbPlugins->~QMap();

	return 0;
}

int UnloadDefaultModules()
{

	//-- Hide tray icon
	UnloadTrayModule();

	return 0;
}

int PluginLoader::getPluginsList(QMap<QString, IDBPlugin *>* dbPlugins,
								 QMap<QString, IPlugin *>* loadablePlugins)
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

	QPluginLoader loader;
	QObject* plugin = NULL;
	//-- Trying to load each file in dir as plugin
	foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
		loader.setFileName(pluginsDir.absoluteFilePath(fileName));
		plugin = loader.instance();
		if (plugin) {
			IPlugin* validPlugin = qobject_cast<IPlugin*>(plugin);
			if (validPlugin) {
				if (validPlugin->ElisePluginInfo(eliseVersion) != NULL)
					loadablePlugins->insert(fileName, validPlugin);
			}
			else {
				IDBPlugin* validDBPlugin = qobject_cast<IDBPlugin*>(plugin);
				if (validDBPlugin) {
					if (validPlugin->ElisePluginInfo(eliseVersion) != NULL)
						dbPlugins->insert(fileName, validDBPlugin);
				}
			}
		} //if plugin
	} //foreach

	return 0;
}

int PluginLoader::loadDBPlugin()
{
	//if (loadablePlugins->contains())
	//return 0;
}

int PluginLoader::loadPlugins(QMap<QString, IPlugin *>* loadablePlugins)
{
	QMapIterator<QString,IPlugin*> iter(*loadablePlugins);
	IPlugin* p;
	while (iter.hasNext()) {
		iter.next();
		p = iter.value();
		if (p->Load(&pluginLink))
			return 1;
	}

	return 0;
}
