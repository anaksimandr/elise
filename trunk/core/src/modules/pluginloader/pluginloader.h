#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include "../../commonheaders.h"

typedef struct {
	QString name;
	IDBPlugin* plugin;
} LOADEDDBPLUGIN;

class PluginLoader
{
private:
	static QMap<QString, IPlugin*>*	plugins;
	//static QMap<QUuid, QString>*	interfaces;
	static LOADEDDBPLUGIN	loadedDBPlugin;
public:
	static QDir			getPluginsDir();
	static int			getAvailablePlugins(QMap<QString, IDBPlugin*>* dbPlugins,
											QMap<QString, IPlugin*>* loadablePlugins);
	static int			loadDBPlugin(QString pluginName, IDBPlugin* dbPlugin);
	static int			loadPlugins(QMap<QString, IPlugin*>* loadablePlugins);
	static int			unloadPlugins();
};

#endif // PLUGINLOADER_H
