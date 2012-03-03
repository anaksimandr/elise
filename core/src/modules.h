#ifndef MODULES_H
#define MODULES_H

#include "commonheaders.h"

int LoadDefaultModules();
int UnloadDefaultModules();

class PluginLoader
{
private:
	static QMap<QString, IPlugin*>*	loadablePlugins;

public:
	static int			getPluginsList();
	static int			loadPlugins();
};


#endif // MODULES_H
