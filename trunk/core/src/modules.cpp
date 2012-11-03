#include "commonheaders.h"

//-- Temporary module for tests
#include "tests.h"

extern int shutDown(intptr_t, intptr_t);
int ChangeProfile(intptr_t, intptr_t);

int LoadSystemModule()
{
	if (CreateServiceFunction(&SHUTDOWN_SERVICE, (ELISESERVICE)shutDown))
		return 1;
	if (CreateServiceFunction(&CHANGEPROFILE_SERVICE, (ELISESERVICE)ChangeProfile))
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
	QMap<QString, IDBPlugin*>* dbPlugins	= new QMap<QString, IDBPlugin*>();

	if (PluginLoader::getAvailablePlugins(dbPlugins, loadablePlugins)
			|| loadablePlugins->count() == 0
			|| dbPlugins->count() == 0)
	{
		QMessageBox::critical(0,
						QStringLiteral("getAvailablePlugins error"),
						QStringLiteral("Failed to get plugins list or no one plugin was found."),
						QMessageBox::Cancel);
		dbPlugins->~QMap();
		loadablePlugins->~QMap();
		return 1;
	}

	//-- 'false' indicates that this is not changeProfile service
	ProfileManager* manager =  new ProfileManager(dbPlugins, false);

	if (PluginLoader::callLoginWindow(loadablePlugins, 1)) {
		loadablePlugins->~QMap();
		return 1;
	}

	//-- Loading plugins.
	int res = PluginLoader::loadPlugins(loadablePlugins);

	loadablePlugins->~QMap();
	dbPlugins->~QMap();

	if (res)
		return 1;

	return 0;
}

int ChangeProfile(intptr_t, intptr_t)
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
