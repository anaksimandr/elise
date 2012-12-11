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
#ifndef ELISE_CORE_MODULES_OPTIONS_TREEITEM_H_
#define ELISE_CORE_MODULES_OPTIONS_TREEITEM_H_

class QString;
template <typename T>
class QList;

class TreeItem
{
private:
	QString				id;
	QString				header;
	int					index;	//-- index in QStackedLayout
	TreeItem*			parentItem;
	QList<TreeItem*>	childItems;
public:
	TreeItem(const QString& headerExt, const QString& idExt, const int indx, TreeItem* parent = 0);
	~TreeItem();

	TreeItem*	parent();
	TreeItem*	child(int number);
	int			childCount() const;
	int			childNumber() const;
	int			getLayoutIndex() const;
	QString		getHeader() const;
	QString		getId() const;
	bool		insertChild(const QString& headerExt, const QString& idExt, int indx);
	bool		removeChild(int position);
	void		setHeader(const QString& value);
};

#endif // ELISE_CORE_MODULES_OPTIONS_TREEITEM_H_
