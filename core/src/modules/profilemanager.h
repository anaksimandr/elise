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

typedef struct {
	QString	password;
	QString	dbPluginName;
	bool	savePassword;
	bool	defaultProfile;
} Profile;

class ProfileManager : public QDialog
{
	Q_OBJECT
private:
	QComboBox*		cmbProfiles_;
	QComboBox*		cmbDBPlugins_;
	QLineEdit*		lePassword_;
	QCheckBox*		cbSavePassword_;
	QCheckBox*		cbDefaultProfile_;

	//const QSet<QString>*				DBPlugins_;
	QMap<QString, Profile>*				profiles_;
	QString								qsDefaultProfile_;
protected:

private:
	void			abort();
	void			ok();
	void			createProfile();
	void			importProfile();
	void			loadProfiles();
	void			setCBEnable(int state);
	void			makeDefault(const QString& profile);
	QString			readStrFromFile(QDataStream& in);
	bool			readBoolFromFile(QDataStream& in);
	void			writeStrToFile(QDataStream& out, const QString& text);
	void			writeBoolToFile(QDataStream& out, bool val);
public slots:
	void			loadProfileDetails(const QString& name);
public:
	ProfileManager();
	~ProfileManager();

	int				loadDefaultProfile();
	QDir			getProfileDir();
};

#endif // ELISE_CORE_MODULES_PROFILEMANAGER_H_
