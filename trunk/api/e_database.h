#ifndef ELISE_API_E_DATABASE_H_
#define ELISE_API_E_DATABASE_H_

class QLatin1String;
class QString;
class QByteArray;

//-- Types of settings
/*
  0;	int
  1;	double
  2;	QString
  3;	QByteArray
*/

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
