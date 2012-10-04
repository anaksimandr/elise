#include "../../commonheaders.h"

TreeItem::TreeItem(const QString& header, TreeItem* parent)
{
	parentItem = parent;
	this->header = header;
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

QString TreeItem::getHeader() const
{
	return header;
}

bool TreeItem::insertChild(QString& header)
{
	TreeItem* item = new TreeItem(header, this);
	childItems.insert(childItems.size(), item);

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

bool lessThan(const TreeItem* i1, const TreeItem* i2)
{
	QString s1 = i1->getHeader();
	QString s2 = i2->getHeader();
	return s1.toLower() < s2.toLower();
}

void TreeItem::sortChildren()
{
	qSort(childItems.begin(), childItems.end(), lessThan);
}
