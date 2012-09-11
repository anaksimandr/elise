#ifndef OPTIONS_H
#define OPTIONS_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
//#include <QVector>
#include <QList>
#include "../commonheaders.h"

class OptionsDialog;
/*class TreeModel;
class TreeItem;

class TreeItem
{
private:
	QList<TreeItem*>	childItems;
	QString				itemTitle;
	QVariant			itemData;
	TreeItem*			parentItem;
public:
	TreeItem(const QString& title, const QVariant& data, TreeItem* parent = 0);
	~TreeItem();

	TreeItem*			child(int number);
	int					childCount() const;
	//int					columnCount() const;
	QVariant			data(int column) const;
	bool				insertChildren(int position, int count, int columns);
	//bool				insertColumns(int position, int columns);
	TreeItem*			parent();
	bool				removeChildren(int position, int count);
	//bool				removeColumns(int position, int columns);
	int					childNumber() const;
	bool				setData(int column, const QVariant& value);
};

class TreeModel : public QAbstractItemModel
{
	Q_OBJECT
private:
	TreeItem*	getItem(const QModelIndex& index) const;
	TreeItem*	rootItem;
public:
	TreeModel(const QString& header, QObject* parent = 0);
	~TreeModel();

	QVariant		data(const QModelIndex& index, int role) const;
	QVariant		headerData(int section, Qt::Orientation orientation,
							   int role = Qt::DisplayRole) const;
	QModelIndex		index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
	QModelIndex		parent(const QModelIndex& index) const;
	int				rowCount(const QModelIndex& parent = QModelIndex()) const;
	int				columnCount(const QModelIndex& parent = QModelIndex()) const;
	Qt::ItemFlags	flags(const QModelIndex& index) const;
	bool			setData(const QModelIndex& index, const QVariant& value,
							int role = Qt::EditRole);
	bool			setHeaderData(int section, Qt::Orientation orientation,
								  const QVariant& value, int role = Qt::EditRole);
	bool			insertColumns(int position, int columns,
								  const QModelIndex& parent = QModelIndex());
	bool			removeColumns(int position, int columns,
								  const QModelIndex& parent = QModelIndex());
	bool			insertRows(int position, int rows, const QModelIndex& parent = QModelIndex());
	bool			removeRows(int position, int rows, const QModelIndex& parent = QModelIndex());
};*/

class OptionsDialog : public QWidget
{
	Q_OBJECT
private:
	QTreeView*				optionsTree;
	QGroupBox*				widgetClientArea;
public:
	static OptionsDialog*	options;
	OptionsDialog();
	~OptionsDialog();

};

int LoadOptionsModule();
int UnloadOptionsModule();

#endif // OPTIONS_H
