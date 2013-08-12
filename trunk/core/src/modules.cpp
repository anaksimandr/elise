/*  Elise IM, free crossplatform IM client
	Copyright (C) 2012  Elise project

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "core.h"
#include "modules/pluginloader/pluginloader.h"
#include "modules/profilemanager.h"
#include "modules/tray.h"
#include "modules/options/options.h"
#include "modules/folders/folders.h"

ICore* core = 0;

int Core::loadCore()
{
	core = dynamic_cast<ICore*>(new Core());

	core->createServiceFunction(&kCoreShutdown_service, &shutdown);
	core->createServiceFunction(&kCoreChangeProfile_service, &loadProfile);
	core->createHookableEvent(&kCorePreshutdown);

	if (Folders::loadFolders())
		return 1;
	if (OptionsDialog::loadOptionsModule())
		return 1;
	if (PluginLoader::loadPluginLoader())
		return 1;
	if (EliseTray::loadTrayModule())
		return 1;

	return 0;
}

int Core::unloadCore()
{
	core->notifyEventHooks(&kCorePreshutdown, 0, 0);

	QApplication::processEvents();

	//-- First, unload all plugins, if there is
	PluginLoader::unloadPlugins();

	//-- Unload core modules
	EliseTray::unloadTrayModule();
	PluginLoader::unloadPluginLoader();
	OptionsDialog::unloadOptionsModule();
	Folders::unloadFolders();

	delete core;

	return 0;
}

int Core::loadProfile(intptr_t, intptr_t)
{
	//-- Clean hooks&services stack
	unloadCore();
	loadCore();

	//-- Load the profile, do it befor loading plugins.

	if (!PluginLoader::getAvailablePlugins()) {
		QMessageBox::critical(0,
						QStringLiteral("getAvailablePlugins error"),
						QStringLiteral("Failed to get plugins list or no one plugin was found."),
						QMessageBox::Cancel);
		return 1;
	}

	ProfileManager* manager =  new ProfileManager();

	int result = true;

	//-- Try to load default profile on start
	if (!profileLoaded)
		result = manager->loadDefaultProfile();

	//-- Show login window on loadDefaultProfile() fail
	if (result) {
		if (manager->exec()) {
			delete manager;
			return 1;
		}
	}

	profileLoaded = true;

	result = PluginLoader::loadPlugins();

	delete manager;

	return result;
}

