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

QMap<QString, IPlugin*>* PluginLoader::loadablePlugins;

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

	//-- Loading plugins.
	//-- Note: trying access loadPlugins() without getPluginsList() cause a crash.
	if (!PluginLoader::getPluginsList()) {
		PluginLoader::loadPlugins();
	}
	else {
		//-- TODO: Fix this
		QMessageBox qmes;
		qmes.setText("getPluginsList FAILED!");
		qmes.exec();
	}
	return 0;
}

int UnloadDefaultModules()
{

	//-- Hide tray icon
	UnloadTrayModule();

	return 0;
}

int PluginLoader::getPluginsList()
{
	//-- Static plugins? not intresting
	//foreach (QObject *plugin, QPluginLoader::staticInstances())
	//	populateMenus(plugin);

	loadablePlugins = new QMap<QString,IPlugin*>();

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

	pluginsDir.cd("plugins");

	QPluginLoader loader;
	QObject* plugin = NULL;
	//-- Trying loading each file in dir as plugin
	foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
		loader.setFileName(pluginsDir.absoluteFilePath(fileName));
		plugin = loader.instance();
		if (plugin) {
			IPlugin* validPlugin = qobject_cast<IPlugin*>(plugin);
			if (validPlugin) {
				if (validPlugin->ElisePluginInfo(eliseVersion) != NULL)
					loadablePlugins->insert(fileName, validPlugin);
			} //if validPlugin
		} //if plugin
	} //foreach

	return 0;
}

int PluginLoader::loadPlugins()
{
	QMapIterator<QString,IPlugin*> iter(*loadablePlugins);
	IPlugin* p;
	while (iter.hasNext()) {
		iter.next();
		p = iter.value();
		p->Load(&pluginLink);
	}

	loadablePlugins->~QMap();

	return 0;
}
