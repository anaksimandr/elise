#include "commonheaders.h"

/*int LoadSystemModule()
{
	//if (CreateHookableEvent(&hkevName))
		//return 1;

	return 0;
}*/


int LoadDefaultModules()
{
	//if (LoadSystemModule())
		//return 1;
	if (LoadTrayModule())
		return 1;

	return 0;
}

int UnloadDefaultModules()
{

	//-- Hide tray icon
	UnloadTrayModule();

	return 0;
}

PluginLoader::PluginLoader()
{

}

PluginLoader::~PluginLoader()
{

}

int PluginLoader::getPluginsList()
{
	//-- Static plugins? not intresting
	//foreach (QObject *plugin, QPluginLoader::staticInstances())
	//	populateMenus(plugin);

	loadablePlugins = new QStringList();

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
	//-- Trying loading each file in dir as plugin
	foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
		loader.setFileName(pluginsDir.absoluteFilePath(fileName));
		plugin = loader.instance();
		if (plugin) {
			IPlugin* validPlugin = qobject_cast<IPlugin*>(plugin);
			if (validPlugin) {
				if (validPlugin->ElisePluginInfo() != NULL) {
					loadablePlugins[fileName] = plugin;
				}
			} //if validPlugin
		} //if plugin
	} //foreach

	return 0;
}

int PluginLoader::loadPlugins()
{
	foreach(IPlugin* plugin, loadablePlugins) {
		plugin->Load();
	}

	return 0;
}
