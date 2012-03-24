#ifndef ELISEDB_H
#define ELISEDB_H

#include "commonheaders.h"

class EliseDB {
private:
	static QSqlDatabase			sysdb;
	static int						createSysDB();
	static QDir						getProfileDir(const QString& name);
public:
	static QMap<QString, ACCOUNT*>*	getAccounts();
	static int						Login(const QString& name, const QString& password,
										  bool savePassword, bool loginDefault);
	static int						CreateAccount(const QString& name, const QString& password);
};

#endif // ELISEDB_H
