#ifndef ELISE_CORE_MODULES_PLUGINLOADER_PLUGINSTABLEMODEL_H_
#define ELISE_CORE_MODULES_PLUGINLOADER_PLUGINSTABLEMODEL_H_

#include <QtWidgets/QtWidgets>

class PluginsTreeItem;

class CheckBoxDelegate : public QStyledItemDelegate
{
public:
	CheckBoxDelegate(QObject* parent)
		: QStyledItemDelegate(parent) {}

	QSize sizeHint(const QStyleOptionViewItem&, const QModelIndex&) const;
};

class PluginsTreeModel : public QAbstractItemModel
{
	Q_OBJECT
private:
	PluginsTreeItem*	rootItem;

	PluginsTreeItem*	getItem(const QModelIndex& itemIndex) const;

public:
	PluginsTreeModel(QObject* parent = 0);
	~PluginsTreeModel();

	QModelIndex index(int row, int column, const QModelIndex& parentIndex = QModelIndex()) const;
	QModelIndex parent(const QModelIndex& itemIndex) const;

	QVariant data(const QModelIndex& itemIndex, int role) const;

	int rowCount(const QModelIndex& parentIndex = QModelIndex()) const;
	int columnCount(const QModelIndex& = QModelIndex()) const;

	Qt::ItemFlags flags(const QModelIndex& itemIndex) const;

	bool setData(const QModelIndex& itemIndex, const QVariant& value, int role = Qt::EditRole);
	bool insert(bool pluginLoaded, const QString& pluginName, const QString& pluginVersion,
				const QString pluginUuid, const QModelIndex& parentIndex = QModelIndex());
	bool remove(const QModelIndex& itemIndex = QModelIndex());
};

#endif // ELISE_CORE_MODULES_PLUGINLOADER_PLUGINSTABLEMODEL_H_
