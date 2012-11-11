#ifndef ELISE_CORE_MODULES_OPTIONS_OPTIONS_H_
#define ELISE_CORE_MODULES_OPTIONS_OPTIONS_H_

#include <QtWidgets/QtWidgets>
#include "../../../../api/e_options.h"
//#include "../../commonheaders.h"

class QModelIndex;

//class OptionsDialog;
//class TreeModel;
//class TreeItem;

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

	int					addPage(OptionsPage* newPage);
	void				selectPage(const QModelIndex& current, const QModelIndex& previous);

	void				applay();

	void				okButton();
};

int LoadOptionsModule();
int UnloadOptionsModule();

#endif // ELISE_CORE_MODULES_OPTIONS_OPTIONS_H_
