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
	QGroupBox*				widgetArea;

	QLineEdit*				edit;
public:
	static OptionsDialog*	options;
	OptionsDialog();
	~OptionsDialog();

	bool					addPage(QWidget *page);

	void					sort();

	//int						insertItem();
	bool					addChild();
	bool					deleteChild();
	bool					findItem();
};

int LoadOptionsModule();
int UnloadOptionsModule();

#endif // OPTIONS_H
