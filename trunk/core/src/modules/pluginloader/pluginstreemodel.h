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
#ifndef ELISE_CORE_MODULES_PLUGINLOADER_PLUGINSTABLEMODEL_H_
#define ELISE_CORE_MODULES_PLUGINLOADER_PLUGINSTABLEMODEL_H_

#include <QtWidgets/QtWidgets>

class PluginsTreeItem;

class CheckBoxDelegate : public QStyledItemDelegate
{
public:
	CheckBoxDelegate(QObject* parent)
		: QStyledItemDelegate(parent) {}

	inline QSize sizeHint(const QStyleOptionViewItem&, const QModelIndex&) const
	{ return QSize(20,20); }
};

class PluginsTreeModel : public QAbstractItemModel
{
	Q_OBJECT
private:
	PluginsTreeItem*		rootItem;
	QTreeView*				treeView;
	//QMap<QString, Plugin>*	plugins;

	inline PluginsTreeItem*	getItem(const QModelIndex& itemIndex) const
	{
		if (itemIndex.isValid()) {
			PluginsTreeItem* item = static_cast<PluginsTreeItem*>(itemIndex.internalPointer());
			if (item)
				return item;
		}
		return rootItem;
	}

public:
	PluginsTreeModel(QTreeView* treeViewExt, QObject* parent = 0);
	~PluginsTreeModel();

	QModelIndex index(int row, int column, const QModelIndex& parentIndex = QModelIndex()) const;
	QModelIndex parent(const QModelIndex& itemIndex) const;

	QVariant	data(const QModelIndex& itemIndex, int role) const;

	int			rowCount(const QModelIndex& parentIndex = QModelIndex()) const;
	int			columnCount(const QModelIndex& = QModelIndex()) const;

	Qt::ItemFlags flags(const QModelIndex& itemIndex) const;

	//bool setData(const QModelIndex& itemIndex, const QVariant& value, int role = Qt::EditRole);
	bool		insert(const QString& pluginModuleName, const QString& pluginName,
					   const QString& pluginVersion,
					   const QModelIndex& parentIndex = QModelIndex());
	bool		remove(const QModelIndex& itemIndex = QModelIndex());
	void		updateLoadControls(bool update) const;
};

#endif // ELISE_CORE_MODULES_PLUGINLOADER_PLUGINSTABLEMODEL_H_
