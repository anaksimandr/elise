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
#ifndef ELISE_CORE_MODULES_OPTIONS_OPTIONS_H_
#define ELISE_CORE_MODULES_OPTIONS_OPTIONS_H_

#include <QtWidgets/QtWidgets>
#include "../../../../api/e_options.h"


class QModelIndex;

//-- Services
extern const QLatin1String	kOptionsShow_service;
//-- Hookable events
//-- There is also "Options/Show" event
#define kOptionsShow_event kOptionsShow_service
extern const QLatin1String	kOptionsSave_event;
extern const QLatin1String	kOptionsClose_event;

class OptionsDialog : public QWidget
{
	Q_OBJECT
private:
	QTreeView*				treeView_;
	QStackedLayout*			layout_;
	QSet<OptionsSaver>		saveFunctions_;

public:
	static OptionsDialog*	options_;
	OptionsDialog();
	~OptionsDialog();

	static int			addPage(OptionsPage* newPage);
	//static void			addDefaultPages();
	void				selectPage(const QModelIndex& current, const QModelIndex&);

	void				applay();

	void				okButton();

	static int			showOptions(intptr_t, intptr_t);
	static int			loadOptionsModule();
	static int			unloadOptionsModule();
};

int LoadOptionsModule();
int UnloadOptionsModule();

#endif // ELISE_CORE_MODULES_OPTIONS_OPTIONS_H_
