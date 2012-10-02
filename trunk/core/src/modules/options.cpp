
#include "../commonheaders.h"

OptionsDialog* OptionsDialog::options = 0;













OptionsDialog::OptionsDialog()
{
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setFixedSize(700, 500);
	//-- Options tree
	treeView = new QTreeView(this);
	treeView->move(-1, -1);
	treeView->resize(200, this->height() + 2);

	QString header = "11";
	TreeModel* model = new TreeModel(header, this);

	TreeItemDelegate* delegate = new TreeItemDelegate();
	treeView->setModel(model);
	treeView->setItemDelegate(delegate);
	treeView->setHeaderHidden(true);
	//treeView->setRootIsDecorated(true);
	//this->setStyleSheet("QTreeView { show-decoration-selected: 1;}"
		//				"QTreeView::item { border: 5px solid #d9d9d9; }");
	/*this->setStyleSheet(
						"QTreeView::branch:has-siblings:adjoins-item {background: red;}"
				"QTreeView::branch:!has-children:!has-siblings:adjoins-item {background: blue;}"
				"QTreeView::branch:closed:has-children:has-siblings {background: pink;}");*/

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

	btn = new QPushButton("sort selected", widgetClientArea);
	btn->move(50, 240);
	connect(btn, &QPushButton::clicked, this, &OptionsDialog::sort);

	btn = new QPushButton("delete", widgetClientArea);
	btn->move(130, 240);
	connect(btn, &QPushButton::clicked, this, &OptionsDialog::deleteChild);

	QPalette p = widgetClientArea->palette();
	p.setColor(QPalette::Window, Qt::red);
	p.setColor(QPalette::WindowText, Qt::red);
	//p.setColor(QPalette::Base, Qt::red);
	//p.setColor(QPalette::AlternateBase, Qt::red);
	p.setColor(QPalette::Text, Qt::red);
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
	//QMessageBox::critical(0, "Debug", "Destroy", QMessageBox::Ok);
}

bool OptionsDialog::addChild()
{
	QModelIndex index = treeView->selectionModel()->currentIndex();
	TreeModel* model = dynamic_cast<TreeModel*>(treeView->model());

	if (!model->insert(index, edit->text()))
		return false;

	//QModelIndex child = model->index(0, 1, index);

	return true;
}

bool OptionsDialog::deleteChild()
{
	QModelIndex index = treeView->selectionModel()->currentIndex();
	TreeModel* model = dynamic_cast<TreeModel*>(treeView->model());

	if (!model->remove(index))
		return false;

	//QModelIndex child = model->index(0, 1, index);

	return true;
}

bool OptionsDialog::findItem()
{
	TreeModel* model = dynamic_cast<TreeModel*>(treeView->model());
	QModelIndex index = model->index(0, 0);
	QModelIndex result = model->match(index, edit->text());
	if (result.isValid()) {
		//QMessageBox::critical(0, "Debug", model->info(result), QMessageBox::Ok);
		treeView->setCurrentIndex(result);
	}
	else
		return false;

	return true;
}

bool OptionsDialog::sort()
{
	QModelIndex index = treeView->selectionModel()->currentIndex();
	TreeModel* model = dynamic_cast<TreeModel*>(treeView->model());
	model->sortChildren(index);

	return true;
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
//-- class TreeItemDelegate --//////////////////////////////////////////////////////////////////////

QSize TreeItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	return QSize(20,20);
}

void TreeItemDelegate::paint(QPainter* painter,
							 const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	QStyleOptionViewItem opt = option;
	if ((index.isValid()) && (index.column() == 0)) {
		//opt.displayAlignment = Qt::AlignCenter;
		opt.backgroundBrush = Qt::red;
	}

	QStyledItemDelegate::paint(painter, opt, index);
}

//-- class TreeItemDelegate --//////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
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

int TreeModel::rowCount(const QModelIndex& parentIndex) const
{
	TreeItem* parent = getItem(parentIndex);

	return parent->childCount();
}

int TreeModel::columnCount(const QModelIndex & /* parentIndex */) const
{
	return 1;
}

QVariant TreeModel::data(const QModelIndex& itemIndex, int role) const
{
	if (!itemIndex.isValid())
		return QVariant();

	if (role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	TreeItem* item = getItem(itemIndex);

	return item->getHeader();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex& itemIndex) const
{
	if (!itemIndex.isValid())
		return 0;

	//return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

TreeItem* TreeModel::getItem(const QModelIndex& itemIndex) const
{
	if (itemIndex.isValid()) {
		TreeItem* item = static_cast<TreeItem*>(itemIndex.internalPointer());
		if (item)
			return item;
	}
	return rootItem;
}

QModelIndex TreeModel::match(const QModelIndex& startIndex, const QString& header) const
{
	QModelIndex resultIndex;
	QModelIndex p = parent(startIndex);
	int from = startIndex.row();
	int to = rowCount(p);

	for (int r = from; r < to; ++r) {
		resultIndex = index(r, 0, p);

		if (!resultIndex.isValid())
			continue;

		QString s = data(resultIndex, Qt::DisplayRole).toString();

		if (header == s) {
			//QMessageBox::critical(0, "Debug", "Found!", QMessageBox::Ok);
			return resultIndex;
		}

		if (hasChildren(resultIndex)) { // search the hierarchy
			//QMessageBox::critical(0, "Debug", "Check children!", QMessageBox::Ok);
			resultIndex = match(index(0, resultIndex.column(), resultIndex), header);
			if (resultIndex.isValid())
				return resultIndex;
		}
	}

	return QModelIndex();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex& parentIndex) const
{
	if (parentIndex.isValid() && parentIndex.column() != 0)
		return QModelIndex();

	TreeItem* parent = getItem(parentIndex);

	TreeItem* child = parent->child(row);
	if (child)
		return createIndex(row, column, child);
	else
		return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex& itemIndex) const
{
	if (!itemIndex.isValid())
		return QModelIndex();

	TreeItem* child = getItem(itemIndex);
	TreeItem* parent = child->parent();

	if (parent == rootItem)
		return QModelIndex();

	return createIndex(parent->childNumber(), 0, parent);
}

bool TreeModel::insert(const QModelIndex& parentIndex, QString& header)
{	
	if (header.isEmpty())
		return false;

	TreeItem* parent = getItem(parentIndex);
	bool success;

	beginInsertRows(parentIndex, 0, 0);
	success = parent->insertChild(header);
	endInsertRows();
	//sortChildren(parentIndex);

	return success;
}

bool TreeModel::remove(const QModelIndex& itemIndex)
{
	if (!itemIndex.isValid())
		return false;

	TreeItem* item = getItem(itemIndex);
	TreeItem* parent = item->parent();

	QModelIndex parentIndex = this->parent(itemIndex);

	bool success;
	int pos = item->childNumber();

	beginRemoveRows(parentIndex, pos, pos);
	success = parent->removeChild(pos);
	endRemoveRows();

	return success;
}

bool TreeModel::setData(const QModelIndex& itemIndex, const QVariant& value, int role)
{
	if (role != Qt::EditRole)
		return false;

	TreeItem* item = getItem(itemIndex);
	item->setHeader(value.toString());

	emit dataChanged(itemIndex, itemIndex);

	return true;
}

void TreeModel::sortChildren(const QModelIndex& parentIndex)
{
	if (!parentIndex.isValid())
		return ;

	TreeItem* item = getItem(parentIndex);
	item->sortChildren();
}

//-- class TreeModel --/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
//-- class TreeItem --//////////////////////////////////////////////////////////////////////////////

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
	return s1 < s2;
}

void TreeItem::sortChildren()
{
	qSort(childItems.begin(), childItems.end(), lessThan);
}

//-- class TreeItem --//////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
