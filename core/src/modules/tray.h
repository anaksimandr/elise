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
