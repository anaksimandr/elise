/*  Elise IM, free crossplatform IM client
	Copyright (C) 2012  Elise project

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

