
#include "commonheaders.h"

const QString	TESTDBPLUGIN_SERVICE	=	"TESTDBPLUGIN_SERVICE";
const PLUGINLINK* pluginLink;

PLUGININFO pluginInfo = {
	{0,0,0,1},
	"{ca0ae4d0-ea7c-4743-b34e-1a2c9c61991d}"
};


PLUGININFO* DBPlugin::ElisePluginInfo(EVersion)
{
	return &pluginInfo;
}

const QUuid* DBPlugin::ElisePluginInterfaces(void)
{
	return &pluginInfo.uuid;
}


const QString tttemp = QUuid::createUuid();
int testPluginFunction(intptr_t result, intptr_t lParam)
{
	QMessageBox qmes;
	qmes.setText("Call test plugin success! uuid db is\n" + tttemp);
	qmes.exec();
	return 0;
}

int DBPlugin::Load(const PLUGINLINK* link)
{
	pluginLink = link;
	CreateServiceFunction(&TESTDBPLUGIN_SERVICE, (ELISESERVICE)testPluginFunction);
	return 0;
}

int DBPlugin::Unload(void)
{
	return 0;
}

Q_EXPORT_PLUGIN2(dbplugin, DBPlugin)
