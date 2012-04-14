#ifndef E_PLUGINAPI_H
#define E_PLUGINAPI_H

////////////////////////////////////////////////////////////////////////////////////////////////////
//-- See services.h for more details --/////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

//-- Store information about version here
typedef struct {
	union
	{
		unsigned char chars[4];		// [3] - major version, [2] - minor version
		qint32 value;				// [1] - release num, [0] - build num
	};
} EVersion;

#ifndef SERVICES_H__
//-- Use this to store information about your hooks and to call UnhookEvent() function
//-- See UnhookEvent() function description for more details
typedef struct
{
	QString* name;
	int num;
} THook;
#endif

//-- Hook functions
typedef int (*ELISEHOOK)(uintptr_t,uintptr_t);

//-- Services functions
typedef intptr_t (*ELISESERVICE)(uintptr_t,uintptr_t);

typedef struct {
	//int cbSize;
	QString name;
	EVersion version;
	//char *description;
	//char *author;
	//char *authorEmail;
	//char *copyright;
	//char *homepage;
	QUuid uuid;
} PLUGININFO;

typedef struct {
	int (*CreateHookableEvent)(const QString*);
	int (*DestroyHookableEvent)(const QString*);
	int (*NotifyEventHooks)(const QString*, uintptr_t, uintptr_t);
	int (*HookEvent)(const QString*, ELISEHOOK);
	int (*UnhookEvent)(const THook);
	int (*CreateServiceFunction)(const QString*, ELISESERVICE);
	int (*DestroyServiceFunction)(const QString*);
	intptr_t (*CallService)(const QString*, uintptr_t, uintptr_t);
	int (*ServiceExists)(const QString*);
} PLUGINLINK;

#ifndef SERVICES_H__
	//relies on a global variable 'pluginLink' in the plugins
	extern const PLUGINLINK* pluginLink;
	#define CreateHookableEvent(a)                    pluginLink->CreateHookableEvent(a)
	#define DestroyHookableEvent(a)                   pluginLink->DestroyHookableEvent(a)
	#define NotifyEventHooks(a,b,c)                   pluginLink->NotifyEventHooks(a,b,c)
	#define HookEvent(a,b)                            pluginLink->HookEvent(a,b)
	#define UnhookEvent(a)                            pluginLink->UnhookEvent(a)
	#define CreateServiceFunction(a,b)                pluginLink->CreateServiceFunction(a,b)
	#define DestroyServiceFunction(a)                 pluginLink->DestroyServiceFunction(a)
	#define CallService(a,b,c)                        pluginLink->CallService(a,b,c)
	#define ServiceExists(a)                          pluginLink->ServiceExists(a)
#endif

#ifndef E_DBPLUGINAPI_H
class IPlugin
{
public:
	virtual						~IPlugin() {}

	//-- If this function return not NULL then the plugin is valid and can be load
	virtual	PLUGININFO*			ElisePluginInfo(EVersion) = 0;

	//--
	virtual	const QUuid*		ElisePluginInterfaces(void) = 0;

	//-- Load plugin
	//-- Must be called after login function
	//-- Return 0 on success, non-zero on failure
	//-- NOTE: if this function return non-zero then Elise loading wiil be aborted
	virtual	int					Load(const PLUGINLINK*) = 0;

	//-- Unload plugin
	//-- Return 0 on success, non-zero on failure
	virtual	int					Unload(void) = 0;
};

Q_DECLARE_INTERFACE(IPlugin, "Elise.basicPluginInterface/1.0")
#endif // E_DBPLUGINAPI_H

#endif // E_PLUGINAPI_H
