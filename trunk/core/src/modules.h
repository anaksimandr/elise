#ifndef MODULES_H
#define MODULES_H

#include "commonheaders.h"

int LoadDefaultModules();
int UnloadDefaultModules();

class IPlugin
{
public:
	virtual						~IPlugin() {}

	virtual	PLUGININFO*			ElisePluginInfo(EVersion) = 0;
	virtual	const QUuid*		ElisePluginInterfaces(void) = 0;
	virtual	int					Load(PLUGINLINK*) = 0;
	virtual	int					Unload(void) = 0;
};

Q_DECLARE_INTERFACE(IPlugin, "Elise.basicPluginInterface/1.0")

class PluginLoader
{
private:

public:
						PluginLoader();
						~PluginLoader();
	static int			getPluginsList();
	static int			loadPlugins();
};


#endif // MODULES_H
