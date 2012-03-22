#ifndef SERVICES_H__
#define SERVICES_H__

#include <QMutex>
#include <QMap>

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
const QString	CHANGEACC_SERVICE	=	"CHANGEACC_SERVICE";/* Calls unload plugins and calls login window. */
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

/* CreateHookableEvent
  Adds an named event to the list. The event will be automatically destroyed on exit, or can be
  removed from the list earlier using
	  DestroyHookableEvent();
  Will fail if the given name has already been used.
  Return 0 on success, -1 if the name is empty and 1 if the name has been already used.
*/
int CreateHookableEvent(const QString* name);

/* DestroyHookableEvent
  Removes the event 'name' from the list of events. All modules hooked to it are automatically
  unhooked.
	  NotifyEventHooks(...);
  will fail if called with this name again.
  Return 0 on success, -1 if the name is empty and 1 if the 'name' not found in events list.
*/
int DestroyHookableEvent(const QString* name);

/* NotifyEventHooks
  Calls every module in turn that has hooked 'name', using the parameters wParam and lParam.
  If one of the hooks returned non-zero to indicate abort, returns that abort value immediately,
  without calling the rest of the hooks in the chain.
  Returns 0 on success, -1 if name is invalid, any non-zero value that indicates abort of
  any called hook.
*/
int NotifyEventHooks(const QString* name, uintptr_t wParam, uintptr_t lParam );

/* HookEvent
  Adds a new hook to the chain 'name', to be called when the hook owner calls
	  NotifyEventHooks(...);
  All hooks will be automatically destroyed when (their parent event is destroyed or) the programme
  ends, but can be unhooked earlier using
	  UnhookEvent(..);
  wParam and lParam in hookProc() are defined by the creator of the event when NotifyEventHooks()
  is called. The return value is 0 to continue processing the other hooks, or non-zero to stop
  immediately. This abort value is returned to the caller of NotifyEventHooks() and
  should not be -1 since that is a special return code for NotifyEventHooks() (see above).
  Returns -2 if name is empty, -1 if name not found in events list. If the hook created
  successfully, returns its personal number that must be used by call UnhookEvent() as hook.num .
*/
int HookEvent(const QString* name, ELISEHOOK hookProc);

/* UnhookEvent
  Removes a hook from its event chain. It will no longer receive any events.
  hook.num in Thook is personal number of destroying hook that was returned by HookEvent().
  hook.name in Thook is name of chain of events from which the remove is.
  Returns 0 on success, -1 if name is empty, 1 if name is not found in the list of events and -2
  if there is no hooks in chain 'name'.
*/
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
  Returns 0 on success, -1 if 'name' is empty and 1 if name has been already used.
*/
int CreateServiceFunction(const QString* name, ELISESERVICE serviceProc);

/* ServiceExists
  Finds if a service with the given 'name' exists.
  Returns non-zero if the service was found, and zero if it was not. Returns -1 if name is empty.
*/
int ServiceExists(const QString* name);

/* CallService
  Finds and calls the service function 'name' using the parameters wParam and
  lParam.
  Returns SERVICE_NOTFOUND if no service function called 'name' has been
  created, or the value the service function returned otherwise.
*/
intptr_t CallService(const QString* name, uintptr_t wParam, uintptr_t lParam);

/* DestroyServiceFunction
  Removes the function associated with name from the global service function
  list. Modules calling CallService() will fail if they try to call this
  service's name.
  Returns 0 on success, -1 if 'name' is empty and 1 if name not found in services list.
*/
int DestroyServiceFunction(const QString* name);


#endif // SERVICES_H__
