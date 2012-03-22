#ifndef MODULES_H
#define MODULES_H

#include "commonheaders.h"

int LoadDefaultModules();
int UnloadDefaultModules();

typedef struct {
	QString name;
	IDBPlugin* plugin;
} LOADEDDBPLUGIN;

class PluginLoader
{
private:
	static QMap<QString, IPlugin*>*	plugins;
	static LOADEDDBPLUGIN loadedDBPlugin;
	static int			getAvailablePlugins(QMap<QString, IDBPlugin *>* dbPlugins,
									   QMap<QString, IPlugin*>* loadablePlugins);
public:
	static QDir			getPluginsDir();
	static int			callLoginWindow(QMap<QString, IPlugin*>* loadablePlugins, bool first);
	static int			loadDBPlugin(QString pluginName, IDBPlugin* dbPlugin);
	static int			loadPlugins(QMap<QString, IPlugin *>* loadablePlugins);
	static int			unloadPlugins();
};


#endif // MODULES_H
