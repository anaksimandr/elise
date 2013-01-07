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

#include "options.h"
#include "treemodel.h"
#include "../../core.h"

const QLatin1String	kOptionsShow_service	=	QLatin1String(__Options_Show_service);
const QLatin1String	kOptionsSave_event		=	QLatin1String(__Options_Save_event);
const QLatin1String	kOptionsClose_event		=	QLatin1String(__Options_Close_event);

OptionsDialog* OptionsDialog::options_ = 0;

int OptionsDialog::showOptions(intptr_t, intptr_t)
{
	if (OptionsDialog::options_ == 0) {
		OptionsDialog::options_ = new OptionsDialog();
		//OptionsDialog::addDefaultPages();
		core->notifyEventHooks(&kOptionsShow_event,
							   reinterpret_cast<intptr_t>(&OptionsDialog::addPage), 0);
	}

	OptionsDialog::options_->show();

	return 0;
}

int OptionsDialog::loadOptionsModule()
{
	OptionsDialog::options_ = 0;
	core->createHookableEvent(&kOptionsShow_event);
	core->createServiceFunction(&kOptionsShow_service, &showOptions);
	return 0;
}

int OptionsDialog::unloadOptionsModule()
{
	if (OptionsDialog::options_ != 0)
		OptionsDialog::options_->~OptionsDialog();
	core->destroyHookableEvent(&kOptionsShow_event);
	core->destroyServiceFunction(&kOptionsShow_service);
	return 0;
}

int OptionsDialog::addPage(OptionsPage* newPage)
{
	if (OptionsDialog::options_ == 0)
		return -1;

	TreeModel* model = dynamic_cast<TreeModel*>(options_->treeView_->model());
	QModelIndex index = model->index(0, 0);
	QModelIndex parent = model->match(index, newPage->parentId);

	if (newPage->page == 0) {
		//-- Create default widget for blank pages
		newPage->page = new QWidget(options_);
		QLabel* label = new QLabel("Please select a subentry from the list", newPage->page);
		label->move(150, 220);
	}
	else
		//-- Set OptionsDialog as parent to delete widget on exit
		newPage->page->setParent(options_);

	int layoutIndex = options_->layout_->addWidget(newPage->page);

	//-- Insert to rootItem if parent not found
	if (!model->insert(newPage->title, newPage->id, layoutIndex, parent)) {
		options_->layout_->takeAt(layoutIndex);
		return -1;
	}

	//--
	if (newPage->savePage != 0)
		options_->saveFunctions_.insert(newPage->savePage);

	return 0;
}

void OptionsDialog::selectPage(const QModelIndex& current, const QModelIndex&)
{
	TreeModel* model = dynamic_cast<TreeModel*>(treeView_->model());
	layout_->setCurrentIndex(model->getLayoutIndex(current));
}

void OptionsDialog::applay()
{
	QSet<OptionsSaver>::const_iterator i = saveFunctions_.constBegin();
	QSet<OptionsSaver>::const_iterator iEnd = saveFunctions_.constEnd();
	while (i != iEnd) {
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
	this->setWindowTitle("Options");
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setFixedSize(700, 500);

	//-- Options tree
	treeView_ = new QTreeView(this);
	treeView_->move(-1, -1);
	treeView_->resize(200, this->height() + 2);

	QString str;
	TreeModel* model = new TreeModel(str, str, 0, this);

	TreeItemDelegate* delegate = new TreeItemDelegate(treeView_);
	treeView_->setModel(model);
	treeView_->setItemDelegate(delegate);
	treeView_->setHeaderHidden(true);

	//-- Client area for current settings
	QGroupBox* widgetArea = new QGroupBox(this);
	widgetArea->move(treeView_->width() + 10, 10);
	widgetArea->resize(this->width() - treeView_->width() - 10 - 10,
							 this->height() - 10 - 40);
	layout_ = new QStackedLayout(this);
	//layout->setAlignment(Qt::AlignCenter);
	widgetArea->setLayout(layout_);

	connect(treeView_->selectionModel(), &QItemSelectionModel::currentChanged,
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

	//core::CreateServiceFunction(&OPTIONS_ADD_PAGE, &AddPage);
	core->createHookableEvent(&kOptionsClose_event);
}

OptionsDialog::~OptionsDialog()
{
	saveFunctions_.clear();
	core->notifyEventHooks(&kOptionsClose_event, 0, 0);
	OptionsDialog::options_ = 0;
	core->destroyHookableEvent(&kOptionsClose_event);
	//core::DestroyServiceFunction(&OPTIONS_ADD_PAGE);
}

/*void OptionsDialog::addDefaultPages()
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

	delete page;
}*/
