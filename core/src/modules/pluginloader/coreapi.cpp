#include "coreapi.h"
#include "../../services.h"

int CoreAPI::CreateHookableEvent(const QLatin1String* name)
{
	return core::CreateHookableEvent(name);
}


int CoreAPI::DestroyHookableEvent(const QLatin1String* name)
{
	return core::DestroyHookableEvent(name);
}


int CoreAPI::NotifyEventHooks(const QLatin1String* name, uintptr_t wParam, uintptr_t lParam)
{
	return core::NotifyEventHooks(name, wParam, lParam);
}


int CoreAPI::HookEvent(const QLatin1String* name, EliseHook pfnHook)
{
	return core::HookEvent(name, pfnHook);
}


int CoreAPI::UnhookEvent(const THook hook)
{
	return core::UnhookEvent(hook);
}


int CoreAPI::CreateServiceFunction(const QLatin1String* name, EliseService pfnService)
{
	return core::CreateServiceFunction(name, pfnService);
}


int CoreAPI::DestroyServiceFunction(const QLatin1String* name)
{
	return core::DestroyServiceFunction(name);
}


intptr_t CoreAPI::CallService(const QLatin1String* name, uintptr_t wParam, uintptr_t lParam)
{
	return core::CallService(name, wParam, lParam);
}


int CoreAPI::ServiceExists(const QLatin1String* name)
{
	return core::ServiceExists(name);
}
