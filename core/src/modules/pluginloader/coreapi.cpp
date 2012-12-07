#include "coreapi.h"
#include "../../services.h"

int CoreAPI::CreateHookableEvent(const QLatin1String* name)
{
	return core::createHookableEvent(name);
}


int CoreAPI::DestroyHookableEvent(const QLatin1String* name)
{
	return core::destroyHookableEvent(name);
}


int CoreAPI::NotifyEventHooks(const QLatin1String* name, uintptr_t wParam, uintptr_t lParam)
{
	return core::notifyEventHooks(name, wParam, lParam);
}


int CoreAPI::HookEvent(const QLatin1String* name, EliseHook pfnHook)
{
	return core::hookEvent(name, pfnHook);
}


int CoreAPI::UnhookEvent(const THook hook)
{
	return core::unhookEvent(hook);
}


int CoreAPI::CreateServiceFunction(const QLatin1String* name, EliseService pfnService)
{
	return core::createServiceFunction(name, pfnService);
}


int CoreAPI::DestroyServiceFunction(const QLatin1String* name)
{
	return core::destroyServiceFunction(name);
}


intptr_t CoreAPI::CallService(const QLatin1String* name, uintptr_t wParam, uintptr_t lParam)
{
	return core::callService(name, wParam, lParam);
}


int CoreAPI::ServiceExists(const QLatin1String* name)
{
	return core::serviceExists(name);
}
