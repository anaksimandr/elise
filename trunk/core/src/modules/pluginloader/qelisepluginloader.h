#ifndef ELISE_CORE_MODULES_PLUGINLOADER_QELISEPLUGINLOADER_H_
#define ELISE_CORE_MODULES_PLUGINLOADER_QELISEPLUGINLOADER_H_

#include <QPluginLoader>

#include "../../core.h"

class QElisePluginLoader : public QPluginLoader
{
	Q_OBJECT
public:
	explicit QElisePluginLoader(QObject* parent = 0);
	
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
	
};

#endif // ELISE_CORE_MODULES_PLUGINLOADER_QELISEPLUGINLOADER_H_
