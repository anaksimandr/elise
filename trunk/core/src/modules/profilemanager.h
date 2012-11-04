#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include "../commonheaders.h"

class ProfileManager : public QDialog
{
	Q_OBJECT
private:
	QComboBox*		cmbProfiles;
	QComboBox*		cmbDBPlugins;
	QLineEdit*		lePassword;
	QCheckBox*		cbSavePassword;
	QCheckBox*		cbDefaultProfile;
	QMap<QString, IDBPlugin*>* DBPlugins;
	QMap<QString, PROFILE*>* profiles;
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
	ProfileManager(QMap<QString, IDBPlugin*>* availableDBPlugins);
	~ProfileManager();
	int				loadDefaultProfile();
};

#endif // PROFILEMANAGER_H
