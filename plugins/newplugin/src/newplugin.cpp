
#include "newplugin.h"

const QLatin1String	TESTPLUGIN_SERVICE	=	QLatin1String("TESTPLUGIN_SERVICE");
ICore* core;
QSet<QUuid>* NewPlugin::interfaces = 0;

const PluginInfo pluginInfo = {
	"NewPlugin",
	{0,0,0,1},
	"simple plugin",
	"author",
	"exa@mple.org",
	"(c) example copyright",
	"http://simple"
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
	core->CreateServiceFunction(&TESTPLUGIN_SERVICE, &testPluginFunction);
	return 0;
}

int NewPlugin::Unload(void)
{
	core->DestroyServiceFunction(&TESTPLUGIN_SERVICE);
	return 0;
}

//Q_EXPORT_PLUGIN2(newplugin, NewPlugin)

