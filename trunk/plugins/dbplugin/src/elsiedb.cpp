
#include "commonheaders.h"

//-- Types of settings
extern const unsigned char intType;
extern const unsigned char realType;
extern const unsigned char textType;
extern const unsigned char blobType;

QString qsProfile;

int WriteSettingToBase(uintptr_t  wParam, uintptr_t lParam)
{
	SETTING* set = (SETTING*)lParam;
	QSqlQuery query(QSqlDatabase::database(qsProfile));
	query.prepare("insert into :5(contact, module, setting, value) values(:1, :2, :3, :4)");
	query.bindValue(":1", set->contact);
	query.bindValue(":2", set->qsModule);
	query.bindValue(":3", set->qsSetting);
	switch (set->var->type) {
		case intType:
		{
			query.bindValue(":4", set->var->intValue);
			query.bindValue(":5", "int_settings");
		}
			break;
		case realType:
		{
			query.bindValue(":4", set->var->realValue);
			query.bindValue(":5", "real_settings");
		}
			break;
		case textType:
		{
			query.bindValue(":4", *set->var->textValue);
			query.bindValue(":5", "text_settings");
		}
			break;
		case blobType:
		{
			query.bindValue(":4", *set->var->blobValue);
			query.bindValue(":5", "blob_settings");
		}
			break;
	}
	query.exec();
	query.finish();

	return 0;
}

int ReadSettingFromBase(uintptr_t  wParam, uintptr_t lParam)
{
	return 0;
}
