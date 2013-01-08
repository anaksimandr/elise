#ifndef ELISE_API_DEFINES_H_
#define ELISE_API_DEFINES_H_

/*
 * Replaceable plugin interfaces.
 * This is unique IDs for standart interfaces. If your plugin implements one of standart
 * interfaces it must return this interface in IPlugin::ElisePluginInterfaces(void) method.
 * Note: other interfaces must be returned too.
 * Note: DBPlugin should not implements any other interfaces.
 * Note: at the same time only one plugin can implement these interfaces.
 */
#define __UUID_Database			"{4df3e270-fb8b-4654-9271-2f0f31e0eb84}"
#define __UUID_Clist			"{88441947-8188-474e-ae18-bb2f3795d4fe}"
//#define __UUID_ChatWindow		"{29fbc48a-bf3e-4fdd-a537-35ad427da091}"
//#define __UUID_ChatLog			"{d6f1dce9-2c88-495d-b89c-9fb147f4863d}"

/*
 * These interfaces allows more than one plugin to implement it at the same time.
 */
#define __UUID_TestPlugin		"{a3c43dc8-4bb0-454d-baca-7682f924115c}"

/*
 *Names of the known services and hookable events
 */
#define __Core_Shutdown_service				"System/Shutdown"
#define __Core_ChangeProfile_service		"System/ChangeAcc"
#define __Core_IsPluginLoaded_service		"Core/IsPluginLoaded"
#define __Core_GetPluginInterfaces_service	"Core/GetPluginInterfaces"

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
