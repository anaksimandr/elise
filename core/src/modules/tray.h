#ifndef ELISE_CORE_MODULES_TRAY_H_
#define ELISE_CORE_MODULES_TRAY_H_

#include <QtWidgets/QtWidgets>

//-- Services
extern const QLatin1String	TRAY_ADD_MENUITEM;	//"Tray/AddMenuItem" /* Adds item to tray context menu. */
extern const QLatin1String	TRAY_SET_ICON;		//"Tray/SetIcon"

//-- Hookable events
extern const QLatin1String	TRAY_SINGLECLICK;	//"Tray/SingleClick"
extern const QLatin1String	TRAY_DOUBLECLICK;	//"Tray/DoubleClick"
extern const QLatin1String	TRAY_MIDDLECLICK;	//"Tray/MiddleClick"

class EliseTray;
extern EliseTray* trayElise;

class EliseTray : public QSystemTrayIcon {
	Q_OBJECT
public:
	//QSystemTrayIcon	trayIcon;
	EliseTray();
	~EliseTray();

	int		addToMenu(QAction* action);
	int		delFromMenu(QAction* action);
	void	trayActivationNotify(QSystemTrayIcon::ActivationReason reason);
	//int	setIcon(QIcon* icon);
	//int	addToContextMenu();
};

int LoadTrayModule();
int UnloadTrayModule();

#endif // ELISE_CORE_MODULES_TRAY_H_
