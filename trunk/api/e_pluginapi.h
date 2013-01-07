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

/* Replaceable plugin interfaces.

This is unique IDs for standart interfaces. If your plugin implements one of standart
interfaces it must return this interface in IPlugin::ElisePluginInterfaces(void) method.
But non standard interfaces must be returned too.

Note: DBPlugin should not implements any other interfaces.
	const QUuid UUID_DATABASE	=	"{4df3e270-fb8b-4654-9271-2f0f31e0eb84}";
	const QUuid UUID_CLIST		=	"{88441947-8188-474e-ae18-bb2f3795d4fe}";
	const QUuid UUID_CHATWINDOW	=	"{29fbc48a-bf3e-4fdd-a537-35ad427da091}";
	const QUuid UUID_CHATLOG	=	"{d6f1dce9-2c88-495d-b89c-9fb147f4863d}";
	const QUuid UUID_TESTPLUGIN	=	"{a3c43dc8-4bb0-454d-baca-7682f924115c}";
*/

//-- Hook functions
typedef int (*EliseHook)(intptr_t,intptr_t);

//-- Services functions
typedef intptr_t (*EliseService)(intptr_t,intptr_t);

typedef struct {
	QString	name;				// [3] - major version,	[2] - minor version
	unsigned char version[4];	// [1] - build num,	[0] - svn revision
	QString	description;
	QString	homepage;
	QString	author;
	QString	authorEmail;
	QString	copyright;
	QUuid	uuid;
} PluginInfo;

class ICore
{
public:
	virtual				~ICore() {}

	//-- See services.h for more details
	virtual int			createHookableEvent(const QLatin1String*) = 0;
	virtual int			destroyHookableEvent(const QLatin1String*) = 0;
	virtual int			notifyEventHooks(const QLatin1String*, uintptr_t, uintptr_t) = 0;
	virtual int			hookEvent(const QLatin1String*, EliseHook) = 0;
	virtual int			unhookEvent(const QLatin1String*, EliseHook) = 0;
	virtual int			createServiceFunction(const QLatin1String*, EliseService) = 0;
	virtual int			destroyServiceFunction(const QLatin1String*) = 0;
	virtual intptr_t	callService(const QLatin1String*, uintptr_t, uintptr_t) = 0;
	virtual int			serviceExists(const QLatin1String*) = 0;
};

class IPlugin
{
public:
	virtual						~IPlugin() {}

	//-- If this function return not NULL then the plugin is valid and can be load
	virtual	const PluginInfo*	ElisePluginInfo() = 0;

	//--
	virtual	const QSet<QUuid>*	ElisePluginInterfaces(void) = 0;

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
