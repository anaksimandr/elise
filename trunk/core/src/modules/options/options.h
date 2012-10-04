#ifndef OPTIONS_H
#define OPTIONS_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
//#include <QVector>
#include <QList>
#include "../commonheaders.h"

class OptionsDialog;
class TreeModel;
class TreeItem;

class TreeModel : public QAbstractItemModel
{
	Q_OBJECT
private:
	TreeItem*	rootItem;

	TreeItem*	getItem(const QModelIndex& index) const;

public:
	TreeModel(const QString& header, QObject *parent = 0);
	~TreeModel();

	QModelIndex match(const QModelIndex& startIndex, const QString& header) const;
	QModelIndex index(int row, int column, const QModelIndex& parentIndex = QModelIndex()) const;
	QModelIndex parent(const QModelIndex& itemIndex) const;

	QVariant data(const QModelIndex& itemIndex, int role) const;

	int rowCount(const QModelIndex& parentIndex = QModelIndex()) const;
	int columnCount(const QModelIndex& parentIndex = QModelIndex()) const;

	Qt::ItemFlags flags(const QModelIndex& itemIndex) const;

	bool setData(const QModelIndex& itemIndex, const QVariant& value, int role = Qt::EditRole);
	bool insert(const QModelIndex& parentIndex = QModelIndex(), QString& header = QString());
	bool remove(const QModelIndex& itemIndex = QModelIndex());

	void sortChildren(const QModelIndex& parentIndex);
};

class TreeItemDelegate : public QStyledItemDelegate
{
public:
	TreeItemDelegate()
	{}

	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
	void paint(QPainter* painter,
			   const QStyleOptionViewItem& option, const QModelIndex& index ) const;
};

class OptionsDialog : public QWidget
{
	Q_OBJECT
private:
	QTreeView*				treeView;
	QGroupBox*				widgetClientArea;

	QLineEdit*				edit;
public:
	static OptionsDialog*	options;
	OptionsDialog();
	~OptionsDialog();

	//int						insertItem();
	bool					addChild();
	bool					deleteChild();
	bool					findItem();
	bool					sort();
};

int LoadOptionsModule();
int UnloadOptionsModule();

#endif // OPTIONS_H
