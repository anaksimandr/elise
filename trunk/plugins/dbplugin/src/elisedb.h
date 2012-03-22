#ifndef ELISEDB_H
#define ELISEDB_H

#include "commonheaders.h"

class EliseDB {
private:
	static QSqlDatabase		sysdb;
public:
	static int						createSysDB();
	static int						createNewAcc();
	static int						selectAcc();
	static int						loadProfile();
	static QMap<QString, ACCOUNT*>*	getAccounts();
	static int						Login(const QString& name, const QString& password,
										  bool savePassword, bool loginDefault);
	static int						CreateAccount(const QString& name, const QString& password);
};

#endif // ELISEDB_H
