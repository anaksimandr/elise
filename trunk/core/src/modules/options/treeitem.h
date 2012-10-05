#ifndef TREEITEM_H
#define TREEITEM_H

#include "../../commonheaders.h"

class TreeItem
{
private:
	QString				id;
	QString				header;
	QWidget*			page;
	int					index;	//-- index in QStackedLayout
	TreeItem*			parentItem;
	QList<TreeItem*>	childItems;
public:
	TreeItem(const QString& headerExt, const QString& idExt, QWidget* widget, const int indx,
			 TreeItem* parent = 0);
	~TreeItem();

	TreeItem*	parent();
	TreeItem*	child(int number);
	int			childCount() const;
	int			childNumber() const;
	int			getLayoutIndex() const;
	QString		getHeader() const;
	QString		getId() const;
	QWidget*	getWidget() const;
	bool		insertChild(QString& headerExt, QString& idExt, QWidget* widget, int indx);
	bool		removeChild(int position);
	void		setHeader(const QString& value);
};

#endif // TREEITEM_H
