
#include "newplugin.h"

const QLatin1String	TESTPLUGIN_SERVICE	=	QLatin1String("TESTPLUGIN_SERVICE");
ICore* core;
QSet<QUuid>* NewPlugin::interfaces = 0;

const PluginInfo pluginInfo = {
	"NewPlugin",
	{0,0,0,1},
	"Simple plugin",
	"http://code.google.com/p/elise/",
	"somebody",
	"somebody@nomail.com",
	"© me",
	"{792e2ede-6e43-4891-9ab1-a9eb8f88cc09}"
};


const PluginInfo* NewPlugin::ElisePluginInfo()
{
	return &pluginInfo;
}

const QSet<QUuid>* NewPlugin::ElisePluginInterfaces(void)
{
	if (interfaces == 0) {
		interfaces = new QSet<QUuid>();
		interfaces->insert("{ca0ae4d0-ea7c-4743-b34e-1a2c9c61991d}");
	}

	return interfaces;
}

intptr_t testPluginFunction(intptr_t, intptr_t)
{
	QMessageBox qmes;
	qmes.setText("Call test plugin success! uuid is\n");
	qmes.exec();
	return 0;
}

int NewPlugin::Load(ICore* coreAPI)
{
	core = coreAPI;
	core->createServiceFunction(&TESTPLUGIN_SERVICE, &testPluginFunction);
	return 0;
}

int NewPlugin::Unload(void)
{
	core->destroyServiceFunction(&TESTPLUGIN_SERVICE);
	return 0;
}

//Q_EXPORT_PLUGIN2(newplugin, NewPlugin)

