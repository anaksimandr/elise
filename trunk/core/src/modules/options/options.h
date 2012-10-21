#ifndef OPTIONS_H
#define OPTIONS_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
//#include <QVector>
#include <QList>
#include "../../commonheaders.h"

class OptionsDialog;
class TreeModel;
class TreeItem;

class OptionsDialog : public QWidget
{
	Q_OBJECT
private:
	QTreeView*				treeView;
	//QGroupBox*				widgetArea;
	QStackedLayout*			layout;

public:
	static OptionsDialog*	options;
	OptionsDialog();
	~OptionsDialog();

	int					addPage(OPTIONSPAGE* newPage);
	void				selectPage(const QModelIndex& current, const QModelIndex& previous);

	void				okButton();
	void				applayButton();
};

int LoadOptionsModule();
int UnloadOptionsModule();

#endif // OPTIONS_H
