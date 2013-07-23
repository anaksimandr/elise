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

#ifndef ELISE_PLUGINS_DBPLUGIN_DBPLUGIN_H_
#define ELISE_PLUGINS_DBPLUGIN_DBPLUGIN_H_

#include <QtWidgets/QtWidgets>
#include <QtSql>
#include "../../../api/e_dbpluginapi.h"
#include "../../../api/e_pluginapi.h"

class DBPlugin : public QObject, IPlugin, IDBPlugin
{
	Q_OBJECT
	Q_INTERFACES(IDBPlugin)
	Q_INTERFACES(IPlugin)
	Q_PLUGIN_METADATA(IID "elise.IDBPlugin" FILE "../metadata.json")
private:
	//int							openSysDB();
	//QDir						getProfileDir();
	//QDir						getProfileDir(const QString& name);
	int							loadProfile(const QString& name, const QString& passwd);
public:
	int							Load(ICore* coreAPI);
	int							Unload(void);
	int							Login(const QString& name, const QString& password);
	int							CreateProfile(const QString& name, const QString& password);
};


#endif // ELISE_PLUGINS_DBPLUGIN_DBPLUGIN_H_
