#ifndef ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADER_H_
#define ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADER_H_

#include <QtWidgets/QtWidgets>

class IPlugin;
class IDBPlugin;
class CoreAPI;

typedef struct {
	QString name;
	IDBPlugin* plugin;
} LoadedDBPlugin;

typedef struct {
	bool		loaded;
	bool		loadable;
	IPlugin*	pluginInterface;
} Plugin;

class PluginLoader
{
private:
	static CoreAPI		coreAPI;
	static QMap<QString, Plugin>*	plugins;
	//static QMap<QUuid, QString>*	interfaces;
	static LoadedDBPlugin	loadedDBPlugin;
public:
	static QDir			getPluginsDir();
	static int			getAvailablePlugins(QMap<QString, IDBPlugin*>* dbPlugins,
											QMap<QString, IPlugin*>* loadablePlugins);
	static int			loadDBPlugin(QString pluginName, IDBPlugin* dbPlugin);
	static int			loadPlugins(QMap<QString, IPlugin*>* loadablePlugins);
	static int			unloadPlugins();
	static int			LoadPluginLoader();
};

#endif // ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADER_H_
