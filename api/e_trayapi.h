#ifndef ELISE_API_E_TRAYAPI_H_
#define ELISE_API_E_TRAYAPI_H_

class QLatin1String;

//-- Services
const QLatin1String	TRAY_ADD_MENUITEM	=	QLatin1String("Tray/AddMenuItem");	/* Adds item to tray context menu. */
const QLatin1String	TRAY_SET_ICON		=	QLatin1String("Tray/SetIcon");		/* */

//-- Hookable events
const QLatin1String	TRAY_SINGLECLICK	=	QLatin1String("Tray/SingleClick");
const QLatin1String	TRAY_DOUBLECLICK	=	QLatin1String("Tray/DoubleClick");
const QLatin1String	TRAY_MIDDLECLICK	=	QLatin1String("Tray/MiddleClick");

#endif // ELISE_API_E_TRAYAPI_H_
