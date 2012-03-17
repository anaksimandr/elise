
#ifndef LOGON_H
#define LOGON_H

#include "../commonheaders.h"

//-- This class provides account managment functional
class AccountManager : public QDialog
{
	Q_OBJECT
private:

private slots:
	void			abort();

public:
	AccountManager(QMap<QString, IDBPlugin*>* dbPlugins);
	~AccountManager();

};

#endif // LOGON_H
