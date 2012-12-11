/*  Elise IM, free crossplatform IM client
	Copyright (C) 2012  Elise project

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
