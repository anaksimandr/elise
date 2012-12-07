#include "core.h"
#include "modules/pluginloader/pluginloader.h"
#include "modules/profilemanager.h"
#include "modules/tray.h"
#include "modules/options/options.h"


//-- Temporary module for tests
#include "tests.h"

ICore* core = 0;

int Core::loadCore()
{
	core = dynamic_cast<ICore*>(new Core());

	if (core->createServiceFunction(&kShutdown_service, &shutDown))
		return 1;
	if (core->createServiceFunction(&kChangeProfile_service, &changeProfile))
		return 1;
	if (OptionsDialog::loadOptionsModule())
		return 1;
	if (PluginLoader::loadPluginLoader())
		return 1;
	if (LoadTrayModule())
		return 1;

	return 0;
}

int Core::unloadCore()
{
	UnloadTrayModule();
	OptionsDialog::unloadOptionsModule();
	//-- This will be cleaned during the destruction of the core
	//destroyServiceFunction(&kShutdown_service);
	//destroyServiceFunction(&kChangeProfile_service);

	delete core;

	return 0;
}


/*int Core::launchElise()
{
	if (Core::loadCore())
		return 1;

	//-- 'true' means that this is launch of the application
	return Core::loadProfile(true);
}

/*int Core::unloadDefaultModules()
{

	//-- Hide tray icon
	UnloadTrayModule();

	unloadCore();

	return 0;
}*/

int Core::loadProfile(bool launchApp)
{
	//-- First, unload all plugins, if there is
	PluginLoader::unloadAllPlugins();
	//-- Clean hooks&services stack
	unloadCore();
	loadCore();

	//-- Now we will load the profile, do it befor loading plugins, because we must know which
	//-- plugins must be loaded for this profile.

	if (!PluginLoader::getAvailablePlugins()) {
		QMessageBox::critical(0,
						QStringLiteral("getAvailablePlugins error"),
						QStringLiteral("Failed to get plugins list or no one plugin was found."),
						QMessageBox::Cancel);
		return 1;
	}

	const QMap<QString, IDBPlugin*>* dbPlugins = PluginLoader::getDBPlugins();

	if (!dbPlugins) {
		QMessageBox::critical(0,
						QStringLiteral("getDBPlugins error"),
						QStringLiteral("Failed to get DBPlugins or no one DBPlugin was found."),
						QMessageBox::Cancel);
		delete dbPlugins;
		return 1;
	}

	ProfileManager* manager =  new ProfileManager(dbPlugins);

	int result = true;

	//-- Try to load default profile
	if (launchApp)
		result = manager->loadDefaultProfile();

	if (result) {
		//-- Show login window on loadDefaultProfile fail
		if (manager->exec()) {
			delete manager;
			delete dbPlugins;
			return 1;
			//return shutDown(-1, 0);
		}
	}

	//-- Loading plugins.
	result = PluginLoader::loadPlugins();

	delete manager;
	delete dbPlugins;

	if (result)
		return 1;
		//return shutDown(-1, 0);

	//-- For test
	new QTestWindow();
	return 0;
}

