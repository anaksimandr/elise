#ifndef ELISE_API_DEFINES_H_
#define ELISE_API_DEFINES_H_

/*
 * Plugin types
 */
// 0 - any type of plugin which can be loaded more than one copy
// 1 - DB plugins
// 2 - simple plugin
// 3 - developer plugin



/*
 *Names of the known services and hookable events
 */
#define __Core_Shutdown_service				"System/Shutdown"
#define __Core_ChangeProfile_service		"System/ChangeAcc"
#define __Core_IsPluginLoaded_service		"Core/IsPluginLoaded"
#define __Core_GetPluginsDir_service		"Core/GetPluginsDir"
#define __Core_GetProfilesDir_service		"Core/GetProfilesDir"

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
#define __Options_Show_event			__Options_Show_service
#define __Options_Save_event			"Options/Save"
#define __Options_Close_event			"Options/Close"

#define __Tray_SingleClick_event		"Tray/SingleClick"
#define __Tray_DoubleClick_event		"Tray/DoubleClick"
#define __Tray_MiddleClick_event		"Tray/MiddleClick"

#define __CList_Show_event				__CList_Show_service
#define __CList_Hide_event				__CList_Hide_service

#endif // ELISE_API_DEFINES_H_
