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

	return 0;
}

int PluginLoader::loadPlugins()
{
	return 0;
}
