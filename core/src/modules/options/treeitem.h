#ifndef TREEITEM_H
#define TREEITEM_H

#include "../../commonheaders.h"

class TreeItem
{
private:
	QList<TreeItem*>	childItems;
	QString				header;
	TreeItem*			parentItem;
public:
	TreeItem(const QString& header, TreeItem* parent = 0);
	~TreeItem();

	TreeItem*	parent();
	TreeItem*	child(int number);
	int			childCount() const;
	int			childNumber() const;
	QString		getHeader() const;
	bool		insertChild(QString& header);
	bool		removeChild(int position);
	void		setHeader(const QString& value);
	void		sortChildren();
};

#endif // TREEITEM_H
