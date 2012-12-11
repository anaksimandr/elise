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

#include "pluginstreemodel.h"
#include "pluginstreeitem.h"

PluginsTreeModel::PluginsTreeModel(QTreeView* treeViewExt, QObject* parent)
	: QAbstractItemModel(parent)
{
	treeView = treeViewExt;
	QString str;
	rootItem = new PluginsTreeItem(str, str, str, this);
}

PluginsTreeModel::~PluginsTreeModel()
{
	delete rootItem;
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

	if (role != Qt::DisplayRole)
		return QVariant();

	switch (itemIndex.column()) {
		case 0:
			return QVariant();
			break;
		case 1:
			return item->getPluginModuleName();
			break;
		case 2:
			return item->getPluginName();
			break;
		case 3:
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
	return 4;
}

Qt::ItemFlags PluginsTreeModel::flags(const QModelIndex& itemIndex) const
{
	if (!itemIndex.isValid())
		return 0;

	Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

	//PluginsTreeItem* item = getItem(itemIndex);
	//if (item->isPluginLoaded() || item->isPluginLoadable())
	//	flags |=  Qt::ItemIsEnabled;

	//if (itemIndex.column() == 0 )
	//	flags |= Qt::ItemIsUserCheckable;

	return flags;
}

void PluginsTreeModel::updateLoadControls(bool update) const
{
	QModelIndex itemIndex;
	QModelIndex p = parent(index(0,0));
	int to = rowCount(p);

	for (int r = 0; r < to; ++r) {
		itemIndex = index(r, 0, p);

		PluginsTreeItem* item = getItem(itemIndex);
		if (update)
			//-- Update state
			item->isControlActive();
		else
			//-- Insert control in model
			treeView->setIndexWidget(itemIndex, item->getLoadControl());

	}
}

bool PluginsTreeModel::insert(const QString& pluginModuleName, const QString& pluginName,
							  const QString& pluginVersion, const QModelIndex& parentIndex)
{
	if (pluginName.isEmpty() || pluginVersion.isEmpty() || pluginModuleName.isEmpty())
		return false;

	PluginsTreeItem* parent = getItem(parentIndex);

	beginInsertRows(parentIndex, 0, 0);
	bool success = parent->insertChild(pluginModuleName, pluginName, pluginVersion, this);
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
