
#include "commonheaders.h"

QSqlDatabase EliseDB::sysdb;


int EliseDB::createSysDB()
{
	//-- Create new DB
	//QSqlDatabase db = QSqlDatabase::addDatabase("QSQLCIPHER");
	sysdb = QSqlDatabase::addDatabase("QSQLCIPHER");

	sysdb.setDatabaseName(elisesys);

	if (!sysdb.open()) {
		QMessageBox::critical(0,
							  "Cannot open system database",
							  "Unable to establish a database connection.\n"
								 "SQLCipher not found.",
							  QMessageBox::Cancel);
		return -1;
	}

	QSqlQuery query;

	//-- Set encryption key
	query.exec("pragma key = '" + DBKEY + "';");

	//-- Create table, return if failure
	bool b = query.exec("CREATE TABLE sys_acc ("
					   "name VARCHAR(20) PRIMARY KEY NOT NULL,"
					   "passwd VARCHAR(20),"
					   "savepass BOOLEAN"
					   ")");

	if (!b) {
		QMessageBox::critical(0, "Error",
							  "Unable to create system table.",
							  QMessageBox::Cancel);
	}


	return 0;
}

int EliseDB::createNewAcc()
{


	return 0;
}

int EliseDB::selectAcc()
{
	return 0;
}

int EliseDB::loadProfile()
{
	return 0;
}
