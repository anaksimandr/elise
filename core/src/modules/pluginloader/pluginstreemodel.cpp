#include "pluginstreemodel.h"
#include "pluginstreeitem.h"

QSize CheckBoxDelegate::sizeHint(const QStyleOptionViewItem&, const QModelIndex&) const
{
	return QSize(50, 20);
}

PluginsTreeModel::PluginsTreeModel(QObject* parent)
	: QAbstractItemModel(parent)
{
	QString str;
	rootItem = new PluginsTreeItem(str, str, str);
}

PluginsTreeModel::~PluginsTreeModel()
{
	delete rootItem;
}

PluginsTreeItem* PluginsTreeModel::getItem(const QModelIndex& itemIndex) const
{
	if (itemIndex.isValid()) {
		PluginsTreeItem* item = static_cast<PluginsTreeItem*>(itemIndex.internalPointer());
		if (item)
			return item;
	}
	return rootItem;
}

QModelIndex PluginsTreeModel::index(int row, int column, const QModelIndex& parentIndex) const
{
	//if (parentIndex.isValid() && parentIndex.column() != 0)
	//return QModelIndex();

	PluginsTreeItem* parent = getItem(parentIndex);

	PluginsTreeItem* child = parent->child(row);
	if (child)
		return createIndex(row, column, child);
	else
		return QModelIndex();
}

QModelIndex PluginsTreeModel::parent(const QModelIndex& itemIndex) const
{
	if (!itemIndex.isValid())
		return QModelIndex();

	PluginsTreeItem* child = getItem(itemIndex);
	PluginsTreeItem* parent = child->parent();

	if (parent == rootItem)
		return QModelIndex();

	return createIndex(parent->childNumber(), 0, parent);
}


QVariant PluginsTreeModel::data(const QModelIndex& itemIndex, int role) const
{
	if (!itemIndex.isValid())
		return QVariant();

	PluginsTreeItem* item = static_cast<PluginsTreeItem*>(itemIndex.internalPointer());

	if (role == Qt::CheckStateRole && itemIndex.column() == 0 )
		return static_cast<int>(item->isPluginLoaded() ? Qt::Checked : Qt::Unchecked );

	if (role != Qt::DisplayRole)
		return QVariant();

	switch (itemIndex.column()) {
		case 0:
			return item->getPluginModuleName();
			break;
		case 1:
			return item->getPluginName();
			break;
		case 2:
			return item->getPluginVersion();
			break;
		default:
#ifndef NDEBUG
			QMessageBox::critical(0, "Error", "Wrong item index!", QMessageBox::Cancel);
#endif
			return QVariant();
	}
}


int PluginsTreeModel::rowCount(const QModelIndex& parentIndex) const
{
	PluginsTreeItem* parent = getItem(parentIndex);

	return parent->childCount();
}

int PluginsTreeModel::columnCount(const QModelIndex&) const
{
	return 3;
}


Qt::ItemFlags PluginsTreeModel::flags(const QModelIndex& itemIndex) const
{
	if (!itemIndex.isValid())
		return 0;

	Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

	if (itemIndex.column() == 0 )
		flags |= Qt::ItemIsUserCheckable;

	return flags;
}

bool PluginsTreeModel::setData(const QModelIndex& itemIndex, const QVariant& value, int role)
{
	if (role != Qt::CheckStateRole || itemIndex.column() != 0)
		return false;

	PluginsTreeItem* item = static_cast<PluginsTreeItem*>(itemIndex.internalPointer());
	item->setPluginLoaded(value.toBool());

	emit dataChanged(itemIndex, itemIndex);

	return true;
}

bool PluginsTreeModel::insert(const QString& pluginModuleName, const QString& pluginName,
							  const QString& pluginVersion, const QModelIndex& parentIndex)
{
	if (pluginName.isEmpty() || pluginVersion.isEmpty() || pluginUuid.isEmpty())
		return false;

	PluginsTreeItem* parent = getItem(parentIndex);
	bool success;

	beginInsertRows(parentIndex, 0, 0);
	success = parent->insertChild(pluginModuleName, pluginName, pluginVersion);
	endInsertRows();

	return success;
}

bool PluginsTreeModel::remove(const QModelIndex& itemIndex)
{
	if (!itemIndex.isValid())
		return false;

	PluginsTreeItem* item = getItem(itemIndex);
	PluginsTreeItem* parent = item->parent();

	QModelIndex parentIndex = this->parent(itemIndex);

	bool success;
	int pos = item->childNumber();

	beginRemoveRows(parentIndex, pos, pos);
	success = parent->removeChild(pos);
	endRemoveRows();

	return success;
}
