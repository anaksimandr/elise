#ifndef ELISEDB_H
#define ELISEDB_H

#include "commonheaders.h"

class EliseDB {
private:
	static QSqlDatabase		sysdb;
public:
	static int				createSysDB();
	static int				createNewAcc();
	static int				selectAcc();
	static int				loadProfile();
};

#endif // ELISEDB_H
