
#include "../commonheaders.h"

EliseTray* trayElise;

void EliseTray::trayActivationNotify(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
		case QSystemTrayIcon::Trigger:
		{
			NotifyEventHooks(&TRAY_SINGLECLICK, 0, 0);
			break;
		}
		case QSystemTrayIcon::DoubleClick:
		{
			NotifyEventHooks(&TRAY_DOUBLECLICK, 0, 0);
			break;
		}
		case QSystemTrayIcon::MiddleClick:
		{
			NotifyEventHooks(&TRAY_MIDDLECLICK, 0, 0);
			break;
		}
		default:
			break;
	} //switch
	//return 0;
}

int addToContextMenu(uintptr_t wParam,uintptr_t lParam)
{
	QAction* action = (QAction*) wParam;
	trayElise->addToMenu(action);
	return 0;
}

int EliseTray::addToMenu(QAction* action)
{
	contextMenu()->addAction(action);
	return 0;
}

int LoadTrayModule()
{
	trayElise = new EliseTray();
	trayElise->setIcon(QIcon(":/icons/img/main.png"));
	trayElise->show();
	CreateHookableEvent(&TRAY_SINGLECLICK);
	CreateHookableEvent(&TRAY_DOUBLECLICK);
	CreateHookableEvent(&TRAY_MIDDLECLICK);
	QObject::connect(trayElise, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
					 trayElise, SLOT(trayActivationNotify(QSystemTrayIcon::ActivationReason)));
	trayElise->setContextMenu(new QMenu());
	CreateServiceFunction(&TRAY_ADD_MENUITEM, (ELISESERVICE)addToContextMenu);
	return 0;
}

int UnloadTrayModule()
{
	trayElise->hide();
	trayElise->~EliseTray();
	return 0;
}

EliseTray::EliseTray()
{

}

EliseTray::~EliseTray()
{

}
