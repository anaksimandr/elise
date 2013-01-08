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

#ifndef ELISE_API_E_DATABASE_H_
#define ELISE_API_E_DATABASE_H_

class QLatin1String;
class QString;
class QByteArray;

//-- Types of settings
#define __Int_Type		0	//-- int
#define __Real_Type		1	//-- double
#define __Text_Type		2	//-- QString
#define __Blob_Type		3	//-- QByteArray

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
	int			contact;
	QString*	qsModule;
	QString*	qsSetting;
	DBVariant*	var;
} Setting;


#endif // ELISE_API_E_DATABASE_H_
