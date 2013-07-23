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

#ifndef ELISE_CORE_CORE_H_
#define ELISE_CORE_CORE_H_

#include <stdint.h>
#include "../../api/e_pluginapi.h"

class QLatin1String;
class QMutex;

//-- Services
extern const QLatin1String	kShutdown_service;
extern const QLatin1String	kChangeProfile_service;
extern const QLatin1String	kDBWriteSetting_service;
extern const QLatin1String	kDBReadSetting_service;
extern const QLatin1String	kDBDellSetting_service;
//extern const QLatin1String	kClistShow_service;

typedef struct
{
	QSet<EliseHook>*	qsetSubscribers;
	QMutex*				qmutexHook;
} THookableEvent;

typedef struct
{
	int type;
	union {
		EliseService pfnService;
		//QEliseService pfnQService;
	};
} TService;

extern ICore* core;

class Core : public ICore
{
private:
	static bool	profileLoaded;

	//-- Critical sections
	QMutex qmutexHooks_;
	QMutex qmutexServices_;

	//-- Arrays of hookable events and services
	QMap <QLatin1String, THookableEvent*> qmapHooks_;
	QMap <QLatin1String, TService*> qmapServices_;

	//int	createService(const QLatin1String* name, TService* service);

public:

	Core() {}
	~Core() {}

	static int		loadCore();
	static int		unloadCore();

	static int		launch();
	static int		shutdown(intptr_t result, intptr_t);

	//-- This function unloads already loaded profile if exist.
	static int		loadProfile(intptr_t, intptr_t);

	//-- Hook functions --//////////////////////////////////////////////////////////////////////////

	/* CreateHookableEvent
	 * Adds an named event to the list. The event will be automatically destroyed on exit, or can be
	 * removed from the list earlier using
	 *		DestroyHookableEvent();
	 * Will fail if the given name has already been used.
	 * Return 0 on success, -2 if the name is empty and -1 if the name has been already used.
	 */
	int createHookableEvent(const QLatin1String* name);

	/* DestroyHookableEvent
	 * Removes the event 'name' from the list of events. All modules hooked to it are automatically
	 * unhooked.
	 *	  NotifyEventHooks(...);
	 * will fail if called with this name again.
	 * Return 0 on success, -2 if the name is empty and -1 if the 'name' not found in events list.
	 */
	int destroyHookableEvent(const QLatin1String* name);

	/* NotifyEventHooks
	 * Calls every module in turn that has hooked 'name', using the parameters wParam and lParam
	 * (they are defined by the creator of the event when NotifyEventHooks() is called).
	 * If one of the hooks returned non-zero to indicate abort, returns that abort value immediately,
	 * without calling the rest of the hooks in the chain.
	 * Returns 0 on success, -1 if name is invalid, any non-zero value that indicates break of one
	 * of called hooks.
	 */
	int notifyEventHooks(const QLatin1String* name, intptr_t wParam, intptr_t lParam );

	/* HookEvent
	 * Adds a new hook to the chain 'name', to be called when the hook owner calls
	 *	  NotifyEventHooks(...);
	 * All hooks will be automatically destroyed when their parent event is destroyed or
	 * the programm ends, but they can be unhooked earlier using
	 *	  UnhookEvent(..);
	 * wParam and lParam in hookProc() are defined by the creator of the event when NotifyEventHooks()
	 * is called. The return value of hookProc is 0 to continue processing the other hooks,
	 * or non-zero to stop immediately. This abort value is returned to the caller of
	 * NotifyEventHooks() and should not be -1 since that is a special return code
	 * for NotifyEventHooks() (see above).
	 * Returns 0 on success, -2 if name is empty, -1 if name not found in events list.
	 */
	int hookEvent(const QLatin1String* name, EliseHook hookProc);

	/* UnhookEvent
	 * Removes a hook from its event chain. It will no longer receive any events.
	 * 'name' is the name of chain of events from which the remove is.
	 * 'hookProc' is the pointer to unhooking function.
	 * Returns 0 on success, -2 if name is empty, -1 if name is not found in the list of events, -3
	 * if there is no 'hookProc' in chain 'name'.
	 */
	int unhookEvent(const QLatin1String* name, EliseHook hookProc);

	//-- Service functions --///////////////////////////////////////////////////////////////////////

	/* CreateServiceFunction
	 * Adds a new service function called 'name' to the global list. Service function pointers are
	 * destroyed automatically on exit, but can be removed from the list earlier using
	 *		DestroyServiceFunction()
	 * serviceProc is defined by the caller as
	 *		int ServiceProc(uintptr_t wParam, uintptr_t lParam)
	 * where the creator publishes the meanings of wParam, lParam and the return value.
	 * Service functions must not return SERVICE_NOTFOUND since that would confuse
	 * callers of CallService().
	 * Returns 0 on success, -2 if name is empty and -1 if name has been already used.
	 */
	int createServiceFunction(const QLatin1String* name, EliseService serviceProc);
	//int createServiceFunction(const QLatin1String* name, QEliseService serviceProc);

	/* ServiceExists
	 * Finds if a service with the given 'name' exists.
	 * Returns non-zero if the service was found, and zero if it was not. Returns -2 if name is
	 * empty.
	 */
	int serviceExists(const QLatin1String* name);

	/* CallService
	 * Finds and calls the service function name using the parameters wParam and
	 * lParam.
	 * Returns -1 if there is no service found otherwise returns the value of the service
	 * function.
	 */
	intptr_t callService(const QLatin1String* name, intptr_t wParam, intptr_t lParam);

	/* DestroyServiceFunction
	 * Removes the function associated with name from the global service function
	 * list. Modules calling CallService() will fail if they try to call this
	 * service's name.
	 * Returns 0 on success, -2 if name is empty and -1 if name not found in services list.
	 */
	int destroyServiceFunction(const QLatin1String* name);
};

#endif // ELISE_CORE_CORE_H_
