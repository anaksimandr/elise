#ifndef PLUGINAPI_H
#define PLUGINAPI_H

//#include <QString>
//#include <QUuid>

//-- Store information about version here
typedef struct {
	union
	{
		int value;               // [3] - major version, [2] - minor version
		unsigned char chars[4];  // [1] - release num, [0] - build num
	};
} EVersion;

#ifndef SERVICES_H__
//-- Use this to store information about your hooks and to call UnhookEvent() function
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
	//char *shortName;
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
	extern PLUGINLINK* pluginLink;
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

#endif // PLUGINAPI_H
