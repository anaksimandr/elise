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

#include "events.h"
#include "../../api/e_pluginapi.h"

class QLatin1String;
class QMutex;

//-- Services
extern const QLatin1String	g_kCoreShutdown_service;
extern const QLatin1String	g_kCoreChangeProfile_service;
extern const QLatin1String	g_kCorePreshutdown;
extern const QLatin1String	g_kDBWriteSetting_service;
extern const QLatin1String	g_kDBReadSetting_service;
extern const QLatin1String	g_kDBDellSetting_service;

extern ICore*	g_core;

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

class Core : public QObject, public ICore
{
	Q_OBJECT
public:
	Core(QObject* parent = 0) : QObject(parent) { profileLoaded_ = false; }
	~Core() {}

	static void	initialize();

	//-- Services through QEvents
	static int		changeProfileService(intptr_t, intptr_t);
	static int		shutdownService(intptr_t, intptr_t);

	intptr_t	getAvailableEventsList();
	int			createHookableEvent(const QLatin1String* name);
	int			destroyHookableEvent(const QLatin1String* name);
	int			notifyEventHooks(const QLatin1String* name, intptr_t wParam, intptr_t lParam );
	int			hookEvent(const QLatin1String* name, EliseHook hookProc);
	int			unhookEvent(const QLatin1String* name, EliseHook hookProc);
	intptr_t	getAvailableServicesList();
	int			createServiceFunction(const QLatin1String* name, EliseService serviceProc);
	int			serviceExists(const QLatin1String* name);
	intptr_t	callService(const QLatin1String* name, intptr_t wParam, intptr_t lParam);
	int			destroyServiceFunction(const QLatin1String* name);

private:
	bool					profileLoaded_;

	//-- Critical sections
	QMutex qmutexHooks_;
	QMutex qmutexServices_;

	//-- Arrays of hookable events and services
	QMap <QLatin1String, THookableEvent*> qmapHooks_;
	QMap <QLatin1String, TService*> qmapServices_;

	void	customEvent(QEvent* event);

	int		loadCore();
	int		unloadCore();

	int		loadProfile(); //-- This function unloads already loaded profile if exist.
	void	shutdown(int result = 0);
};

#endif // ELISE_CORE_CORE_H_
