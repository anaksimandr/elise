#ifndef ELISE_API_E_PLUGINAPI_H_
#define ELISE_API_E_PLUGINAPI_H_

#include <QtCore>

/* Replaceable plugin interfaces.

This is unique IDs for standart interfaces. If your plugin implements one of standart
interfaces it must return this interface in IPlugin::ElisePluginInterfaces(void) method.
But non standard interfaces must be returned too.

Note: DBPlugin should not implements any other interfaces.
	const QUuid EUUID_DATABASE = "{4df3e270-fb8b-4654-9271-2f0f31e0eb84}";

*/

//-- Hook functions
typedef int (*EliseHook)(intptr_t,intptr_t);

//-- Services functions
typedef intptr_t (*EliseService)(intptr_t,intptr_t);

typedef struct {
	QString	name;				// [3] - major version,	[2] - minor version
	unsigned char version[4];	// [1] - release num,	[0] - build num
	QString	description;
	QString	homepage;
	QString	author;
	QString	authorEmail;
	QString	copyright;
	QUuid	uuid;
} PluginInfo;

//-- Use this to store information about your hooks and to call UnhookEvent() function
//-- See UnhookEvent() function description in services.h for more details.
typedef struct
{
	QLatin1String* name;
	int num;
} THook;

class ICore
{
public:
	virtual				~ICore() {}

	//-- See services.h for more details
	virtual int			createHookableEvent(const QLatin1String*) = 0;
	virtual int			destroyHookableEvent(const QLatin1String*) = 0;
	virtual int			notifyEventHooks(const QLatin1String*, uintptr_t, uintptr_t) = 0;
	virtual int			hookEvent(const QLatin1String*, EliseHook) = 0;
	virtual int			unhookEvent(const THook) = 0;
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
