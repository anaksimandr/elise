#ifndef ELISE_API_DEFINES_H_
#define ELISE_API_DEFINES_H_

/*
 * Plugin types
 */
#define __Plugins_NoType			0 // any type of plugin which can be loaded more than one copy
#define __Plugins_DBPlugin			1 // DB plugins
#define __Plugins_SimplePlugin		2 // simple plugin
#define __Plugins_DeveloperPlugin	3 // developer plugin


/*
 *Names of the known services and hookable events
 */
#define __Core_Shutdown_service				"Core/Shutdown"
#define __Core_ChangeProfile_service		"Core/ChangeAcc"

#define __Folders_GetPluginsDir_service		"Folders/GetPluginsDir"
#define __Folders_GetProfilesDir_service	"Folders/GetProfilesDir"

#define __PluginLoader_IsPluginLoaded_service		"PluginLoader/IsPluginLoaded"

#define __Options_Show_service				"Options/Show"

#define __DB_WriteSetting_service		"DB/WriteSetting"
#define __DB_ReadSetting_service		"DB/ReadSetting"
#define __DB_DellSetting_service		"DB/DeleteSetting"

#define __Tray_AddMenuItem_service		"Tray/AddMenuItem" /* Adds item to tray context menu. */
#define __Tray_SetIcon_service			"Tray/SetIcon"

#define __CList_Show_service			"CList/Show"
#define __CList_Hide_service			"CList/Hide"

#define __Tests_TestService				"TESTPLUGIN_SERVICE"

/*
 * Names of the known hookable events
 */
#define __Core_Preshutdown				"Core/Preshutdown"

#define __PluginLoader_PluginLoaded		"PluginLoader/PluginLoaded"
#define __PluginLoader_PluginUnloaded	"PluginLoader/PluginUnloaded"
#define __PluginLoader_PluginsLoaded	"PluginLoader/PluginsLoaded"
//#define __PluginLoader_PluginsUnloaded	"PluginLoader/PluginsUnloaded"

#define __Options_Show_event			__Options_Show_service
#define __Options_Save_event			"Options/Save"
#define __Options_Close_event			"Options/Close"

#define __Tray_SingleClick_event		"Tray/SingleClick"
#define __Tray_DoubleClick_event		"Tray/DoubleClick"
#define __Tray_MiddleClick_event		"Tray/MiddleClick"

#define __CList_Show_event				__CList_Show_service
#define __CList_Hide_event				__CList_Hide_service

#endif // ELISE_API_DEFINES_H_
