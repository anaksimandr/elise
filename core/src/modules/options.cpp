
#include "../commonheaders.h"

OptionsDialog* OptionsDialog::options = 0;













OptionsDialog::OptionsDialog()
{
	this->setFixedSize(700, 500);
	//-- Options tree
	optionsTree = new QTreeView(this);
	optionsTree->move(0, 0);
	optionsTree->resize(200, this->height());
	QStandardItemModel* model = new QStandardItemModel;
	model->appendRow(new QStandardItem("test"));
	optionsTree->setModel(model);
	//-- Client area for current settings
	widgetClientArea = new QGroupBox(this);
	widgetClientArea->move(optionsTree->width() + 10, 10);
	widgetClientArea->resize(this->width() - optionsTree->width() - 10 - 10,
							 this->height() - 10 - 40);
	QPalette p = widgetClientArea->palette();
	p.setColor(QPalette::Window, Qt::red);
	//p.setColor(QPalette::WindowText, Qt::red);
	//p.setColor(QPalette::Base, Qt::red);
	//p.setColor(QPalette::AlternateBase, Qt::red);
	//p.setColor(QPalette::Text, Qt::red);
	widgetClientArea->setPalette(p);
	//if (widgetClientArea->palette(). == QPalette::NoRole)
		//QMessageBox::critical(0, "Debug", "bingo nahui", QMessageBox::Ok);
	//widgetClientArea->show();
	//QTreeView* vv = new QTreeView(widgetClientArea);
	//vv->resize(vv->parentWidget()->size());
}

OptionsDialog::~OptionsDialog()
{
	OptionsDialog::options = 0;
}


int ShowOptions(uintptr_t wParam,uintptr_t lParam)
{
	if (OptionsDialog::options == 0)
		OptionsDialog::options = new OptionsDialog();
	OptionsDialog::options->show();
	return 0;
}

int LoadOptionsModule()
{
	OptionsDialog::options = 0;
	//CreateHookableEvent(&TRAY_SINGLECLICK);
	//CreateHookableEvent(&TRAY_DOUBLECLICK);
	//CreateHookableEvent(&TRAY_MIDDLECLICK);
	CreateServiceFunction(&OPTIONS_SHOW, (ELISESERVICE)ShowOptions);
	return 0;
}

int UnloadOptionsModule()
{
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//-- class TreeModel --/////////////////////////////////////////////////////////////////////////////

TreeModel::TreeModel(const QString& header, QObject* parent)
	: QAbstractItemModel(parent)
{
	rootItem = new TreeItem(header);
}

TreeModel::~TreeModel()
{
	delete rootItem;
}

TreeItem* TreeModel::getItem(const QModelIndex& index) const
{
	if (index.isValid()) {
		TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
		if (item) return item;
	}
	return rootItem;
}

int TreeModel::rowCount(const QModelIndex& parent) const
{
	TreeItem* parentItem = getItem(parent);

	return parentItem->childCount();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

int TreeModel::columnCount(const QModelIndex& /* parent */) const
{
	return rootItem->columnCount();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex& parent) const
{
	if (parent.isValid() && parent.column() != 0)
		return QModelIndex();
	TreeItem* parentItem = getItem(parent);

	TreeItem* childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex& index) const
{
	if (!index.isValid())
		return QModelIndex();

	TreeItem* childItem = getItem(index);
	TreeItem* parentItem = childItem->parent();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool TreeModel::insertRows(int position, int rows, const QModelIndex& parent)
{
	TreeItem* parentItem = getItem(parent);
	bool success;

	beginInsertRows(parent, position, position + rows - 1);
	success = parentItem->insertChildren(position, rows, rootItem->columnCount());
	endInsertRows();

	return success;
}

bool TreeModel::removeColumns(int position, int columns, const QModelIndex& parent)
{
	bool success;

	beginRemoveColumns(parent, position, position + columns - 1);
	success = rootItem->removeColumns(position, columns);
	endRemoveColumns();

	if (rootItem->columnCount() == 0)
		removeRows(0, rowCount());

	return success;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex& parent)
{
	TreeItem* parentItem = getItem(parent);
	bool success = true;

	beginRemoveRows(parent, position, position + rows - 1);
	success = parentItem->removeChildren(position, rows);
	endRemoveRows();

	return success;
}

int TreeModel::rowCount(const QModelIndex& parent) const
{
	TreeItem* parentItem = getItem(parent);

	return parentItem->childCount();
}

bool TreeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (role != Qt::EditRole)
		return false;
	TreeItem* item = getItem(index);
	bool result = item->setData(index.column(), value);
	if (result)
		emit dataChanged(index, index);
	return result;
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
							  const QVariant& value, int role)
{
	if (role != Qt::EditRole || orientation != Qt::Horizontal)
		return false;
	bool result = rootItem->setData(section, value);
	if (result)
		emit headerDataChanged(orientation, section, section);
	return result;
}

//-- class TreeModel --/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
//-- class TreeItem --//////////////////////////////////////////////////////////////////////////////

TreeItem::TreeItem(const QString& title, const QVariant& data, TreeItem* parent)
{
	parentItem = parent;
	itemData = data;
	itemTitle = title;
}

TreeItem::~TreeItem()
{
	qDeleteAll(childItems);
}

TreeItem* TreeItem::child(int number)
{
	return childItems.value(number);
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

int TreeItem::columnCount() const
{
	return itemData.count();
}

QVariant TreeItem::data(int column) const
{
	return itemData;
}

bool TreeItem::insertChildren(int position, int count, int columns)
{
	if (position < 0 || position > childItems.size())
		return false;

	for (int row = 0; row < count; ++row) {
		QVariant data;
		QString title;
		TreeItem* item = new TreeItem(title, data, this);
		childItems.insert(position, item);
	}

	return true;
}

bool TreeItem::insertColumns(int position, int columns)
{
	return false;
}

TreeItem* TreeItem::parent()
{
	return parentItem;
}

bool TreeItem::removeChildren(int position, int count)
{
	if (position < 0 || position + count > childItems.size())
		return false;

	for (int row = 0; row < count; ++row)
		delete childItems.takeAt(position);

	return true;
}

bool TreeItem::removeColumns(int position, int columns)
{
	if (position < 0 || position + columns > itemData.size())
		return false;

	for (int column = 0; column < columns; ++column)
		itemData.remove(position);

	foreach (TreeItem *child, childItems)
		child->removeColumns(position, columns);

	return true;
}

bool TreeItem::setData(int column, const QVariant& value)
{
	if (column < 0 || column >= itemData.size())
		return false;

	itemData = value;
	return true;
}

//-- class TreeItem --//////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
