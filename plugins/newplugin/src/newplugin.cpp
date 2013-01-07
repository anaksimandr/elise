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

#include "../../../api/version.h"
#include "newplugin.h"

const QLatin1String	kTestPlugin_service	=	QLatin1String(__Tests_TestService);
ICore* core;
QSet<QUuid>* NewPlugin::interfaces = 0;

const PluginInfo pluginInfo = {
	"NewPlugin",
	{__MAJOR_VERSION, __MINOR_VERSION, __BUILD_NUM, __SVN_REVISION},
	"Simple plugin",
	"http://code.google.com/p/elise/",
	"somebody",
	"somebody@nomail.com",
	"© 2012 Elise IM project",
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
		interfaces->insert(__UUID_TestPlugin);
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
	core->createServiceFunction(&kTestPlugin_service, &testPluginFunction);
	return 0;
}

int NewPlugin::Unload(void)
{
	core->destroyServiceFunction(&kTestPlugin_service);
	return 0;
}

