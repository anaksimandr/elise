#ifndef ELISE_PLUGINS_NEWPLUGIN_NEWPLUGIN_H_
#define ELISE_PLUGINS_NEWPLUGIN_NEWPLUGIN_H_

#include <QtWidgets/QtWidgets>
#include "../../../api/e_pluginapi.h"

class NewPlugin : public QObject, IPlugin
 {
	Q_OBJECT
	Q_INTERFACES(IPlugin)
	Q_PLUGIN_METADATA(IID "elise.IPlugin")
	static QSet<QUuid>*		interfaces;
public:
	PluginInfo*				ElisePluginInfo();
	const QSet<QUuid>*		ElisePluginInterfaces(void);
	int						Load(ICore* coreAPI);
	int						Unload(void);
};


#endif // ELISE_PLUGINS_NEWPLUGIN_NEWPLUGIN_H_
