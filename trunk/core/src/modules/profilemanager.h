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

#ifndef ELISE_CORE_MODULES_PROFILEMANAGER_H_
#define ELISE_CORE_MODULES_PROFILEMANAGER_H_

#include <QtWidgets/QtWidgets>
//#include "../../../api/e_pluginapi.h"
#include "../../../api/e_dbpluginapi.h"


class ProfileManager : public QDialog
{
	Q_OBJECT
private:
	QComboBox*		cmbProfiles;
	QComboBox*		cmbDBPlugins;
	QLineEdit*		lePassword;
	QCheckBox*		cbSavePassword;
	QCheckBox*		cbDefaultProfile;
	const QMap<QString, IDBPlugin*>* DBPlugins;
	QMap<QString, Profile*>* profiles;
	QString			qsDefaultProfile;
protected:

private slots:
	void			abort();
	void			ok();
	void			createProfile();
	void			importProfile();
	void			loadProfiles(const QString& text);
	void			setCBEnable(int state);
public slots:
	void			loadProfileDetails(const QString& name);
public:
	ProfileManager(const QMap<QString, IDBPlugin*>* availableDBPlugins);
	~ProfileManager();
	int				loadDefaultProfile();
};

#endif // ELISE_CORE_MODULES_PROFILEMANAGER_H_
