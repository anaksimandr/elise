#ifndef E_OPTIONS_H
#define E_OPTIONS_H

const QLatin1String	OPTIONS_SHOW		=	QLatin1String("Options/Show");
const QLatin1String	OPTIONS_ADD_PAGE	=	QLatin1String("Options/AddPage");
const QLatin1String	OPTIONS_SAVE		=	QLatin1String("Options/Save");
const QLatin1String	OPTIONS_CLOSE		=	QLatin1String("Options/Close");

//-- Functions that will be called during the save settings
typedef int (*OPTIONSSAVE)();

/*
QWidget* page size is 478x448;
*/

typedef struct
{
	QString		id;
	QString		parentId;
	QString		title;
	QWidget*	page;
	OPTIONSSAVE	save;
} OPTIONSPAGE;


#endif // E_OPTIONS_H
