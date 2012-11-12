#ifndef ELISE_API_E_OPTIONS_H_
#define ELISE_API_E_OPTIONS_H_

class QLatin1String;
class QString;
class QWidget;

const QLatin1String	OPTIONS_SHOW		=	QLatin1String("Options/Show");
const QLatin1String	OPTIONS_ADD_PAGE	=	QLatin1String("Options/AddPage");
const QLatin1String	OPTIONS_SAVE		=	QLatin1String("Options/Save");
const QLatin1String	OPTIONS_CLOSE		=	QLatin1String("Options/Close");

//-- Functions that will be called during the save settings
typedef int (*OptionsSaver)();

/*
QWidget* page size is 478x448;
*/

typedef struct
{
	QString		id;
	QString		parentId;
	QString		title;
	QWidget*	page;
	OptionsSaver	savePage;
} OptionsPage;


#endif // ELISE_API_E_OPTIONS_H_
