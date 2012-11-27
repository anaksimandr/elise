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
	if (OptionsDialog::loadOptionsModule())
		return 1;
	if (PluginLoader::loadPluginLoader())
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
	PluginLoader::unloadAllPlugins();

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

} //namespace core
