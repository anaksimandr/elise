#ifndef ELISE_API_E_PLUGINAPI_H_
#define ELISE_API_E_PLUGINAPI_H_

#include <QtCore>

/* Replaceable plugin interfaces.

This is unique IDs for standart interfaces. If your plugin implements one of standart
interfaces it must return this interface in IPlugin::ElisePluginInterfaces(void) method.
But non standard interfaces must be returned too.

Note: DBPlugin should not implements any other interfaces.
	QUuid EUUID_DATABASE = "{4df3e270-fb8b-4654-9271-2f0f31e0eb84}";

*/

/*/-- Store information about version here
typedef struct {
	union
	{
		unsigned char chars[4];		// [3] - major version, [2] - minor version
		qint32 intValue;				// [1] - release num, [0] - build num
	};
} EVersion;*/

//-- Hook functions
typedef int (*EliseHook)(intptr_t,intptr_t);

//-- Services functions
typedef intptr_t (*EliseService)(intptr_t,intptr_t);

typedef struct {
	QString name;
	unsigned char version[4];
	QString description;
	QString author;
	QString authorEmail;
	QString copyright;
	QString homepage;
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
	virtual int			CreateHookableEvent(const QLatin1String*) = 0;
	virtual int			DestroyHookableEvent(const QLatin1String*) = 0;
	virtual int			NotifyEventHooks(const QLatin1String*, uintptr_t, uintptr_t) = 0;
	virtual int			HookEvent(const QLatin1String*, EliseHook) = 0;
	virtual int			UnhookEvent(const THook) = 0;
	virtual int			CreateServiceFunction(const QLatin1String*, EliseService) = 0;
	virtual int			DestroyServiceFunction(const QLatin1String*) = 0;
	virtual intptr_t	CallService(const QLatin1String*, uintptr_t, uintptr_t) = 0;
	virtual int			ServiceExists(const QLatin1String*) = 0;
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
