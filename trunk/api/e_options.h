#ifndef ELISE_API_E_OPTIONS_H_
#define ELISE_API_E_OPTIONS_H_

class QLatin1String;
class QString;
class QWidget;

/* Use this in your code if need
 *
 * //-- Services
 * const QLatin1String	kOptionsShow_service	=	QLatin1String("Options/Show");
 *
 * //-- Hookable events
 * //-- There is also "Options/Show" event
 * #define kOptionsShow_event kOptionsShow_service
 * const QLatin1String	kOptionsSave_event		=	QLatin1String("Options/Save");
 * const QLatin1String	kOptionsClose_event		=	QLatin1String("Options/Close");
*/

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
