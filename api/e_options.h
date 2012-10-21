#ifndef E_OPTIONS_H
#define E_OPTIONS_H

const QString	OPTIONS_SHOW		=	QStringLiteral("Options/Show");
const QString	OPTIONS_ADD_PAGE	=	QStringLiteral("Options/AddPage");
const QString	OPTIONS_SAVE		=	QStringLiteral("Options/Save");
const QString	OPTIONS_CLOSE		=	QStringLiteral("Options/Close");

/*
QWidget* page size is 478x448;
*/

typedef struct
{
	QString		id;
	QString		parentId;
	QString		title;
	QWidget*	page;

} OPTIONSPAGE;


#endif // E_OPTIONS_H
