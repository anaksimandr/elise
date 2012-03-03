#ifndef MODULES_H
#define MODULES_H

#include "commonheaders.h"

int LoadDefaultModules();
int UnloadDefaultModules();

class PluginLoader
{
private:
	//QStringList*			loadablePlugins;
	QMap<QString, IPlugin*>*	loadablePlugins;

public:
						PluginLoader();
						~PluginLoader();
	static int			getPluginsList();
	static int			loadPlugins();
};


#endif // MODULES_H
