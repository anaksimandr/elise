
#include "../../commonheaders.h"

OptionsDialog* OptionsDialog::options = 0;

int AddPage(uintptr_t wParam, uintptr_t lParam)
{
	if (OptionsDialog::options == 0)
		return -1;

	OPTIONSPAGE* page = (OPTIONSPAGE*)wParam;

	return OptionsDialog::options->addPage(page);
}

int ShowOptions(uintptr_t wParam, uintptr_t lParam)
{
	if (OptionsDialog::options == 0) {
		OptionsDialog::options = new OptionsDialog();
		NotifyEventHooks(&OPTIONS_SHOW, 0, 0);
	}

	OptionsDialog::options->show();

	return 0;
}

int LoadOptionsModule()
{
	OptionsDialog::options = 0;
	CreateHookableEvent(&OPTIONS_SHOW);
	CreateServiceFunction(&OPTIONS_SHOW, (ELISESERVICE)ShowOptions);
	return 0;
}

int UnloadOptionsModule()
{
	if (OptionsDialog::options != 0)
		OptionsDialog::options->~OptionsDialog();
	DestroyHookableEvent(&OPTIONS_SHOW);
	DestroyServiceFunction(&OPTIONS_SHOW);
	return 0;
}

int OptionsDialog::addPage(OPTIONSPAGE* newPage)
{
	TreeModel* model = dynamic_cast<TreeModel*>(treeView->model());
	QModelIndex index = model->index(0, 0);
	QModelIndex parent = model->match(index, newPage->parentId);

	if (newPage->page == 0) {
		//-- Create default widget for blank pages
		newPage->page = new QWidget(this);
		QLabel* label = new QLabel("Please select a subentry from the list", newPage->page);
		label->move(150, 220);
	}
	else
		//-- Set OptionsDialog as parent to delete widget on exit
		newPage->page->setParent(this);

	int layoutIndex = layout->addWidget(newPage->page);

	//-- Insert to rootItem if parent not found
	if (!model->insert(parent, newPage->title, newPage->id, layoutIndex)) {
		layout->takeAt(layoutIndex);
		return -1;
	}

	return 0;
}

void OptionsDialog::selectPage(const QModelIndex& current, const QModelIndex& previous)
{
	TreeModel* model = dynamic_cast<TreeModel*>(treeView->model());
	layout->setCurrentIndex(model->getLayoutIndex(current));
}

void OptionsDialog::applayButton()
{
	NotifyEventHooks(&OPTIONS_SAVE, 0, 0);
}

void OptionsDialog::okButton()
{
	NotifyEventHooks(&OPTIONS_SAVE, 0, 0);
	this->close();
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
	TreeModel* model = new TreeModel(str, str, 0, this);

	TreeItemDelegate* delegate = new TreeItemDelegate();
	treeView->setModel(model);
	treeView->setItemDelegate(delegate);
	treeView->setHeaderHidden(true);

	//-- Client area for current settings
	QGroupBox* widgetArea = new QGroupBox(this);
	widgetArea->move(treeView->width() + 10, 10);
	widgetArea->resize(this->width() - treeView->width() - 10 - 10,
							 this->height() - 10 - 40);
	layout = new QStackedLayout(this);
	//layout->setAlignment(Qt::AlignCenter);
	widgetArea->setLayout(layout);

	connect(treeView->selectionModel(), &QItemSelectionModel::currentChanged,
			this, &OptionsDialog::selectPage);

	QPushButton* btn = new QPushButton(QStringLiteral("Ok"), this);
	btn->setFixedSize(70, 25);
	btn->move(470, 467);
	connect(btn, &QPushButton::clicked, this, &OptionsDialog::okButton);

	btn = new QPushButton(QStringLiteral("Cancel"), this);
	btn->setFixedSize(70, 25);
	btn->move(545, 467);
	connect(btn, &QPushButton::clicked, this, &OptionsDialog::close);

	btn = new QPushButton(QStringLiteral("Applay"), this);
	btn->setFixedSize(70, 25);
	btn->move(620, 467);
	connect(btn, &QPushButton::clicked, this, &OptionsDialog::applayButton);

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

	QWidget* wi;
	OPTIONSPAGE* page = new OPTIONSPAGE;

	str = "Chat";
	page->id = str;
	page->parentId = "FFFF";
	page->title = str;
	wi = new QWidget();
	wi->setToolTip(str);
	page->page = wi;
	addPage(page);

	str = "Popup windows";
	page->id = str;
	page->parentId = "FFFF";
	page->title = str;
	wi = new QWidget();
	wi->setToolTip(str);
	page->page = wi;
	addPage(page);

	str = "History";
	page->id = str;
	page->parentId = "FFFF";
	page->title = str;
	wi = new QWidget();
	wi->setToolTip(str);
	page->page = wi;
	addPage(page);

	str = "Moduls";
	page->id = str;
	page->parentId = "FFFF";
	page->title = str;
	wi = new QWidget();
	wi->setToolTip(str);
	page->page = wi;
	addPage(page);

	str = "Network";
	page->id = str;
	page->parentId = "FFFF";
	page->title = str;
	wi = new QWidget();
	wi->setToolTip(str);
	page->page = wi;
	addPage(page);

	str = "Services";
	page->id = str;
	page->parentId = "FFFF";
	page->title = str;
	page->page = 0;
	addPage(page);

	str = "Ivents";
	page->id = str;
	page->parentId = "FFFF";
	page->title = str;
	wi = new QWidget();
	wi->setToolTip(str);
	page->page = wi;
	addPage(page);

	str = "Contact list";
	page->id = str;
	page->parentId = "FFFF";
	page->title = str;
	wi = new QWidget();
	wi->setToolTip(str);
	page->page = wi;
	addPage(page);

	str = "Status";
	page->id = str;
	page->parentId = "FFFF";
	page->title = str;
	wi = new QWidget();
	wi->setToolTip(str);
	page->page = wi;
	addPage(page);

	str = "Config";
	page->id = str;
	page->parentId = "FFFF";
	page->title = str;
	wi = new QWidget();
	wi->setToolTip(str);
	page->page = wi;
	addPage(page);

	CreateServiceFunction(&OPTIONS_ADD_PAGE, (ELISESERVICE)AddPage);
	CreateHookableEvent(&OPTIONS_SAVE);
	CreateHookableEvent(&OPTIONS_CLOSE);
}

OptionsDialog::~OptionsDialog()
{
	NotifyEventHooks(&OPTIONS_CLOSE, 0, 0);
	OptionsDialog::options = 0;
	DestroyHookableEvent(&OPTIONS_SAVE);
	DestroyHookableEvent(&OPTIONS_CLOSE);
	DestroyServiceFunction(&OPTIONS_ADD_PAGE);
}
