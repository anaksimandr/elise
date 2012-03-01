#ifndef SERVICES_H
#define SERVICES_H

#include <QMutex>
#include <QMap>
#include <QWidget>
class QTestWindow;
#include "tests.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//-- Constants definitions --///////////////////////////////////////////////////////////////////////

//typedef void* HANDLE;

//#ifdef _WIN64
//    #define SERVICE_NOTFOUND      ((INT_PTR)0x8000000000000000)
//#else
	#define SERVICE_NOTFOUND      ((int)0x80000000)
//#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
//-- Names of system services and events --/////////////////////////////////////////////////////////

//-- Services
const QString	SHUTDOWN_SERVICE	=	"SHUTDOWN_SERVICE";	/* Call this method to close Elise. */
const QString	TRAY_ADD_MENUITEM	=	"TRAY_ADD_MENUITEM";/* Adds item to tray context menu. */

//-- Hookable events
const QString	TRAY_SINGLECLICK	=	"TRAY_SINGLECLICK";	/* The system tray entry was clicked. */
const QString	TRAY_DOUBLECLICK	=	"TRAY_DOUBLECLICK";	/* The system tray entry was double clicked. */
const QString	TRAY_MIDDLECLICK	=	"TRAY_MIDDLECLICK";	/* The system tray entry was clicked with the middle mouse button. */

////////////////////////////////////////////////////////////////////////////////////////////////////
//-- Service and hook definitions --////////////////////////////////////////////////////////////////

//-- Hook functions
typedef int (*ELISEHOOK)(uintptr_t,uintptr_t);
//typedef int (*ELISEHOOKPARAM)(uintptr_t,uintptr_t,uintptr_t);
//typedef int (*ELISEHOOKOBJ)(void*,uintptr_t,uintptr_t);
//typedef int (*ELISEHOOKOBJPARAM)(void*,uintptr_t,uintptr_t,uintptr_t);

//-- Services functions
typedef intptr_t (*ELISESERVICE)(uintptr_t,uintptr_t);								//-- type = 0
//typedef void (QTestWindow::* ptToWidgetMemb)();
//typedef intptr_t (*ELISESERVICEWIDG)(QWidget*, ptToWidgetMemb);			//-- type = 1
//typedef intptr_t (*ELISESERVICEPARAM)(uintptr_t,uintptr_t,uintptr_t);
//typedef intptr_t (*ELISESERVICEOBJ)(void*,uintptr_t,uintptr_t);
//typedef intptr_t (*ELISESERVICEOBJPARAM)(void*,uintptr_t,uintptr_t,uintptr_t);

//-- Hooks
typedef struct
{
	//void* hOwner;
	int num;
	int type;
	union {
		struct {
			union {
				ELISEHOOK pfnHook;
				//ELISEHOOKPARAM pfnHookParam;
				//ELISEHOOKOBJ pfnHookObj;
				//ELISEHOOKOBJPARAM pfnHookObjParam;
			};
			//void* object;
			//intptr_t lParam;
		};
		//struct {
		//	void* hwnd;
		//	uint message;
		//};
	}; // union
} THookSubscriber;

typedef struct
{
	//char name[MAXMODULELABELLENGTH];
	//ModuleLable name;
	//int  id;
	int  subscriberCount;
	//THookSubscriber* subscriber;
	QMap<int, THookSubscriber*>* qmapSubscribers;
	//ELISEHOOK pfnHook;
	QMutex* qmutexHook;
} THookEvent;

//-- Use this to store information about your hooks and to call UnhookEvent function
typedef struct
{
	QString* name;
	int num;
} THook;

//-- Services
typedef struct
{
	//DWORD nameHash;
	//HINSTANCE hOwner;
	int type;
	union {
		ELISESERVICE pfnService;
		//ELISESERVICEWIDG pfnServiceWidg;
		//ELISESERVICEPARAM pfnServiceParam;
		//MIRANDASERVICEOBJ pfnServiceObj;
		//MIRANDASERVICEOBJPARAM pfnServiceObjParam;
	};
	//int flags;
	//LPARAM lParam;
	//void* object;
	//char name[1];
} TService;

////////////////////////////////////////////////////////////////////////////////////////////////////
//-- Hook functions --//////////////////////////////////////////////////////////////////////////////

int CreateHookableEvent(const QString* name);

int DestroyHookableEvent(const QString* name);

int NotifyEventHooks(const QString* name, uintptr_t wParam, uintptr_t lParam );

int HookEventInt(const QString* name, ELISEHOOK hookProc);

int UnhookEvent(const THook hook);

////////////////////////////////////////////////////////////////////////////////////////////////////
//-- Service functions --///////////////////////////////////////////////////////////////////////////

/* CreateServiceFunction
  Adds a new service function called 'name' to the global list. Service function pointers are
  destroyed automatically on exit, but can be removed from the list earlier using
	  DestroyServiceFunction()
  serviceProc is defined by the caller as
	  int ServiceProc(uintptr_t wParam, uintptr_t lParam)
  where the creator publishes the meanings of wParam, lParam and the return value.
  Service functions must not return SERVICE_NOTFOUND since that would confuse
  callers of CallService().
  Rerurns 0 on success or non-zero if 'name' is invalid or name is already used.
*/
int CreateServiceFunction(const QString* name, ELISESERVICE serviceProc);

/* ServiceExists
  Finds if a service with the given 'name' exists.
  Returns non-zero if the service was found, and zero if it was not
*/
int ServiceExists(const QString* name);

/* CallService
  Finds and calls the service function 'name' using the parameters wParam and
  lParam.
  Returns CALLSERVICE_NOTFOUND if no service function called 'name' has been
  created, or the value the service function returned otherwise.
*/
intptr_t CallService(const QString* name, uintptr_t wParam, uintptr_t lParam);

/* DestroyServiceFunction
  Removes the function associated with name from the global service function
  list. Modules calling CallService() will fail if they try to call this
  service's name.
  Returns 0 on success or non-zero if name is invalid.
*/
int DestroyServiceFunction(QString* name);


#endif // MODULES_H
