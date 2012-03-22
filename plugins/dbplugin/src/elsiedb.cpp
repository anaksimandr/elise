
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
	//AccountManager* manager =  new AccountManager();
	//return manager->exec();
	//return 0;
}

QMap<QString, ACCOUNT*>* EliseDB::getAccounts()
{
	QMap<QString, ACCOUNT*>* list = new QMap<QString, ACCOUNT*>();
	ACCOUNT* item;
	for (int i = 0; i < 2; i++) {
		item = new ACCOUNT;
		item->savePassword = i;
		item->defaultAccount = i;
		if (item->savePassword)
			item->password = "test";
		else
			item->password = "";
		list->insert("test" + QString::number(i), item);
	}
	return list;
}

int EliseDB::Login(const QString& name, const QString& password,
				   bool savePassword, bool loginDefault)
{
	QMessageBox qmes;
	qmes.setText("You are trying to login now, but something going wrong =(\n"
				 + name
				 + " - "
				 + password
				 + ", "
				 + QString::number(savePassword)
				 + ", "
				 + QString::number(loginDefault));
	qmes.exec();
	return 0;
}

int EliseDB::CreateAccount(const QString& name, const QString& password)
{
	QMessageBox qmes;
	qmes.setText("You are trying to create account now, but something going wrong =(\n"
				 + name
				 + " - "
				 + password);
	qmes.exec();
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
