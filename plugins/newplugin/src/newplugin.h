#ifndef NEWPLUGIN_H
#define NEWPLUGIN_H

#include <QtWidgets/QtWidgets>
#include "../../../api/e_pluginapi.h"

class NewPlugin : public QObject, IPlugin
 {
	Q_OBJECT
	Q_INTERFACES(IPlugin)
	Q_PLUGIN_METADATA(IID "elise.IPlugin")

public:
	PluginInfo*			ElisePluginInfo();
	const QUuid*		ElisePluginInterfaces(void);
	int					Load(ICore* coreAPI);
	int					Unload(void);
};


#endif // NEWPLUGIN_H
