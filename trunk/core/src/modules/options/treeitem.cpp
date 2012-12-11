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

#include <QtWidgets/QtWidgets>
#include "treeitem.h"


TreeItem::TreeItem(const QString& headerExt, const QString& idExt, const int indx, TreeItem* parent)
{
	this->parentItem = parent;
	this->header = headerExt;
	this->id = idExt;
	this->index = indx;
}

TreeItem::~TreeItem()
{
	qDeleteAll(childItems);
}

TreeItem* TreeItem::child(int number)
{
	return childItems.value(number);
}

TreeItem* TreeItem::parent()
{
	return parentItem;
}

int TreeItem::childCount() const
{
	return childItems.count();
}

int TreeItem::childNumber() const
{
	if (parentItem)
		return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));

	return 0;
}

int TreeItem::getLayoutIndex() const
{
	return index;
}

QString TreeItem::getHeader() const
{
	return header;
}

QString TreeItem::getId() const
{
	return id;
}

bool TreeItem::insertChild(const QString& headerExt, const QString& idExt, int indx)
{
	TreeItem* item = new TreeItem(headerExt, idExt, indx, this);

	if (childItems.count() == 0)
		childItems.insert(0, item);
	else {
		QList<TreeItem*>::iterator i;
		for (i = childItems.begin(); i != childItems.end(); ++i) {
			if ((*i)->getHeader().toLower() > headerExt.toLower()) {
				childItems.insert(i, item);
				return true;
			}
		}
		childItems.insert(i, item);
	}

	return true;
}

bool TreeItem::removeChild(int position)
{
	if (position < 0 || position > childItems.size())
		return false;

	delete childItems.takeAt(position);

	return true;
}

void TreeItem::setHeader(const QString &value)
{
	header = value;
}
/*
bool lessThan(const TreeItem* i1, const TreeItem* i2)
{
	QString s1 = i1->getHeader();
	QString s2 = i2->getHeader();
	return s1.toLower() < s2.toLower();
}

void TreeItem::sortChildren()
{
	qSort(childItems.begin(), childItems.end(), lessThan);
}*/
