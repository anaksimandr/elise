#ifndef ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADER_H_
#define ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADER_H_

#include <QtWidgets/QtWidgets>
#include "../../../../api/e_pluginapi.h"

extern const QLatin1String	kCoreIsPluginLoaded;			// "Core/IsPluginLoaded"

class IPlugin;
class IDBPlugin;
class CoreAPI;

typedef struct {
	bool		loaded;
	QObject*	instance;
	QUuid*		uuid;
} Plugin;

class PluginLoader
{
private:
	static CoreAPI					coreAPI_;
	static QDir						pluginsDir_;
	static QMap<QString, Plugin>*	plugins_;
	static QMap<QUuid, QString>*	interfaces_;

	static bool			isLoadingPluginDisabled(const QString& pluginModuleName);
public:
	static QDir			getPluginsDir();
	static PluginInfo*	getElisePluginInfo(const QString& pluginModuleName);
	static int			loadPluginLoader();
	static int			loadDBPlugin(const QString& pluginModuleName,
									 const QMap<QString, IDBPlugin*>* dbPlugins);
	static int			loadPlugins();
	static int			loadPlugin(const QString& pluginModuleName);
	static int			unloadPlugin(const QString& pluginModuleName);
	static int			unloadAllPlugins();
	static int			savePluginStateOrDelete(const QString& pluginModuleName,
												bool disableOrDelete);
	static bool			isPluginUnloadable(const QString& pluginModuleName);
	static bool			isPluginLoadable(const QString& pluginModuleName);
	static bool			isPluginLoaded(const QString& pluginModuleName);
	static intptr_t		isPluginLoaded(intptr_t id, intptr_t);
	static const QMap<QString, Plugin>*	getAvailablePlugins();
	static const QMap<QString, IDBPlugin*>*	getDBPlugins();
};

#endif // ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADER_H_
