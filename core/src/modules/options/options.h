#ifndef ELISE_CORE_MODULES_OPTIONS_OPTIONS_H_
#define ELISE_CORE_MODULES_OPTIONS_OPTIONS_H_

#include <QtWidgets/QtWidgets>
#include "../../../../api/e_options.h"


class QModelIndex;

//-- Services
extern const QLatin1String	OPTIONS_SHOW;		//"Options/Show"
//-- Hookable events
//-- There is also OPTIONS_SHOW event
extern const QLatin1String	OPTIONS_SAVE;		//"Options/Save"
extern const QLatin1String	OPTIONS_CLOSE;		//"Options/Close"

class OptionsDialog : public QWidget
{
	Q_OBJECT
private:
	QTreeView*				treeView;
	QStackedLayout*			layout;
	QSet<OptionsSaver>		saveFunctions;

public:
	static OptionsDialog*	options;
	OptionsDialog();
	~OptionsDialog();

	static int			addPage(OptionsPage* newPage);
	static void			addDefaultPages();
	void				selectPage(const QModelIndex& current, const QModelIndex&);

	void				applay();

	void				okButton();
};

int LoadOptionsModule();
int UnloadOptionsModule();

#endif // ELISE_CORE_MODULES_OPTIONS_OPTIONS_H_
