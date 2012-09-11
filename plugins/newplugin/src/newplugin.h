#ifndef NEWPLUGIN_H
#define NEWPLUGIN_H

#include "commonheaders.h"

class NewPlugin : public QObject, IPlugin
 {
	Q_OBJECT
	Q_INTERFACES(IPlugin)
	//Q_PLUGIN_METADATA(IID "elise.IPlugin" FILE "../plugin.json")
	Q_PLUGIN_METADATA(IID "elise.IPlugin")

public:
	PLUGININFO*			ElisePluginInfo(EVersion);
	const QUuid*		ElisePluginInterfaces(void);
	int					Load(const PLUGINLINK* link);
	int					Unload(void);
};


#endif // NEWPLUGIN_H
