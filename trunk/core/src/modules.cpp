#include "modules.h"
#include "elise.h"
#include "modules/pluginloader/pluginloader.h"
#include "modules/profilemanager.h"
#include "modules/tray.h"
#include "modules/options/options.h"


//-- Temporary module for tests
#include "tests.h"


namespace core
{

intptr_t ChangeProfile(intptr_t, intptr_t);
int LoadProfile(bool);

int LoadSystemModule()
{
	if (CreateServiceFunction(&SHUTDOWN_SERVICE, &shutDown))
		return 1;
	if (CreateServiceFunction(&CHANGEPROFILE_SERVICE, &ChangeProfile))
		return 1;
	//if (CreateHookableEvent(&hkevName))
		//return 1;

	return 0;
}


int LoadDefaultModules()
{
	if (LoadSystemModule())
		return 1;
	if (LoadOptionsModule())
		return 1;
	if (PluginLoader::LoadPluginLoader())
		return 1;
	if (LoadTrayModule())
		return 1;

	//-- 'true' means that this is launch of the application
	return LoadProfile(true);
}

int UnloadDefaultModules()
{

	//-- Hide tray icon
	UnloadTrayModule();

	return 0;
}

intptr_t ChangeProfile(intptr_t, intptr_t)
{
	//-- 'false' means that this is not launch of the application
	if (LoadProfile(false))
		return shutDown(-1, 0);

	return 0;
}

int LoadProfile(bool launchApp)
{
	//-- First, unload all plugins, if there is
	PluginLoader::unloadPlugins();

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
		//return shutDown(-1, 0);
	}

	ProfileManager* manager =  new ProfileManager(dbPlugins);

	int result = true;

	//-- Try to load default profile
	if (launchApp)
		result = manager->loadDefaultProfile();

	if (result) {
		//-- Show login window on fail
		if (manager->exec()) {
			manager->~ProfileManager();
			dbPlugins->~QMap();
			loadablePlugins->~QMap();
			return 1;
			//return shutDown(-1, 0);
		}
	}

	//-- Loading plugins.
	result = PluginLoader::loadPlugins(loadablePlugins);

	manager->~ProfileManager();
	loadablePlugins->~QMap();
	dbPlugins->~QMap();

	if (result)
		return 1;
		//return shutDown(-1, 0);

	//-- For test
	new QTestWindow();
	return 0;
}

} //namespace core
