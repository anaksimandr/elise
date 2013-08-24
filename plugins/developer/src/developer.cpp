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
#include "testwindow.h"
#include "developer.h"

ICore* g_core;

int DeveloperPlugin::Load(ICore* coreAPI)
{
	g_core = coreAPI;
	g_core->createServiceFunction(&kClistShow_service, &TestWindow::showCList);
	g_core->createServiceFunction(&kClistHide_service, &TestWindow::hideCList);
	TestWindow::showCList(0, 0);
	return 0;
}

int DeveloperPlugin::Unload(void)
{
	g_core->destroyServiceFunction(&kClistShow_service);
	g_core->destroyServiceFunction(&kClistHide_service);

	TestWindow::hideCList(0, 0);

	return 0;
}

