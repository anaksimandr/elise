#ifndef ELISE_CORE_MODULES_TRAY_H_
#define ELISE_CORE_MODULES_TRAY_H_

#include <QtWidgets/QtWidgets>
//#include "../commonheaders.h"

class EliseTray;
extern EliseTray* trayElise;

class EliseTray : public QSystemTrayIcon {
	Q_OBJECT
private:

private slots:
	void			trayActivationNotify(QSystemTrayIcon::ActivationReason reason);
public:
	//QSystemTrayIcon	trayIcon;
					EliseTray();
					~EliseTray();
    int				addToMenu(QAction* action);
	int				delFromMenu(QAction* action);
	//int				setIcon(QIcon* icon);
	//int				addToContextMenu();
};

int LoadTrayModule();
int UnloadTrayModule();

#endif // ELISE_CORE_MODULES_TRAY_H_
