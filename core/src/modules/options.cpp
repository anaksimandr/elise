
#include "../commonheaders.h"

OptionsDialog* OptionsDialog::options = 0;













OptionsDialog::OptionsDialog()
{
	this->setFixedSize(700, 500);
	//-- Options tree
	treeView = new QTreeView(this);
	treeView->move(0, 0);
	treeView->resize(200, this->height());

	QString headers = "11";
	TreeModel* model1 = new TreeModel(headers, "test\ttest2\ntest3\ttest4", this);
	//TreeModel* model = new TreeModel("Test", 0);
	//model->appendRow(new QStandardItem("test"));
	//model->insertRows(1, 1);
	treeView->setModel(model1);
	treeView->setHeaderHidden(true);

	QModelIndex index = treeView->selectionModel()->currentIndex();
	QAbstractItemModel* model = treeView->model();

	if (!model->insertRow(0, index))
		QMessageBox::critical(0, "deb", "insert row failed", QMessageBox::Ok);

	QModelIndex child = model->index(0, 1, index);
	model->setData(child, QVariant("122222"), Qt::EditRole);

	dynamic_cast<TreeModel*>(model)->setInfo(child, "hhh");

	//treeView->selectionModel()->setCurrentIndex(model->index(0, 0, index),
		//									QItemSelectionModel::ClearAndSelect);

	QModelIndex index2 = model->index(0, 0);
	//dynamic_cast<TreeModel*>(model)->info(model->match(index2, 0, "hhh", 0, 0)[0]);
	QMessageBox::critical(0, "Debug",
						 dynamic_cast<TreeModel*>(model)->info(model->match(index2, 0, "hhh", 0, 0)[0]),
						  QMessageBox::Ok);


	//QModelIndex index = optionsTree->selectionModel()->currentIndex();
	//model->insertRow(index.row()+1, index.parent());

	//-- Client area for current settings
	widgetClientArea = new QGroupBox(this);
	widgetClientArea->move(treeView->width() + 10, 10);
	widgetClientArea->resize(this->width() - treeView->width() - 10 - 10,
							 this->height() - 10 - 40);
	edit = new QLineEdit(widgetClientArea);
	edit->move(20, 20);

	QPushButton* btn = new QPushButton("create", widgetClientArea);
	btn->move(50, 200);
	connect(btn, &QPushButton::clicked, this, &OptionsDialog::addChild);

	btn = new QPushButton("search", widgetClientArea);
	btn->move(130, 200);
	connect(btn, &QPushButton::clicked, this, &OptionsDialog::findItem);

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
	QMessageBox::critical(0, "Debug", "Destroy", QMessageBox::Ok);
}

bool OptionsDialog::addChild()
{
	QModelIndex index = treeView->selectionModel()->currentIndex();
	TreeModel* model = dynamic_cast<TreeModel*>(treeView->model());

	if (!model->insertRow(0, index))
		return false;

	QModelIndex child = model->index(0, 1, index);

	model->setData(child, QVariant(edit->text()), 0);

	model->setInfo(child, edit->text());
	return true;
}

bool OptionsDialog::findItem()
{
	QMessageBox::critical(0, "Debug", "find!", QMessageBox::Ok);
	return false;
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

TreeModel::TreeModel(const QString& header, const QString& data, QObject *parent)
	: QAbstractItemModel(parent)
{
	rootItem = new TreeItem(header, "HHH");
}

TreeModel::~TreeModel()
{
	delete rootItem;
}

int TreeModel::columnCount(const QModelIndex & /* parent */) const
{
	return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	TreeItem* item = getItem(index);

	return item->data();
}

QString TreeModel::info(const QModelIndex &index) const
{
	if (!index.isValid())
		return QString();

	TreeItem* item = getItem(index);

	return item->info();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
	if (index.isValid()) {
		TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
		if (item) return item;
	}
	return rootItem;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
							   int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data();

	return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
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

bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
	TreeItem* parentItem = getItem(parent);
	bool success;

	beginInsertRows(parent, position, position + rows - 1);
	success = parentItem->insertChildren(position, rows);
	endInsertRows();

	return success;
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

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
	TreeItem *parentItem = getItem(parent);
	bool success = true;

	beginRemoveRows(parent, position, position + rows - 1);
	success = parentItem->removeChildren(position, rows);
	endRemoveRows();

	return success;
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
	TreeItem* parentItem = getItem(parent);

	return parentItem->childCount();
}

bool TreeModel::setInfo(const QModelIndex& index, const QString& value)
{
	TreeItem* item = getItem(index);
	bool result = item->setInfo(value);

	if (result)
		emit dataChanged(index, index);

	return result;
}

bool TreeModel::setData(const QModelIndex& index, const QVariant& value,
						int role)
{
	if (role != Qt::EditRole)
		return false;

	TreeItem* item = getItem(index);
	bool result = item->setData(value);

	if (result)
		emit dataChanged(index, index);

	return result;
}

/*bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
							  const QVariant &value, int role)
{
	if (role != Qt::EditRole || orientation != Qt::Horizontal)
		return false;

	bool result = rootItem->setData(value);

	if (result)
		emit headerDataChanged(orientation, section, section);

	return result;
}*/

QModelIndex TreeModel::match(const QModelIndex& start, const QString& value) const
{
	QModelIndex result;
	QModelIndex p = parent(start);
	int from = start.row();
	int to = rowCount(p);

	for (int r = from; r < to; ++r) {
		QModelIndex idx = index(r, 0, p);
		if (!idx.isValid())
			continue;
		QString s = info(idx);

		if (value.toString() == s) {
			QMessageBox::critical(0, "Debug", "Found!", QMessageBox::Ok);
			result.append(idx);
		}

		if (hasChildren(idx)) { // search the hierarchy
			QMessageBox::critical(0, "Debug", "Check children!", QMessageBox::Ok);
			result += match(index(0, idx.column(), idx), role, value, hits, flags);
		}
	}

	return result;
}
//-- class TreeModel --/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
//-- class TreeItem --//////////////////////////////////////////////////////////////////////////////

TreeItem::TreeItem(const QVariant &data, const QString &header, TreeItem* parent)
{
	parentItem = parent;
	itemData = data;
	itemInfo = header;
}

TreeItem::~TreeItem()
{
	qDeleteAll(childItems);
}

QString TreeItem::info() const
{
	return itemInfo;
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
	return 1;
}

QVariant TreeItem::data() const
{
	return itemData;
}

bool TreeItem::insertChildren(int position, int count)
{
	if (position < 0 || position > childItems.size())
		return false;

	for (int row = 0; row < count; ++row) {
		QVariant data;
		QString info;
		TreeItem* item = new TreeItem(data, info, this);
		childItems.insert(position, item);
	}

	return true;
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

bool TreeItem::setData(const QVariant &value)
{
	//QMessageBox::critical(0, "Debug", "Debugdf", QMessageBox::Ok);

	itemData = value;
	return true;
}
bool TreeItem::setInfo(const QString &value)
{
	itemInfo = value;
	return true;
}
//-- class TreeItem --//////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
