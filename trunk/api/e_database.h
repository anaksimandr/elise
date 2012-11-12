#ifndef ELISE_API_E_DATABASE_H_
#define ELISE_API_E_DATABASE_H_

class QLatin1String;
class QString;
class QByteArray;

//-- Services
const QLatin1String	DB_WRITESETTING	=	QLatin1String("DB/WriteSetting");	/* Write to DB. */
const QLatin1String	DB_READSETTING	=	QLatin1String("DB/ReadSetting");	/* Read from DB. */
const QLatin1String	DB_DELSETTING	=	QLatin1String("DB/DelteSetting");	/* Delete from DB.*/

//-- Types of settings
const unsigned char intType		=	0;	//-- int
const unsigned char realType	=	1;	//-- double
const unsigned char textType	=	2;	//-- QString
const unsigned char blobType	=	3;	//-- QByteArray

typedef struct {
	unsigned char type;
	union {
		int intValue;
		double realValue;
		QString* textValue;
		QByteArray* blobValue;
	};
} DBVariant;

//-- Used in calls of functions WriteSettingToBase() and ReadSettingFromBase()
//-- And at some other places
//-- Note: you must delete SETTING and its DBVARIANT when it becomes not necessary
typedef struct {
	int				contact;
	QLatin1String*	qsModule;
	QLatin1String*	qsSetting;
	DBVariant*		var;
} Setting;


#endif // ELISE_API_E_DATABASE_H_
