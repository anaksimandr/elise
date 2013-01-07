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
#include "tests.h"

const QLatin1String	kTestPlugin_service	=	QLatin1String(__Tests_TestService);
ICore* core;
QSet<QUuid>* TestPlugin::interfaces = 0;

const PluginInfo pluginInfo = {
	"TestPlugin",
	{__MAJOR_VERSION, __MINOR_VERSION, __BUILD_NUM, __SVN_REVISION},
	"Plugin for testing (for developers only).",
	"http://code.google.com/p/elise/",
	"Sergey Andreenko",
	"s.andreenko@gmail.com",
	"© 2012 Elise IM project",
	"{c8aa9219-6544-481a-8790-d0cd42831647}"
};


const PluginInfo* TestPlugin::ElisePluginInfo()
{
	return &pluginInfo;
}

const QSet<QUuid>* TestPlugin::ElisePluginInterfaces(void)
{
	if (interfaces == 0) {
		interfaces = new QSet<QUuid>();
		interfaces->insert(__UUID_Clist);
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

int TestPlugin::Load(ICore* coreAPI)
{
	core = coreAPI;
	if (core->createServiceFunction(&kTestPlugin_service, &testPluginFunction) == -1)
		QMessageBox::critical(0, "Error", "Test service already exists", QMessageBox::Ok);
	return 0;
}

int TestPlugin::Unload(void)
{
	core->destroyServiceFunction(&kTestPlugin_service);
	return 0;
}

//Q_EXPORT_PLUGIN2(newplugin, NewPlugin)

