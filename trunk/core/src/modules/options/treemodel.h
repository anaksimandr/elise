#ifndef ELISE_CORE_MODULES_OPTIONS_TREEMODEL_H_
#define ELISE_CORE_MODULES_OPTIONS_TREEMODEL_H_

#include <QtWidgets/QtWidgets>
#include "treeitem.h"


class TreeItemDelegate : public QStyledItemDelegate
{
public:
	TreeItemDelegate(QObject* parent)
		: QStyledItemDelegate(parent)
	{}

	QSize sizeHint(const QStyleOptionViewItem&, const QModelIndex&) const;
	void paint(QPainter* painter,
			   const QStyleOptionViewItem& option, const QModelIndex& index ) const;
};

class TreeModel : public QAbstractItemModel
{
	Q_OBJECT
private:
	TreeItem*	rootItem;

	TreeItem*	getItem(const QModelIndex& index) const;

public:
	TreeModel(const QString& header = 0, const QString& id = QString(),
			  const int index = 0, QObject* parent = 0);
	~TreeModel();

	QModelIndex match(const QModelIndex& startIndex, const QString& id) const;
	QModelIndex index(int row, int column, const QModelIndex& parentIndex = QModelIndex()) const;
	QModelIndex parent(const QModelIndex& itemIndex) const;

	QVariant data(const QModelIndex& itemIndex, int role) const;

	int rowCount(const QModelIndex& parentIndex = QModelIndex()) const;
	int columnCount(const QModelIndex& parentIndex = QModelIndex()) const;

	int getLayoutIndex(const QModelIndex& itemIndex) const;

	Qt::ItemFlags flags(const QModelIndex& itemIndex) const;

	bool setData(const QModelIndex& itemIndex, const QVariant& value, int role = Qt::EditRole);
	bool insert(const QModelIndex& parentIndex = QModelIndex(), QString& header = QString(),
				QString& id = QString(), int index = 0);
	bool remove(const QModelIndex& itemIndex = QModelIndex());

};

#endif // ELISE_CORE_MODULES_OPTIONS_TREEMODEL_H_
