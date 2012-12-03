#ifndef ELISE_CORE_MODULES_OPTIONS_OPTIONS_H_
#define ELISE_CORE_MODULES_OPTIONS_OPTIONS_H_

#include <QtWidgets/QtWidgets>
#include "../../../../api/e_options.h"


class QModelIndex;

//-- Services
extern const QLatin1String	kOptionsShow_service;		//"Options/Show"
//-- Hookable events
//-- There is also "Options/Show" event
#define kOptionsShow_event kOptionsShow_service
extern const QLatin1String	kOptionsSave_event;			//"Options/Save"
extern const QLatin1String	kOptionsClose_event;		//"Options/Close"

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
