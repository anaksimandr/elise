#ifndef E_DATABASE_H
#define E_DATABASE_H

//-- Services
const QString	DB_WRITESETTING	=	"DB/WriteSetting";	/* Call this method to write setting to DB. */
const QString	DB_READSETTING	=	"DB/ReadSetting";	/* ... to read setting from DB. */
const QString	DB_DELSETTING	=	"DB/DelteSetting";	/* ... to delete setting from DB.*/

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
} DBVARIANT;

//-- Used in calls of functions WriteSettingToBase() and ReadSettingFromBase()
//-- And at some other places
//-- Note: you must delete SETTING and its DBVARIANT when it becomes not necessary
typedef struct {
	int			contact;
	QString*	qsModule;
	QString*	qsSetting;
	DBVARIANT*	var;
} SETTING;


#endif // E_DATABASE_H
