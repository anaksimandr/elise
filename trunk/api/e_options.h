#ifndef ELISE_API_E_OPTIONS_H_
#define ELISE_API_E_OPTIONS_H_

class QLatin1String;
class QString;
class QWidget;

//-- Services
const QLatin1String	OPTIONS_SHOW		=	QLatin1String("Options/Show");
//const QLatin1String	OPTIONS_ADD_PAGE	=	QLatin1String("Options/AddPage");
//-- Hookable events
//-- There is also OPTIONS_SHOW event
const QLatin1String	OPTIONS_SAVE		=	QLatin1String("Options/Save");
const QLatin1String	OPTIONS_CLOSE		=	QLatin1String("Options/Close");

//-- Functions that will be called during the save settings
typedef void (*OptionsSaver)();

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

//-- Pointer to function that you need to call to add page on options dialog.
//-- Will be returned as first parameter in hook OPTIONS_SHOW
typedef int (*PageAdder)(OptionsPage*);


#endif // ELISE_API_E_OPTIONS_H_
