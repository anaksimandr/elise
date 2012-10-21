
#include "commonheaders.h"

const PLUGINLINK* pluginLink;

PLUGININFO pluginInfo = {
	QLatin1String("DBPlugin"),
	{0,0,0,1},
	"{ca0ae4d0-ea7c-4743-b34e-1a2c9c61991d}"
};

QDir DBPlugin::getProfileDir()
{
	//-- All profiles must be stored in "Profiles"
	QDir curDir = QDir(qApp->applicationDirPath());
	if (!curDir.exists("Profiles"))
		curDir.mkdir("Profiles");
	curDir.cd("Profiles");
	return curDir;
}

QDir DBPlugin::getProfileDir(const QString& name)
{
	//if (name.isEmpty())
	//	return 0;
	QDir curDir = getProfileDir();
	if (!name.isEmpty()) {
		if (!curDir.exists(name))
			curDir.mkdir(name);
		curDir.cd(name);
	}
	return curDir;
}

int DBPlugin::openSysDB()
{
	//-- Save current path
	QDir qdCurrent = QDir::current();
	QDir::setCurrent(getProfileDir().path());

	QSqlDatabase sysdb;
#ifdef USE_SQLCIPHER
	sysdb = QSqlDatabase::addDatabase("QSQLCIPHER", qsDBSys);
#else
	sysdb = QSqlDatabase::addDatabase("QSQLITE", qsDBSys);
#endif
	sysdb.setDatabaseName(qsDBSys);

	//-- Check sys database
	if (!QDir::current().exists(qsDBSys)) {
		//-- Create new db
		if (!sysdb.open()) {
			QMessageBox::critical(0,
								  "Cannot open system database",
								  "Unable to establish a database connection.\n"
								  + sysdb.lastError().text(),
								  QMessageBox::Cancel);
			//-- Restore current dir
			QDir::setCurrent(qdCurrent.path());
			return 1;
		}

		QSqlQuery query(sysdb);

#ifdef USE_SQLCIPHER
		//-- Set encryption key
		query.exec("pragma key = '" + DBKEY + "';");
#endif

		//-- Create table, return if failure
		bool b = query.exec("CREATE TABLE accounts ("
							"name VARCHAR(20) PRIMARY KEY NOT NULL,"
							"passwd VARCHAR(20),"
							"savepass BOOLEAN,"
							"defacc BOOLEAN"
							")");

		if (!b) {
			QMessageBox::critical(0, "Error",
								  "Unable to create system table.",
								  QMessageBox::Cancel);
			//-- Restore current dir
			QDir::setCurrent(qdCurrent.path());
			return 1;
		}

	}
	else {
		if (!sysdb.open()) {
			QMessageBox::critical(0,
								  "Cannot open system database",
								  "Unable to establish a database connection.\n"
								  + sysdb.lastError().text(),
								  QMessageBox::Cancel);
			//-- Restore current dir
			QDir::setCurrent(qdCurrent.path());
			return 1;
		}
	}
	//-- Restore current dir
	QDir::setCurrent(qdCurrent.path());
	return 0;
}

QMap<QString, ACCOUNT*>* DBPlugin::GetAccounts()
{
	QMap<QString, ACCOUNT*>* list = new QMap<QString, ACCOUNT*>();
	ACCOUNT* item;
	if (!QSqlDatabase::connectionNames().contains(qsDBSys)) {
		//-- If some trubles is
		if (openSysDB()) {
			item = new ACCOUNT;
			item->savePassword = 0;
			list->insert("Internal plugin error", item);
			return list;
		}
	}
	//-- Save current path
	QDir qdCurrent = QDir::current();
	QDir qdDir = getProfileDir();
	QDir::setCurrent(qdDir.path());
	QSqlQuery query("select passwd, savepass, defacc from accounts where name=:1",
					QSqlDatabase::database(qsDBSys));
	//-- Check all directories in Profiles
	foreach (QString dirName, qdDir.entryList(QDir::AllDirs | QDir::NoDot | QDir::NoDotDot)) {
		if (qdDir.cd(dirName)) {
			//-- If found valid DB - add it to list
			if (qdDir.exists(dirName + qsDBPref)) {
				query.bindValue(":1", dirName);
				query.exec();
				query.next();
				item = new ACCOUNT;
				item->savePassword = query.value(1).toBool();
				item->defaultAccount = query.value(2).toBool();
				if (item->savePassword)
					item->password = query.value(0).toString();
				else
					item->password = "";
				list->insert(dirName, item);
			}
			qdDir.cdUp();
		}
	} //foreach
	//-- Now, clean trash records from system DB
	QString buf;
	QSqlQuery query2("delete from accounts where name=:1", QSqlDatabase::database(qsDBSys));
	query.exec("select name from accounts");
	while (query.next()) {
		buf = query.value(0).toString();
		if (!list->contains(buf)) {
			query2.bindValue(":1", buf);
			query2.exec();
			//QMessageBox::critical(0, "Debug", query.lastError().text(), QMessageBox::Ok);
		}
	}
	//-- Restore current dir
	QDir::setCurrent(qdCurrent.path());

	return list;
}

int DBPlugin::Login(const QString& name, const QString& password,
					bool savePassword, bool loginDefault)
{
	//-- Try to load profile (check pass)
	if (!loadProfile(name, password)) {
		//-- Need it to disconnect from system DB at the end
		{
			QSqlQuery query(QSqlDatabase::database(qsDBSys));
			//-- Update fields in system table for valid account
			if (loginDefault) {
				query.prepare("update accounts set defacc='false'");
				query.exec();
			}
			query.prepare("select count(1) from accounts where name=:1");
			query.bindValue(":1", name);
			query.exec();
			query.next();
			if (query.value(0).toInt() > 0)
				query.prepare("update accounts set passwd=:2, savepass=:3, defacc=:4 where name=:1");
			else
				query.prepare("insert into accounts(name, passwd, savepass, defacc) values(:1, :2, :3, :4)");
			query.bindValue(":1", name);
			if (savePassword)
				query.bindValue(":2", password);
			else
				query.bindValue(":2", "");
			query.bindValue(":3", savePassword);
			query.bindValue(":4", loginDefault);
			query.exec();
			query.finish();
		}
		//--Disconnect from system db
		QSqlDatabase::database(qsDBSys).close();
		QSqlDatabase::removeDatabase(qsDBSys);
		return 0;
	}
	else {
		return 1;
	}
}

int DBPlugin::loadProfile(const QString& name, const QString& passwd)
{
	if (!getProfileDir().exists(name))
		return 1;
	//-- Save current path
	QDir qdCurrent = QDir::current();
	QDir::setCurrent(getProfileDir(name).path());
	if (!QDir::current().exists(name + qsDBPref)) {
		//-- Restore current dir
		QDir::setCurrent(qdCurrent.path());
		return 1;
	}

	QSqlDatabase profiledb;
#ifdef USE_SQLCIPHER
	profiledb = QSqlDatabase::addDatabase("QSQLCIPHER", name);
#else
	profiledb = QSqlDatabase::addDatabase("QSQLITE", name);
#endif
	profiledb.setDatabaseName(name + qsDBPref);

	if (!profiledb.open()) {
		QMessageBox::critical(0,
							  "Cannot open profile database",
							  "Unable to establish a database connection.\n"
							  "SQLCipher not found?",
							  QMessageBox::Cancel);
		QSqlDatabase::removeDatabase(name);
		//-- Restore current dir
		QDir::setCurrent(qdCurrent.path());
		return 1;
	}

	bool invalid = false;
	{
		QSqlQuery query(profiledb);

#ifdef USE_SQLCIPHER
		//-- Set encryption key
		query.exec("pragma key = '" + passwd + "';");
#endif

		//-- Check key
		query.exec("select count(1) from account");
		query.exec();
		query.next();
		if (query.value(0).toInt() > 0) {
			query.exec("select keyPas from account");
			query.next();
			if (query.value(0).toString() == passwd)
				qsProfile = name;
			else
				invalid = true;
		}
		else
			invalid = true;
	}
	if (invalid) {
		profiledb.close();
		QSqlDatabase::removeDatabase(name);
	}
	//-- Restore current dir
	QDir::setCurrent(qdCurrent.path());
	return invalid;
}

int DBPlugin::CreateAccount(const QString& name, const QString& password)
{
	if (getProfileDir().exists(name))
		return 1;
	//-- Save current path
	QDir qdCurrent = QDir::current();
	QDir::setCurrent(getProfileDir(name).path());

	QSqlDatabase profiledb;
#ifdef USE_SQLCIPHER
	profiledb = QSqlDatabase::addDatabase("QSQLCIPHER", name);
#else
	profiledb = QSqlDatabase::addDatabase("QSQLITE", name);
#endif
	profiledb.setDatabaseName(name + qsDBPref);

	if (!profiledb.open()) {
		QMessageBox::critical(0,
							  "Cannot open profile database",
							  "Unable to establish a database connection.\n"
							  "SQLCipher not found?",
							  QMessageBox::Cancel);
		QSqlDatabase::removeDatabase(name);
		//-- Restore current dir
		QDir::setCurrent(qdCurrent.path());
		return 1;
	}
	{
		QSqlQuery query(profiledb);

#ifdef USE_SQLCIPHER
		//-- Set encryption key
		query.exec("pragma key = '" + password + "';");
#endif

		//-- Create table, return if failure
		bool b = query.exec("CREATE TABLE account ("
							"keyPas VARCHAR(20) PRIMARY KEY NOT NULL"
							")");

		if (!b) {
			QMessageBox::critical(0, "Error",
								  "Unable to create system table.",
								  QMessageBox::Cancel);
		}
		else {
			//-- Put valid flag in DB
			query.prepare("insert into account(keyPas) values(:1)");
			query.bindValue(":1", password);
			query.exec();

			//-- Create profile tables
			//-- Integer values
			query.exec("create table int_settings ("
					   "contact INTEGER NOT NULL,"
					   "module VARCHAR NOT NULL,"
					   "setting VARCHAR NOT NULL,"
					   "value INTEGER,"
					   "PRIMARY KEY(contact, module, setting)"
					   ")");
			//-- Real values
			query.exec("create table real_settings ("
					   "contact INTEGER NOT NULL,"
					   "module VARCHAR NOT NULL,"
					   "setting VARCHAR NOT NULL,"
					   "value REAL,"
					   "PRIMARY KEY(contact, module, setting)"
					   ")");
			//-- Text values
			query.exec("create table text_settings ("
					   "contact INTEGER NOT NULL,"
					   "module VARCHAR NOT NULL,"
					   "setting VARCHAR NOT NULL,"
					   "value text,"
					   "PRIMARY KEY(contact, module, setting)"
					   ")");
			//-- Blob values
			query.exec("create table blob_settings ("
					   "contact INTEGER NOT NULL,"
					   "module VARCHAR NOT NULL,"
					   "setting VARCHAR NOT NULL,"
					   "value BLOB,"
					   "PRIMARY KEY(contact, module, setting)"
					   ")");


			query.finish();
		}
		//QMessageBox::critical(0, "Debug", query.lastError().text(), QMessageBox::Ok);
	}
	//--Disconnect from new db
	profiledb.close();
	QSqlDatabase::removeDatabase(name);

	//-- Update system table
	QSqlQuery query(QSqlDatabase::database(qsDBSys));
	query.prepare("insert into accounts(name, passwd, savepass, defacc) values(:1, :2, 0, 0)");
	query.bindValue(":1", name);
	query.bindValue(":2", password);

	if (!query.exec())
		QMessageBox::critical(0, "Error CreateAccount()",
							 query.lastError().text() + "\n" + query.lastQuery(), QMessageBox::Ok);

	//-- Restore current dir
	QDir::setCurrent(qdCurrent.path());
	return 0;
}

PLUGININFO* DBPlugin::ElisePluginInfo(EVersion)
{
	return &pluginInfo;
}

const QUuid* DBPlugin::ElisePluginInterfaces(void)
{
	return &pluginInfo.uuid;
}

int DBPlugin::Load(const PLUGINLINK* link)
{
	pluginLink = link;

	CreateServiceFunction(&DB_WRITESETTING, (ELISESERVICE)WriteSettingToBase);
	CreateServiceFunction(&DB_READSETTING, (ELISESERVICE)ReadSettingFromBase);
	CreateServiceFunction(&DB_DELSETTING, (ELISESERVICE)DelteSettingFromBase);

	return 0;
}

int DBPlugin::Unload(void)
{
	//-- Free connection to account DB
	if (QSqlDatabase::contains(qsProfile)) {
		QSqlDatabase::database(qsProfile).close();
		QSqlDatabase::removeDatabase(qsProfile);
	}

	return 0;
}

//Q_EXPORT_PLUGIN2(dbplugin, DBPlugin)
