/*  Elise IM, free crossplatform IM client
	Copyright (C) 2012  Elise project

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "dbplugin.h"
#include "elisedb.h"
//#include "dbkey.h"
#include "../../../api/e_database.h"
#include "../../../api/version.h"

//Q_PLUGIN_METADATA(IID "Elise.basicDBPluginInterface/1.0" FILE "../metadata.json")

const QLatin1String	kDBWriteSetting_service	=	QLatin1String(__DB_WriteSetting_service);
const QLatin1String	kDBReadSetting_service	=	QLatin1String(__DB_ReadSetting_service);
const QLatin1String	kDBDellSetting_service	=	QLatin1String(__DB_DellSetting_service);

const QLatin1String	kCoreGetProfilesDir	=	QLatin1String(__Core_GetProfilesDir_service);

//-- DB name constants for internal use
const QString qsDBPref = ".sqlite";

ICore* core;

int DBPlugin::Login(const QString& name, const QString& password)
{
	//-- Try to load profile: check DB, pass.
	QDir* profileDir = reinterpret_cast<QDir*>(core->callService(&kCoreGetProfilesDir,
																 reinterpret_cast<intptr_t>(&name),
																 0));

	if (!profileDir->exists(name + qsDBPref)) {
		profileDir->cdUp();
		profileDir->rmdir(name);
		QMessageBox::critical(0,
							  "Cannot open profile database",
							  "Database doesn't exists.",
							  QMessageBox::Cancel);
		delete profileDir;
		return 1;
	}

	QSqlDatabase profiledb = QSqlDatabase::addDatabase("QSQLITE", name);
	profiledb.setDatabaseName(name + qsDBPref);
	if (!profiledb.open()) {
		QMessageBox::critical(0,
							  "Cannot open profile database",
							  "Unable to establish a database connection.",
							  QMessageBox::Cancel);
		QSqlDatabase::removeDatabase(name);
		delete profileDir;
		return 1;
	}

	//-- Check key
	bool invalid = false;
	{
		QSqlQuery query(profiledb);

		//-- Check key
		query.exec("select count(1) from account");
		query.exec();
		query.next();
		if (query.value(0).toInt() > 0) {
			query.exec("select keyPas from account");
			query.next();
			if (query.value(0).toString() == password)
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
		delete profileDir;
		return 1;
	}

	qsProfile = name;

	delete profileDir;
	return 0;
}

int DBPlugin::CreateProfile(const QString& name, const QString& password)
{
	//-- Get/Create profile dir
	QDir* profileDir = reinterpret_cast<QDir*>(core->callService(&kCoreGetProfilesDir,
																 reinterpret_cast<intptr_t>(&name),
																 0));

	if (profileDir->exists(name + qsDBPref)) {
		profileDir->cdUp();
		profileDir->rmdir(name);
		QMessageBox::critical(0,
							  "Cannot create profile database",
							  "Database already exists.",
							  QMessageBox::Cancel);
		delete profileDir;
		return 1;
	}

	QSqlDatabase profiledb = QSqlDatabase::addDatabase("QSQLITE", name);
	profiledb.setDatabaseName(name + qsDBPref);

	if (!profiledb.open()) {
		QMessageBox::critical(0,
							  "Cannot open profile database",
							  "Unable to establish a database connection.",
							  QMessageBox::Cancel);
		QSqlDatabase::removeDatabase(name);
		delete profileDir;
		return 1;
	}

	QSqlQuery query(profiledb);

	//-- Create table, return if failure
	bool b = query.exec("CREATE TABLE account ("
						"keyPas VARCHAR(20) PRIMARY KEY NOT NULL"
						")");

	if (!b) {
		QMessageBox::critical(0, "Error",
							  "Unable to create system table.",
							  QMessageBox::Cancel);
		delete profileDir;
		return 1;
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

	delete profileDir;
	return 0;
}

int DBPlugin::Load(ICore* coreAPI)
{
	core = coreAPI;

	core->createServiceFunction(&kDBWriteSetting_service, &WriteSettingToBase);
	core->createServiceFunction(&kDBReadSetting_service, &ReadSettingFromBase);
	core->createServiceFunction(&kDBDellSetting_service, &DelteSettingFromBase);

	return 0;
}

int DBPlugin::Unload(void)
{
	//-- Free connection to account DB
	if (QSqlDatabase::contains(qsProfile)) {
		QSqlDatabase::database(qsProfile).close();
		QSqlDatabase::removeDatabase(qsProfile);
	}

	core->destroyServiceFunction(&kDBWriteSetting_service);
	core->destroyServiceFunction(&kDBReadSetting_service);
	core->destroyServiceFunction(&kDBDellSetting_service);

	return 0;
}
