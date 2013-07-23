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

#ifndef ELISE_API_E_PLUGINAPI_H_
#define ELISE_API_E_PLUGINAPI_H_

#include <QtCore>
#include "defines.h"

class QObject;

//-- Hook functions
typedef int (*EliseHook)(intptr_t,intptr_t);

//-- Services functions
typedef intptr_t (*EliseService)(intptr_t,intptr_t);	//-- type = 0
//typedef QObject* (*QEliseService)();					//-- type = 1

class ICore
{
public:
	virtual				~ICore() {}

	//-- See services.h for more details
	virtual int			createHookableEvent(const QLatin1String*) = 0;
	virtual int			destroyHookableEvent(const QLatin1String*) = 0;
	virtual int			notifyEventHooks(const QLatin1String*, intptr_t, intptr_t) = 0;
	virtual int			hookEvent(const QLatin1String*, EliseHook) = 0;
	virtual int			unhookEvent(const QLatin1String*, EliseHook) = 0;
	virtual int			createServiceFunction(const QLatin1String*, EliseService) = 0;
	//virtual int			createServiceFunction(const QLatin1String*, QEliseService) = 0;
	virtual int			destroyServiceFunction(const QLatin1String*) = 0;
	virtual intptr_t	callService(const QLatin1String*, intptr_t, intptr_t) = 0;
	virtual int			serviceExists(const QLatin1String*) = 0;
};

class IPlugin
{
public:
	virtual						~IPlugin() {}

	//-- Load plugin
	//-- Must be called after login function
	//-- Return 0 on success, non-zero on failure
	//-- NOTE: if this function return non-zero then Elise loading wiil be aborted
	virtual	int					Load(ICore*) = 0;

	//-- Unload plugin
	//-- Return 0 on success, non-zero on failure
	virtual	int					Unload(void) = 0;
};

Q_DECLARE_INTERFACE(IPlugin, "Elise.basicPluginInterface/1.0")

#endif // ELISE_API_E_PLUGINAPI_H_
