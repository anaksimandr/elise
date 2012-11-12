
//#include "commonheaders.h"
#include "elisedb.h"
#include "../../../api/e_database.h"

//-- Types of settings
extern const unsigned char intType;
extern const unsigned char realType;
extern const unsigned char textType;
extern const unsigned char blobType;

QString qsProfile;

int WriteSettingToBase(intptr_t  wParam, intptr_t)
{
	Setting* set = reinterpret_cast<Setting*>(wParam);
	//if (set->qsModule->isEmpty() || set->qsSetting->isEmpty()) {
	if ((set->qsModule->size() < 1) || (set->qsSetting->size() < 1)) {
#ifndef NDEBUG
		QMessageBox::critical(0, "Debug WriteSettingToBase",
							  "qsModule or qsSetting is empty\n"
							  + *set->qsModule + " - qsModule\n"
							  + *set->qsSetting + " - qsSetting", QMessageBox::Ok);
#endif
		return 1;
	}
	QSqlQuery query(QSqlDatabase::database(qsProfile));
	QString qsType;
	//-- Choose table
	switch (set->var->type) {
		case intType:
			qsType = "int_settings";
			break;
		case realType:
			qsType = "real_settings";
			break;
		case textType:
			qsType = "text_settings";
			break;
		case blobType:
			qsType = "blob_settings";
			break;
	}
	query.prepare("select count(1) from "
				  + qsType
				  + " where contact=:1 and module=:2 and setting=:3");
	query.bindValue(":1", set->contact);
	query.bindValue(":2", *set->qsModule);
	query.bindValue(":3", *set->qsSetting);
	query.exec();
	query.next();
	//-- Insert or update if already exists
	if (query.value(0).toInt() < 1)
		query.prepare("insert into " + qsType + "(contact, module, setting, value) "
					  "values(:1, :2, :3, :4)");
	else
		query.prepare("update " + qsType + " set value=:4 "
					  "where contact=:1 and module=:2 and setting=:3");
	//-- Bind values
	switch (set->var->type) {
		case intType:
			query.bindValue(":4", set->var->intValue);
			break;
		case realType:
			query.bindValue(":4", set->var->realValue);
			break;
		case textType:
			query.bindValue(":4", *set->var->textValue);
			break;
		case blobType:
			query.bindValue(":4", *set->var->blobValue);
			break;
	}
	query.bindValue(":1", set->contact);
	query.bindValue(":2", *set->qsModule);
	query.bindValue(":3", *set->qsSetting);
#ifdef NDEBUG
	query.exec();
#else
	if (!query.exec())
		QMessageBox::critical(0, "Debug WriteSettingToBase", query.lastError().text() + "\n"
							  + query.executedQuery(), QMessageBox::Ok);
#endif
	query.finish();

	return 0;
}

int ReadSettingFromBase(intptr_t  wParam, intptr_t)
{
	Setting* set = reinterpret_cast<Setting*>(wParam);
	//if (set->qsModule->isEmpty() || set->qsSetting->isEmpty()) {
	if ((set->qsModule->size() < 1) || (set->qsSetting->size() < 1)) {
#ifndef NDEBUG
		QMessageBox::critical(0, "Debug ReadSettingFromBase",
							  "qsModule or qsSetting is empty\n"
							  + *set->qsModule + " - qsModule\n"
							  + *set->qsSetting + " - qsSetting", QMessageBox::Ok);
#endif
		return 1;
	}
	QSqlQuery query(QSqlDatabase::database(qsProfile));
	QString qsType;
	switch (set->var->type) {
		case intType:
			qsType = "int_settings";
			break;
		case realType:
			qsType = "real_settings";
			break;
		case textType:
			qsType = "text_settings";
			break;
		case blobType:
			qsType = "blob_settings";
			break;
	}
	query.prepare("select count(1) from "
				  + qsType
				  + " where contact=:1 and module=:2 and setting=:3");
	query.bindValue(":1", set->contact);
	query.bindValue(":2", *set->qsModule);
	query.bindValue(":3", *set->qsSetting);
	query.exec();
	query.next();
	if (query.value(0).toInt() < 1) {
		return 1;
	}
	query.prepare("select value from "
				  + qsType
				  + " where contact=:1 and module=:2 and setting=:3");
	query.bindValue(":1", set->contact);
	query.bindValue(":2", *set->qsModule);
	query.bindValue(":3", *set->qsSetting);
#ifdef NDEBUG
	query.exec();
#else
	if (!query.exec())
		QMessageBox::critical(0, "Debug ReadSettingFromBase", query.lastError().text() + "\n"
							  + query.executedQuery(), QMessageBox::Ok);
#endif
	query.next();
	switch (set->var->type) {
		case intType:
			set->var->intValue = query.value(0).toInt();
			break;
		case realType:
			set->var->realValue = query.value(0).toDouble();
			break;
		case textType:		
			*set->var->textValue = query.value(0).toString();
			break;
		case blobType:
			*set->var->blobValue = query.value(0).toByteArray();
			break;
	}
	return 0;
}

int DelteSettingFromBase(intptr_t  wParam, intptr_t)
{
	Setting* set = reinterpret_cast<Setting*>(wParam);
	//if (set->qsModule->isEmpty() || set->qsSetting->isEmpty()) {
	if ((set->qsModule->size() < 1) || (set->qsSetting->size() < 1)) {
#ifndef NDEBUG
		QMessageBox::critical(0, "Debug ReadSettingFromBase",
							  "qsModule or qsSetting is empty\n"
							  + *set->qsModule + " - qsModule\n"
							  + *set->qsSetting + " - qsSetting", QMessageBox::Ok);
#endif
		return 1;
	}
	QSqlQuery query(QSqlDatabase::database(qsProfile));
	QString qsType;
	switch (set->var->type) {
		case intType:
			qsType = "int_settings";
			break;
		case realType:
			qsType = "real_settings";
			break;
		case textType:
			qsType = "text_settings";
			break;
		case blobType:
			qsType = "blob_settings";
			break;
	}
	query.prepare("delete from " + qsType + " where contact=:1 and module=:2 and setting=:3");
	query.bindValue(":1", set->contact);
	query.bindValue(":2", *set->qsModule);
	query.bindValue(":3", *set->qsSetting);
	query.exec();
	return 0;
}
