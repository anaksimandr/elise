#ifndef ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADER_H_
#define ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADER_H_

#include <QtWidgets/QtWidgets>
#include "coreapi.h"
#include "../../../../api/e_pluginapi.h"
#include "../../../../api/e_dbpluginapi.h"
#include "coreapi.h"

typedef struct {
	QString name;
	IDBPlugin* plugin;
} LoadedDBPlugin;

class PluginLoader
{
private:
	static CoreAPI		coreAPI;
	static QMap<QString, IPlugin*>*	plugins;
	//static QMap<QUuid, QString>*	interfaces;
	static LoadedDBPlugin	loadedDBPlugin;
public:
	static QDir			getPluginsDir();
	static int			getAvailablePlugins(QMap<QString, IDBPlugin*>* dbPlugins,
											QMap<QString, IPlugin*>* loadablePlugins);
	static int			loadDBPlugin(QString pluginName, IDBPlugin* dbPlugin);
	static int			loadPlugins(QMap<QString, IPlugin*>* loadablePlugins);
	static int			unloadPlugins();
};

#endif // ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADER_H_
