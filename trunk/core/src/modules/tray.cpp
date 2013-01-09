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

#include "tray.h"
#include "../core.h"


/*
 * TODO: create menu editor based on services, i.e. simple menu should get a list of service
 * that it will call.
 */

const QLatin1String	kTrayAddMenuItem_service	=	QLatin1String(__Tray_AddMenuItem_service);
const QLatin1String	kTraySetIcon_service		=	QLatin1String(__Tray_SetIcon_service);
const QLatin1String	kTraySingleClick_event	=	QLatin1String(__Tray_SingleClick_event);
const QLatin1String	kTrayDoubleClick_event	=	QLatin1String(__Tray_DoubleClick_event);
const QLatin1String	kTrayMiddleClick_event	=	QLatin1String(__Tray_MiddleClick_event);

EliseTray* EliseTray::trayElise = NULL;

void EliseTray::trayActivationNotify(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
		case QSystemTrayIcon::Trigger:
		{
			core->notifyEventHooks(&kTraySingleClick_event, 0, 0);
			break;
		}
		case QSystemTrayIcon::DoubleClick:
		{
			core->notifyEventHooks(&kTrayDoubleClick_event, 0, 0);
			break;
		}
		case QSystemTrayIcon::MiddleClick:
		{
			core->notifyEventHooks(&kTrayMiddleClick_event, 0, 0);
			break;
		}
		default:
			break;
	} //switch
	//return 0;
}

int EliseTray::addToContextMenu(intptr_t wParam,intptr_t)
{
	QAction* action = reinterpret_cast<QAction*>(wParam);
	trayElise->addToMenu(action);
	return 0;
}

int EliseTray::setTrayIcon(intptr_t wParam,intptr_t)
{
	QIcon* icon = (QIcon*)wParam;
	trayElise->setIcon(*icon);
	return 0;
}

int EliseTray::addToMenu(QAction* action)
{
	contextMenu()->addAction(action);
	return 0;
}

int EliseTray::loadTrayModule()
{
	if (!trayElise)
		trayElise = new EliseTray();
	trayElise->setIcon(QIcon(":/icons/img/main.png"));
	trayElise->show();
	core->createHookableEvent(&kTraySingleClick_event);
	core->createHookableEvent(&kTrayDoubleClick_event);
	core->createHookableEvent(&kTrayMiddleClick_event);
	QObject::connect(trayElise, &EliseTray::activated, trayElise, &EliseTray::trayActivationNotify);
	trayElise->setContextMenu(new QMenu());
	core->createServiceFunction(&kTrayAddMenuItem_service, &EliseTray::addToContextMenu);
	core->createServiceFunction(&kTraySetIcon_service, &EliseTray::setTrayIcon);
	return 0;
}

int EliseTray::unloadTrayModule()
{
	trayElise->hide();
	core->destroyHookableEvent(&kTraySingleClick_event);
	core->destroyHookableEvent(&kTrayDoubleClick_event);
	core->destroyHookableEvent(&kTrayMiddleClick_event);
	core->destroyServiceFunction(&kTrayAddMenuItem_service);
	core->destroyServiceFunction(&kTraySetIcon_service);
	delete trayElise;
	trayElise = NULL;
	return 0;
}

EliseTray::EliseTray()
{

}

EliseTray::~EliseTray()
{

}
