#ifndef ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADER_H_
#define ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADER_H_

#include <QtWidgets/QtWidgets>
#include "../../../../api/e_pluginapi.h"

class IPlugin;
class IDBPlugin;
class CoreAPI;

typedef struct {
	bool		loaded;
	QObject*	instance;
} Plugin;

class PluginLoader
{
private:
	static CoreAPI					coreAPI;
	static QDir						pluginsDir;
	static QMap<QString, Plugin>*	plugins;
	static QMap<QUuid, QString>*	interfaces;
	//static LoadedDBPlugin			loadedDBPlugin;
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
	static bool			isPluginLoaded(const QString& pluginModuleName);
	static bool			isPluginLoadable(const QString& pluginModuleName);
	static const QMap<QString, Plugin>*	getAvailablePlugins();
	static const QMap<QString, IDBPlugin*>*	getDBPlugins();
};

#endif // ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADER_H_
