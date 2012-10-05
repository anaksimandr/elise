#include "../../commonheaders.h"

QSize TreeItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	return QSize(20,20);
}

void TreeItemDelegate::paint(QPainter* painter,
							 const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	QStyleOptionViewItem opt = option;

	if ((index.isValid()) && (index.column() == 0) && !index.parent().isValid()) {

		//opt.displayAlignment = Qt::AlignCenter;

		QLinearGradient gradient(opt.rect.bottomLeft(), opt.rect.bottomRight());
		gradient.setColorAt(0.0, QColor(175, 175, 175, 0));
		gradient.setColorAt(0.5, QColor(175, 175, 175, 255));
		gradient.setColorAt(1.0, QColor(175, 175, 175, 0));

		painter->save();
		painter->setPen(QPen(QBrush(gradient), 1));
		//painter->drawLine(opt.rect.topLeft(), opt.rect.topRight());
		painter->drawLine(opt.rect.bottomLeft(), opt.rect.bottomRight());
		painter->restore();
	}

	QStyledItemDelegate::paint(painter, opt, index);
}

TreeModel::TreeModel(const QString& header, const QString& id, QWidget* widget,
					 const int index, QObject* parent)
	: QAbstractItemModel(parent)
{
	rootItem = new TreeItem(header, id, widget, index);
}

TreeModel::~TreeModel()
{
	delete rootItem;
}

int TreeModel::rowCount(const QModelIndex& parentIndex) const
{
	TreeItem* parent = getItem(parentIndex);

	return parent->childCount();
}

int TreeModel::columnCount(const QModelIndex& /* parentIndex */) const
{
	return 1;
}

int TreeModel::getLayoutIndex(const QModelIndex& itemIndex) const
{
	if (!itemIndex.isValid())
		return -1;

	TreeItem* item = getItem(itemIndex);
	return item->getLayoutIndex();
}

QVariant TreeModel::data(const QModelIndex& itemIndex, int role) const
{
	if (!itemIndex.isValid())
		return QVariant();

	if (role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	TreeItem* item = getItem(itemIndex);

	return item->getHeader();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex& itemIndex) const
{
	if (!itemIndex.isValid())
		return 0;

	//return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

TreeItem* TreeModel::getItem(const QModelIndex& itemIndex) const
{
	if (itemIndex.isValid()) {
		TreeItem* item = static_cast<TreeItem*>(itemIndex.internalPointer());
		if (item)
			return item;
	}
	return rootItem;
}

QModelIndex TreeModel::match(const QModelIndex& startIndex, const QString& header) const
{
	QModelIndex resultIndex;
	QModelIndex p = parent(startIndex);
	int from = startIndex.row();
	int to = rowCount(p);

	for (int r = from; r < to; ++r) {
		resultIndex = index(r, 0, p);

		if (!resultIndex.isValid())
			continue;

		//QString s = data(resultIndex, Qt::DisplayRole).toString();
		TreeItem* item = getItem(resultIndex);
		QString s = item->getId();

		if (header == s)
			return resultIndex;

		//-- search the hierarchy
		if (hasChildren(resultIndex)) {
			resultIndex = match(index(0, resultIndex.column(), resultIndex), header);
			if (resultIndex.isValid())
				return resultIndex;
		}
	}

	return QModelIndex();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex& parentIndex) const
{
	if (parentIndex.isValid() && parentIndex.column() != 0)
		return QModelIndex();

	TreeItem* parent = getItem(parentIndex);

	TreeItem* child = parent->child(row);
	if (child)
		return createIndex(row, column, child);
	else
		return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex& itemIndex) const
{
	if (!itemIndex.isValid())
		return QModelIndex();

	TreeItem* child = getItem(itemIndex);
	TreeItem* parent = child->parent();

	if (parent == rootItem)
		return QModelIndex();

	return createIndex(parent->childNumber(), 0, parent);
}

bool TreeModel::insert(const QModelIndex& parentIndex, QString& header, QString& id,
					   QWidget* widget, int index)
{
	if (header.isEmpty())
		return false;

	TreeItem* parent = getItem(parentIndex);
	bool success;

	beginInsertRows(parentIndex, 0, 0);
	success = parent->insertChild(header, id, widget, index);
	endInsertRows();

	return success;
}

bool TreeModel::remove(const QModelIndex& itemIndex)
{
	if (!itemIndex.isValid())
		return false;

	TreeItem* item = getItem(itemIndex);
	TreeItem* parent = item->parent();

	QModelIndex parentIndex = this->parent(itemIndex);

	bool success;
	int pos = item->childNumber();

	beginRemoveRows(parentIndex, pos, pos);
	success = parent->removeChild(pos);
	endRemoveRows();

	return success;
}

bool TreeModel::setData(const QModelIndex& itemIndex, const QVariant& value, int role)
{
	if (role != Qt::EditRole)
		return false;

	TreeItem* item = getItem(itemIndex);
	item->setHeader(value.toString());

	emit dataChanged(itemIndex, itemIndex);

	return true;
}
