#ifndef ELISE_CORE_SERVICES_H_
#define ELISE_CORE_SERVICES_H_

#include "../../api/e_pluginapi.h"

class QMutex;
class QLatin1String;

////////////////////////////////////////////////////////////////////////////////////////////////////
//-- Names of system services and events --/////////////////////////////////////////////////////////

//-- Services
extern const QLatin1String	SHUTDOWN_SERVICE;		//"System/Shutdown"
extern const QLatin1String	CHANGEPROFILE_SERVICE;	//"System/ChangeAcc"
extern const QLatin1String	DB_WRITESETTING;		//"DB/WriteSetting"
extern const QLatin1String	DB_READSETTING;			//"DB/ReadSetting"
extern const QLatin1String	DB_DELSETTING;			//"DB/DeleteSetting"

//-- Hookable events
//const QLatin1String	TRAY_SINGLECLICK	=	QLatin1String("Tray/SingleClick");

namespace core
{

//-- Hooks
typedef struct
{
	//void* hOwner;
	int num;
	int type;
	union {
		struct {
			union {
				EliseHook pfnHook;
				//ELISEHOOKPARAM pfnHookParam;
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

//-- Services
typedef struct
{
	//DWORD nameHash;
	//HINSTANCE hOwner;
	int type;
	union {
		EliseService pfnService;
		//ELISESERVICEWIDG pfnServiceWidg;
	};
	//int flags;
	//LPARAM lParam;
	//void* object;
	//char name[1];
} TService;

////////////////////////////////////////////////////////////////////////////////////////////////////
//-- Hook functions --//////////////////////////////////////////////////////////////////////////////

/* CreateHookableEvent /////////////////////////////////////////////////////////////////////////////
  Adds an named event to the list. The event will be automatically destroyed on exit, or can be
  removed from the list earlier using
	  DestroyHookableEvent();
  Will fail if the given name has already been used.
  Return 0 on success, -2 if the name is empty and -1 if the name has been already used.
*///////////////////////////////////////////////////////////////////////////////////////////////////
int CreateHookableEvent(const QLatin1String* name);

/* DestroyHookableEvent ////////////////////////////////////////////////////////////////////////////
  Removes the event 'name' from the list of events. All modules hooked to it are automatically
  unhooked.
	  NotifyEventHooks(...);
  will fail if called with this name again.
  Return 0 on success, -2 if the name is empty and -1 if the 'name' not found in events list.
*///////////////////////////////////////////////////////////////////////////////////////////////////
int DestroyHookableEvent(const QLatin1String* name);

/* NotifyEventHooks ////////////////////////////////////////////////////////////////////////////////
  Calls every module in turn that has hooked 'name', using the parameters wParam and lParam.
  If one of the hooks returned non-zero to indicate abort, returns that abort value immediately,
  without calling the rest of the hooks in the chain.
  Returns 0 on success, -2 if name is invalid, any non-zero value that indicates break of one
  of called hooks.
*///////////////////////////////////////////////////////////////////////////////////////////////////
int NotifyEventHooks(const QLatin1String* name, uintptr_t wParam, uintptr_t lParam );

/* HookEvent ///////////////////////////////////////////////////////////////////////////////////////
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
  successfully, returns its personal number that must be used by call UnhookEvent().
*///////////////////////////////////////////////////////////////////////////////////////////////////
int HookEvent(const QLatin1String* name, EliseHook hookProc);

/* UnhookEvent /////////////////////////////////////////////////////////////////////////////////////
  Removes a hook from its event chain. It will no longer receive any events.
  hook.num in THook is personal number of destroying hook that was returned by HookEvent().
  hook.name in THook is name of chain of events from which the remove is.
  Returns 0 on success, -2 if name is empty, -1 if name is not found in the list of events and -3
  if there is no hooks in chain 'name' or hook.num is wrong.
*///////////////////////////////////////////////////////////////////////////////////////////////////
int UnhookEvent(const THook hook);

////////////////////////////////////////////////////////////////////////////////////////////////////
//-- Service functions --///////////////////////////////////////////////////////////////////////////

/* CreateServiceFunction ///////////////////////////////////////////////////////////////////////////
  Adds a new service function called 'name' to the global list. Service function pointers are
  destroyed automatically on exit, but can be removed from the list earlier using
	  DestroyServiceFunction()
  serviceProc is defined by the caller as
	  int ServiceProc(uintptr_t wParam, uintptr_t lParam)
  where the creator publishes the meanings of wParam, lParam and the return value.
  Service functions must not return SERVICE_NOTFOUND since that would confuse
  callers of CallService().
  Returns 0 on success, -2 if name is empty and -1 if name has been already used.
*///////////////////////////////////////////////////////////////////////////////////////////////////
int CreateServiceFunction(const QLatin1String* name, EliseService serviceProc);

/* ServiceExists ///////////////////////////////////////////////////////////////////////////////////
  Finds if a service with the given 'name' exists.
  Returns non-zero if the service was found, and zero if it was not. Returns -2 if name is empty.
*///////////////////////////////////////////////////////////////////////////////////////////////////
int ServiceExists(const QLatin1String* name);

/* CallService /////////////////////////////////////////////////////////////////////////////////////
  Finds and calls the service function name using the parameters wParam and
  lParam.
  Returns -1 if there is no service found otherwise returns the value of the service function.
*///////////////////////////////////////////////////////////////////////////////////////////////////
intptr_t CallService(const QLatin1String* name, uintptr_t wParam, uintptr_t lParam);

/* DestroyServiceFunction //////////////////////////////////////////////////////////////////////////
  Removes the function associated with name from the global service function
  list. Modules calling CallService() will fail if they try to call this
  service's name.
  Returns 0 on success, -2 if name is empty and -1 if name not found in services list.
*/ /////////////////////////////////////////////////////////////////////////////////////////////////
int DestroyServiceFunction(const QLatin1String* name);

} //namespace core

#endif // ELISE_CORE_SERVICES_H_
