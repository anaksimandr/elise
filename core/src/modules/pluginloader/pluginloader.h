#ifndef ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADER_H_
#define ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADER_H_

#include <QtWidgets/QtWidgets>

class IPlugin;
class IDBPlugin;
class CoreAPI;

/*typedef struct {
	QString name;
	IDBPlugin* plugin;
} LoadedDBPlugin;*/

typedef struct {
	bool		loaded;
	bool		loadable;
	IPlugin*	pluginInterface;
} Plugin;

class PluginLoader
{
private:
	static CoreAPI					coreAPI;
	static QMap<QString, Plugin>*	plugins;
	static QMap<QUuid, QString>*	interfaces;
	//static LoadedDBPlugin			loadedDBPlugin;
	static bool			loadingPluginDisabled(const QString& plugin);
public:
	static QDir			getPluginsDir();
	static int			getAvailablePlugins();
	static int			loadDBPlugin(QString pluginName);
	static int			loadPlugins();
	static int			unloadAllPlugins();
	static int			LoadPluginLoader();
	//static Plugin		getPlugin(const QString& name);
	//static QString		getInterfacePlugin(const QUuid& interfacePlugin);
	//static const QMap<QString, Plugin>*		getPlugins();
	static const QMap<QString, IDBPlugin*>*	getDBPlugins();
};

#endif // ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADER_H_
