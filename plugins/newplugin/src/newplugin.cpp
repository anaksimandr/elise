
#include "commonheaders.h"


PLUGININFO pluginInfo = {
	{1},
	"{ca0ae4d0-ea7c-4743-b34e-1a2c9c61991d}"
};


PLUGININFO* NewPlugin::ElisePluginInfo(EVersion)
{
	return &pluginInfo;
}

const QUuid* NewPlugin::ElisePluginInterfaces(void)
{
	return &QUuid::createUuid();
}

int NewPlugin::Load(PLUGINLINK*)
{
	return 0;
}

int NewPlugin::Unload(void)
{
	return 0;
}

Q_EXPORT_PLUGIN2(newplugin, NewPlugin)
