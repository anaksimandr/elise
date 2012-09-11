
#include "commonheaders.h"

const QString	TESTPLUGIN_SERVICE	=	"TESTPLUGIN_SERVICE";
const PLUGINLINK* pluginLink;

PLUGININFO pluginInfo = {
	"NewPlugin",
	{0,0,0,1},
	"{ca0ae4d0-ea7c-4743-b34e-1a2c9c61991d}"
};


PLUGININFO* NewPlugin::ElisePluginInfo(EVersion)
{
	return &pluginInfo;
}

const QUuid* NewPlugin::ElisePluginInterfaces(void)
{
	return &pluginInfo.uuid;
}

int testPluginFunction(intptr_t result, intptr_t lParam)
{
	QMessageBox qmes;
	qmes.setText("Call test plugin success! uuid is\n" + pluginInfo.uuid.toString());
	qmes.exec();
	return 0;
}

int NewPlugin::Load(const PLUGINLINK* link)
{
	pluginLink = link;
	CreateServiceFunction(&TESTPLUGIN_SERVICE, (ELISESERVICE)testPluginFunction);
	return 0;
}

int NewPlugin::Unload(void)
{
	DestroyServiceFunction(&TESTPLUGIN_SERVICE);
	return 0;
}

//Q_EXPORT_PLUGIN2(newplugin, NewPlugin)

