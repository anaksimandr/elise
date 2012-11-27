
//#include "commonheaders.h"
#include "dbplugin.h"
#include "elisedb.h"
#include "dbkey.h"
#include "../../../api/e_database.h"

const QLatin1String	DB_WRITESETTING	=	QLatin1String("DB/WriteSetting");
const QLatin1String	DB_READSETTING	=	QLatin1String("DB/ReadSetting");
const QLatin1String	DB_DELSETTING	=	QLatin1String("DB/DeleteSetting");

QSet<QUuid>* DBPlugin::interfaces = 0;

ICore* core;

PluginInfo pluginInfo = {
	QLatin1String("DBPlugin"),
	{0,0,0,1},
	//"{ca0ae4d0-ea7c-4743-b34e-1a2c9c61991d}"
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

QMap<QString, Profile*>* DBPlugin::GetProfiles()
{
	QMap<QString, Profile*>* list = new QMap<QString, Profile*>();
	Profile* item;
	if (!QSqlDatabase::connectionNames().contains(qsDBSys)) {
		//-- If some trubles is
		if (openSysDB()) {
			item = new Profile;
			item->savePassword = 0;
			list->insert("Internal plugin error", item);
			return list;
		}
	}
	//-- Save current path
	QDir qdCurrent = QDir::current();
	QDir qdDir = getProfileDir();
	QDir::setCurrent(qdDir.path());
	QSqlQuery query(QSqlDatabase::database(qsDBSys));
	QString qsQuery =
			QStringLiteral("select passwd, savepass, defacc from accounts where name=:profileName");
	//-- Check all directories in Profiles
	foreach (QString dirName, qdDir.entryList(QDir::AllDirs | QDir::NoDot | QDir::NoDotDot)) {
		if (qdDir.cd(dirName)) {
			//-- If found valid DB - add it to list
			if (qdDir.exists(dirName + qsDBPref)) {
				query.prepare(qsQuery);
				query.bindValue(QStringLiteral(":profileName"), dirName);
				query.exec();
				query.next();
				item = new Profile;
				item->savePassword = query.value(1).toBool();
				item->defaultProfile = query.value(2).toBool();
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
	QSqlQuery query2(QSqlDatabase::database(qsDBSys));
	qsQuery = QStringLiteral("delete from accounts where name=:profileName");
	query.exec(QStringLiteral("select name from accounts"));
	QString buf;
	while (query.next()) {
		buf = query.value(0).toString();
		if (!list->contains(buf)) {
			query2.prepare(qsQuery);
			query2.bindValue(QStringLiteral(":profileName"), buf);
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
		//-- Need this brackets to disconnect from system DB at the end
		{
			QSqlQuery query(QSqlDatabase::database(qsDBSys));
			//-- Update fields in system table for valid account
			if (loginDefault) {
				query.prepare(QStringLiteral("update accounts set defacc='false'"));
				query.exec();
			}
			query.prepare(QStringLiteral("select count(1) from accounts where name=:profileName"));
			query.bindValue(QStringLiteral(":profileName"), name);
			query.exec();
			query.next();
			if (query.value(0).toInt() > 0)
				query.prepare(QStringLiteral("update accounts set passwd=:2, savepass=:3, defacc=:4 where name=:profileName"));
			else
				query.prepare(QStringLiteral("insert into accounts(name, passwd, savepass, defacc) values(:profileName, :2, :3, :4)"));
			query.bindValue(":profileName", name);
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

int DBPlugin::CreateProfile(const QString& name, const QString& password)
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

PluginInfo* DBPlugin::ElisePluginInfo()
{
	return &pluginInfo;
}

const QSet<QUuid>* DBPlugin::ElisePluginInterfaces(void)
{
	if (interfaces == 0) {
		interfaces = new QSet<QUuid>();
		interfaces->insert("{4df3e270-fb8b-4654-9271-2f0f31e0eb84}");
	}

	return interfaces;
}

int DBPlugin::Load(ICore* coreAPI)
{
	core = coreAPI;

	core->CreateServiceFunction(&DB_WRITESETTING, &WriteSettingToBase);
	core->CreateServiceFunction(&DB_READSETTING, &ReadSettingFromBase);
	core->CreateServiceFunction(&DB_DELSETTING, &DelteSettingFromBase);

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
