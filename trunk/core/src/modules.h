#ifndef MODULES_H
#define MODULES_H

#include "commonheaders.h"

int LoadDefaultModules();
int UnloadDefaultModules();

class PluginLoader
{
private:
	//static QMap<QString, IPlugin*>*	loadablePlugins;
	//static QMap<QString, IDBPlugin*>*	dbPlugins;
	static QMap<QString, IPlugin*>*	plugins;
public:
	static int			getPluginsList(QMap<QString, IDBPlugin *>* dbPlugins,
									   QMap<QString, IPlugin *>* loadablePlugins);
	static int			loadDBPlugin();
	static int			loadPlugins(QMap<QString, IPlugin *>* loadablePlugins);
};


#endif // MODULES_H
