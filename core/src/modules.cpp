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

	QDir pluginsDir = QDir(qApp->applicationDirPath());

	//-- Really not shure that this is needed
#if defined(Q_OS_WIN)
	if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
		pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
	if (pluginsDir.dirName() == "MacOS") {
		pluginsDir.cdUp();
		pluginsDir.cdUp();
		pluginsDir.cdUp();
	}
#endif

	pluginsDir.cd("Plugins");

	//-- Trying loading each file in dir as plugin
	foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
		QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
		QObject* plugin = loader.instance();
		if (plugin) {
			populateMenus(plugin);
			pluginFileNames += fileName;
		}
	}

	return 0;
}

int PluginLoader::loadPlugins()
{
	return 0;
}
