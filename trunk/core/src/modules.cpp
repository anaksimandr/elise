#include "commonheaders.h"

//-- Temporary module for tests
#include "tests.h"

extern int shutDown(intptr_t, intptr_t);
int ChangeAccount(intptr_t, intptr_t);

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
