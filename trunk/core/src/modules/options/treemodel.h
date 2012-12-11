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
#ifndef ELISE_CORE_MODULES_OPTIONS_TREEMODEL_H_
#define ELISE_CORE_MODULES_OPTIONS_TREEMODEL_H_

#include <QtWidgets/QtWidgets>

class TreeItem;

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
	bool insert(const QString& header, const QString& id, int index,
				const QModelIndex& parentIndex = QModelIndex());
	bool remove(const QModelIndex& itemIndex = QModelIndex());

};

#endif // ELISE_CORE_MODULES_OPTIONS_TREEMODEL_H_
