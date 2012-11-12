#ifndef ELISE_CORE_MODULES_PLUGINLOADER_COREAPI_H_
#define ELISE_CORE_MODULES_PLUGINLOADER_COREAPI_H_

#include "../../../../api/e_pluginapi.h"

class QLatin1String;

class CoreAPI : public ICore
{
public:
	~CoreAPI() {}

	int			CreateHookableEvent(const QLatin1String*);
	int			DestroyHookableEvent(const QLatin1String*);
	int			NotifyEventHooks(const QLatin1String*, uintptr_t, uintptr_t);
	int			HookEvent(const QLatin1String*, EliseHook);
	int			UnhookEvent(const THook);
	int			CreateServiceFunction(const QLatin1String*, EliseService);
	int			DestroyServiceFunction(const QLatin1String*);
	intptr_t	CallService(const QLatin1String*, uintptr_t, uintptr_t);
	int			ServiceExists(const QLatin1String*);
};

#endif // ELISE_CORE_MODULES_PLUGINLOADER_COREAPI_H_
