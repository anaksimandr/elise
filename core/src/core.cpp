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
#include "modules/profilemanager.h"
#include "modules/tray.h"
#include "modules/folders/folders.h"
#include "modules/options/options.h"
#include "modules/pluginloader/pluginloader.h"

const QLatin1String	g_kCoreShutdown_service			= QLatin1String(__Core_Shutdown_service);
const QLatin1String	g_kCoreChangeProfile_service	= QLatin1String(__Core_ChangeProfile_service);
const QLatin1String	g_kCorePreshutdown				= QLatin1String(__Core_Preshutdown);
const QLatin1String	g_kDBWriteSetting_service		= QLatin1String(__DB_WriteSetting_service);
const QLatin1String	g_kDBReadSetting_service		= QLatin1String(__DB_ReadSetting_service);
const QLatin1String	g_kDBDellSetting_service		= QLatin1String(__DB_DellSetting_service);

//ICore*	Core::core_ = 0;
//CoreDestroyer Core::destroyer_;
ICore*		g_core = 0;

QEvent::Type g_shutdownEventType = static_cast<QEvent::Type>(QEvent::registerEventType());
QEvent::Type g_changeProfileEventType = static_cast<QEvent::Type>(QEvent::registerEventType());

void Core::initialize()
{
	Core* core = new Core();
	g_core = dynamic_cast<ICore*>(core);

	if (core->loadProfile())
		core->shutdown(1);
}

void Core::shutdown(int result)
{
	unloadCore();

	QApplication::exit(result);
}

int Core::loadCore()
{
	createServiceFunction(&g_kCoreShutdown_service, &shutdownService);
	createServiceFunction(&g_kCoreChangeProfile_service, &changeProfileService);
	createHookableEvent(&g_kCorePreshutdown);

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
	notifyEventHooks(&g_kCorePreshutdown, 0, 0);

	QApplication::processEvents();

	//-- First, unload all plugins, if there is
	PluginLoader::unloadPlugins();

	//-- Unload core modules
	EliseTray::unloadTrayModule();
	PluginLoader::unloadPluginLoader();
	OptionsDialog::unloadOptionsModule();
	Folders::unloadFolders();

	qmapHooks_.clear();
	qmapServices_.clear();

	return 0;
}

int Core::loadProfile()
{
	//-- Clean hooks&services stack
	if (profileLoaded_)
		unloadCore();

	if (loadCore())
		return 1;

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
	if (!profileLoaded_)
		result = manager->loadDefaultProfile();

	//-- Show login window on loadDefaultProfile() fail
	if (result) {
		if (manager->exec()) {
			delete manager;
			return 1;
		}
	}

	profileLoaded_ = true;

	result = PluginLoader::loadPlugins();

	delete manager;

	return result;
}

int Core::changeProfileService(intptr_t, intptr_t)
{
	QApplication::postEvent(g_core, new QChangeProfileEvent());

	return 0;
}

int Core::shutdownService(intptr_t, intptr_t)
{
	QApplication::postEvent(g_core, new QShutdownEvent());

	return 0;
}

void Core::customEvent(QEvent* event)
{
	if (event->type() == g_changeProfileEventType) {
		loadProfile();
	}
	else if (event->type() == g_shutdownEventType) {
		shutdown();
	}
}

