#ifndef OPTIONS_H
#define OPTIONS_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
//#include <QVector>
#include <QList>
#include "../commonheaders.h"

class OptionsDialog;
class TreeModel;
class TreeItem;

class TreeItem
{
private:
	QList<TreeItem*>	childItems;
	QVariant			itemData;
	QString				itemInfo;
	TreeItem*			parentItem;
public:
	TreeItem(const QVariant &data, const QString& header, TreeItem* parent = 0);
	~TreeItem();

	TreeItem*	parent();
	TreeItem*	child(int number);
	int			childCount() const;
	int			columnCount() const;
	int			childNumber() const;
	QVariant	data() const;
	QString		info() const;
	bool		insertChildren(int position, int count);
	bool		removeChildren(int position, int count);
	bool		setData(const QVariant& value);
	bool		setInfo(const QString& value);
};

class TreeModel : public QAbstractItemModel
{
	Q_OBJECT
private:
	TreeItem*	rootItem;

	TreeItem*	getItem(const QModelIndex &index) const;

public:
	TreeModel(const QString &header, const QString& data, QObject *parent = 0);
	~TreeModel();

	QModelIndex match(const QModelIndex& start, const QString& value) const;

	QVariant data(const QModelIndex& index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	QString info(const QModelIndex& index) const;

	QModelIndex index(int row, int column = 1, const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex& index) const;

	int rowCount(const QModelIndex& parent = QModelIndex()) const;
	int columnCount(const QModelIndex& parent = QModelIndex()) const;

	Qt::ItemFlags flags(const QModelIndex& index) const;
	bool setInfo(const QModelIndex& index, const QString& value);
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
	//bool setHeaderData(int section, Qt::Orientation orientation,
					   //const QVariant &value, int role = Qt::EditRole);

	bool insert(int position, const QModelIndex& parent = QModelIndex());
	bool remove(int position, const QModelIndex& parent = QModelIndex());

};

class OptionsDialog : public QWidget
{
	Q_OBJECT
private:
	QTreeView*				treeView;
	QGroupBox*				widgetClientArea;

	QLineEdit*				edit;
public:
	static OptionsDialog*	options;
	OptionsDialog();
	~OptionsDialog();

	//int						insertItem();
	bool					addChild();
	bool					findItem();
};

int LoadOptionsModule();
int UnloadOptionsModule();

#endif // OPTIONS_H
