
#include "../../commonheaders.h"

OptionsDialog* OptionsDialog::options = 0;

int AddPage(uintptr_t wParam,uintptr_t lParam)
{

	return 0;
}



int LoadOptionsModule()
{
	OptionsDialog::options = 0;
	CreateHookableEvent(&OPTIONS_SHOW);
	CreateServiceFunction(&OPTIONS_SHOW, (ELISESERVICE)ShowOptions);
	CreateServiceFunction(&OPTIONS_ADD_PAGE, (ELISESERVICE)AddPage);
	return 0;
}

int UnloadOptionsModule()
{
	if (OptionsDialog::options == 0)
		OptionsDialog::options->~OptionsDialog();
	return 0;
}

int ShowOptions(uintptr_t wParam,uintptr_t lParam)
{
	if (OptionsDialog::options == 0) {
		OptionsDialog::options = new OptionsDialog();
		NotifyEventHooks(&OPTIONS_SHOW, 0, 0);
	}
	OptionsDialog::options->show();
	return 0;
}

bool OptionsDialog::addPage(QWidget* page)
{
	return true;
}

OptionsDialog::OptionsDialog()
{
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setFixedSize(700, 500);

	//-- Options tree
	treeView = new QTreeView(this);
	treeView->move(-1, -1);
	treeView->resize(200, this->height() + 2);

	QString str;
	TreeModel* model = new TreeModel(str, this);

	TreeItemDelegate* delegate = new TreeItemDelegate();
	treeView->setModel(model);
	treeView->setItemDelegate(delegate);
	treeView->setHeaderHidden(true);

	//-- Client area for current settings
	widgetArea = new QGroupBox(this);
	widgetArea->move(treeView->width() + 10, 10);
	widgetArea->resize(this->width() - treeView->width() - 10 - 10,
							 this->height() - 10 - 40);
	QStackedLayout* layout = new QStackedLayout();
	widgetArea->setLayout(layout);

	/*
	//treeView->setRootIsDecorated(true);
	//this->setStyleSheet("QTreeView::item {border-bottom: 1px solid #bfcde4;}"
		//				"QTreeView::item::selected {border-bottom: 1px solid #bfcde4;}");
	this->setStyleSheet(
						"QTreeView::branch:has-siblings:adjoins-item {background: red;}"
				"QTreeView::branch:!has-children:!has-siblings:adjoins-item {background: blue;}"
				"QTreeView::branch:closed:has-children:has-siblings {background: pink;}");

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
	//vv->resize(vv->parentWidget()->size());*/

	QModelIndex index = treeView->selectionModel()->currentIndex();
	str = "Chat";
	model->insert(index, str);
	str = "Popup windows";
	model->insert(index, str);
	str = "History";
	model->insert(index, str);
	str = "Moduls";
	model->insert(index, str);
	str = "Network";
	model->insert(index, str);
	str = "Services";
	model->insert(index, str);
	str = "Ivents";
	model->insert(index, str);
	str = "Contact list";
	model->insert(index, str);
	str = "Status";
	model->insert(index, str);
	str = "Config";
	model->insert(index, str);
}

OptionsDialog::~OptionsDialog()
{
	OptionsDialog::options = 0;
}

bool OptionsDialog::sort()
{
	TreeModel* model = dynamic_cast<TreeModel*>(treeView->model());
	QModelIndex index = model->index(0, 0);

	model->sortChildren(index);

	return true;
}



bool OptionsDialog::addChild()
{
	QModelIndex index = treeView->selectionModel()->currentIndex();
	TreeModel* model = dynamic_cast<TreeModel*>(treeView->model());

	if (!model->insert(index, edit->text()))
		return false;

	return true;
}

bool OptionsDialog::deleteChild()
{
	QModelIndex index = treeView->selectionModel()->currentIndex();
	TreeModel* model = dynamic_cast<TreeModel*>(treeView->model());

	if (model->remove(index))
		return true;

	return false;
}

bool OptionsDialog::findItem()
{
	TreeModel* model = dynamic_cast<TreeModel*>(treeView->model());
	QModelIndex index = model->index(0, 0);
	QModelIndex result = model->match(index, edit->text());

	if (result.isValid()) {
		treeView->setCurrentIndex(result);
		return true;
	}

	return false;
}
