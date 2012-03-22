
#ifndef LOGON_H
#define LOGON_H

class AccountManager;

#include "../commonheaders.h"

/*/-- Monitoring focus changes and call initInputs(); in AccountManager
class FocusMonitor : public QObject
{
	Q_OBJECT
private:
	AccountManager*	parent;
protected:
	bool			eventFilter(QObject* obj, QEvent* event);
public:
					FocusMonitor(AccountManager *parent);
};*/

//-- This class provides account managment functional
class AccountManager : public QDialog
{
	Q_OBJECT
private:
	QComboBox*		cmbAccount;
	QLineEdit*		lePassword;
	QCheckBox*		cbDefaultAcc;
	QCheckBox*		cbSavePassword;
	QComboBox*		listDBPlugins;
	QMap<QString, IDBPlugin*>* avdbPlugins;
	QMap<QString, ACCOUNT*>* listAccounts;
	QString			defaultAcc;
protected:

private slots:
	void			abort();
	void			ok();
	void			createAccount();
	void			importAccount();
	void			loadAccountsList(const QString& text);
	void			loadAccountDetails(const QString& name);
	void			setCBEnable(int state);
public:
	AccountManager(QMap<QString, IDBPlugin*>* dbPlugins);
	~AccountManager();
	int				loadDefault();
};

#endif // LOGON_H
