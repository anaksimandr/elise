#include "options.h"
#include "treemodel.h"
#include "../../services.h"

const QLatin1String	OPTIONS_SHOW		=	QLatin1String("Options/Show");
const QLatin1String	OPTIONS_SAVE		=	QLatin1String("Options/Save");
const QLatin1String	OPTIONS_CLOSE		=	QLatin1String("Options/Close");

OptionsDialog* OptionsDialog::options = 0;

int ShowOptions(intptr_t, intptr_t)
{
	if (OptionsDialog::options == 0) {
		OptionsDialog::options = new OptionsDialog();
		OptionsDialog::addDefaultPages();
		core::NotifyEventHooks(&OPTIONS_SHOW,
							   reinterpret_cast<intptr_t>(&OptionsDialog::addPage), 0);
	}

	OptionsDialog::options->show();

	return 0;
}

int LoadOptionsModule()
{
	OptionsDialog::options = 0;
	core::CreateHookableEvent(&OPTIONS_SHOW);
	core::CreateServiceFunction(&OPTIONS_SHOW, &ShowOptions);
	return 0;
}

int UnloadOptionsModule()
{
	if (OptionsDialog::options != 0)
		OptionsDialog::options->~OptionsDialog();
	core::DestroyHookableEvent(&OPTIONS_SHOW);
	core::DestroyServiceFunction(&OPTIONS_SHOW);
	return 0;
}

int OptionsDialog::addPage(OptionsPage* newPage)
{
	if (OptionsDialog::options == 0)
		return -1;

	TreeModel* model = dynamic_cast<TreeModel*>(options->treeView->model());
	QModelIndex index = model->index(0, 0);
	QModelIndex parent = model->match(index, newPage->parentId);

	if (newPage->page == 0) {
		//-- Create default widget for blank pages
		newPage->page = new QWidget(options);
		QLabel* label = new QLabel("Please select a subentry from the list", newPage->page);
		label->move(150, 220);
	}
	else
		//-- Set OptionsDialog as parent to delete widget on exit
		newPage->page->setParent(options);

	int layoutIndex = options->layout->addWidget(newPage->page);

	//-- Insert to rootItem if parent not found
	if (!model->insert(newPage->title, newPage->id, layoutIndex, parent)) {
		options->layout->takeAt(layoutIndex);
		return -1;
	}

	//--
	options->saveFunctions.insert(newPage->savePage);

	return 0;
}

void OptionsDialog::selectPage(const QModelIndex& current, const QModelIndex&)
{
	TreeModel* model = dynamic_cast<TreeModel*>(treeView->model());
	layout->setCurrentIndex(model->getLayoutIndex(current));
}

void OptionsDialog::applay()
{
	QSet<OptionsSaver>::const_iterator i = saveFunctions.constBegin();
	while (i != saveFunctions.constEnd()) {
		if (*i != 0)
			(*i)();
		++i;
	}
}

void OptionsDialog::okButton()
{
	applay();
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

	TreeItemDelegate* delegate = new TreeItemDelegate(treeView);
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
	connect(btn, &QPushButton::clicked, this, &OptionsDialog::applay);

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

	//core::CreateServiceFunction(&OPTIONS_ADD_PAGE, &AddPage);
	core::CreateHookableEvent(&OPTIONS_CLOSE);
}

OptionsDialog::~OptionsDialog()
{
	core::NotifyEventHooks(&OPTIONS_CLOSE, 0, 0);
	OptionsDialog::options = 0;
	core::DestroyHookableEvent(&OPTIONS_CLOSE);
	//core::DestroyServiceFunction(&OPTIONS_ADD_PAGE);
}

void OptionsDialog::addDefaultPages()
{
	QString str;
	QWidget* wi;
	OptionsPage* page = new OptionsPage;

	str = "Chat";
	page->id = str;
	page->parentId = "FFFF";
	page->title = str;
	wi = new QWidget();
	wi->setToolTip(str);
	page->page = wi;
	page->savePage = 0;
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
}
