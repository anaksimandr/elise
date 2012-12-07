#include <QtWidgets/QtWidgets>
#include "pluginloaderoptions.h"
#include "../../services.h"
#include "../../../../api/e_options.h"
#include "../../../../api/e_pluginapi.h"
#include "pluginstreemodel.h"
#include "pluginloader.h"

QTreeView*	PluginLoaderOptions::treeView_ = 0;
QLabel* PluginLoaderOptions::description_ = 0;
QLabel* PluginLoaderOptions::author_ = 0;
QLabel* PluginLoaderOptions::authorEmail_ = 0;
QLabel* PluginLoaderOptions::copyright_ = 0;
QLabel* PluginLoaderOptions::homepage_ = 0;
QLabel* PluginLoaderOptions::uuid_ = 0;

void PluginLoaderOptions::saveLoaderOptions()
{
	//return 1;
}

void PluginLoaderOptions::loadSelectedPluginInfo(const QModelIndex& current, const QModelIndex&)
{
	PluginInfo* pluginInfo;
	PluginsTreeModel* model = dynamic_cast<PluginsTreeModel*>(treeView_->model());
	pluginInfo = PluginLoader::getElisePluginInfo(model->data(model->index(current.row(), 1),
															  Qt::DisplayRole).toString());
	description_->setText(pluginInfo->description);
	homepage_->setText("<a href=\"" + pluginInfo->homepage + "\">" + pluginInfo->homepage + "</a>");
	author_->setText(pluginInfo->author);
	authorEmail_->setText("<a href=mailto:\"" + pluginInfo->authorEmail + "\">"
						 + pluginInfo->authorEmail + "</a>");
	copyright_->setText(pluginInfo->copyright);
	uuid_->setText(pluginInfo->uuid.toString());
}

int PluginLoaderOptions::createLoaderOptionsPage(intptr_t pfnPageAdder, intptr_t)
{
	QWidget* widget = new QWidget();

	widget->setStyleSheet("QTreeView::item {padding-left: 3px;}");
	treeView_ = new QTreeView(widget);
	treeView_->resize(472, 250);
	treeView_->move(3, 3);
	PluginsTreeModel* model = new PluginsTreeModel(treeView_);
	treeView_->setModel(model);
	treeView_->setColumnWidth(0, 18);
	treeView_->setColumnWidth(1, 168);
	treeView_->setColumnWidth(2, 207);
	treeView_->setColumnWidth(3, 60);
	treeView_->setHeaderHidden(true);
	treeView_->setRootIsDecorated(false);
	CheckBoxDelegate* dl = new CheckBoxDelegate(treeView_);
	treeView_->setItemDelegate(dl);

	QObject::connect(treeView_->selectionModel(), &QItemSelectionModel::currentChanged,
					 &PluginLoaderOptions::loadSelectedPluginInfo);

	PluginInfo* pluginInfo;
	const QMap<QString, Plugin>* plugins = PluginLoader::getAvailablePlugins();
	QMap<QString, Plugin>::const_iterator i = plugins->constBegin();
	QMap<QString, Plugin>::const_iterator iEnd = plugins->constEnd();
	while (i != iEnd) {
		pluginInfo = PluginLoader::getElisePluginInfo(i.key());
		model->insert(i.key(), pluginInfo->name,
					  QString::number(pluginInfo->version[0]) + "."
					+ QString::number(pluginInfo->version[1]) + "."
					+ QString::number(pluginInfo->version[2]) + "."
					+ QString::number(pluginInfo->version[3]));
		delete pluginInfo;
		++i;
	}
	//-- First call of this function. false = insert controls
	model->updateLoadControls(false);

	QGroupBox* box = new QGroupBox(widget);
	box->move(3, 262);
	box->resize(472, 183);

	//-- Description
	QLabel* label = new QLabel(box);
	label->move(10, 10);
	label->resize(95, 20);
	label->setAlignment(Qt::AlignTop);
	label->setText("Description:");
	description_ = new QLabel(box);
	description_->resize(350, 65);
	description_->move(110, 10);
	description_->setWordWrap(true);
	description_->setAlignment(Qt::AlignTop);
	description_->setTextInteractionFlags(Qt::TextSelectableByMouse);

	//-- Homepage
	label = new QLabel(box);
	label->move(10, 83);
	label->resize(95, 20);
	label->setAlignment(Qt::AlignTop);
	label->setText("Homepage:");
	homepage_ = new QLabel(box);
	homepage_->move(110, 83);
	homepage_->resize(350, 20);
	homepage_->setAlignment(Qt::AlignTop);
	homepage_->setTextFormat(Qt::RichText);
	homepage_->setTextInteractionFlags(Qt::TextBrowserInteraction);
	homepage_->setOpenExternalLinks(true);

	//-- Author
	label = new QLabel(box);
	label->move(10, 103);
	label->resize(95, 20);
	label->setAlignment(Qt::AlignTop);
	label->setText("Author:");
	author_ = new QLabel(box);
	author_->move(110, 103);
	author_->resize(350, 20);
	author_->setAlignment(Qt::AlignTop);
	author_->setTextInteractionFlags(Qt::TextSelectableByMouse);

	//-- Author Email
	label = new QLabel(box);
	label->move(10, 123);
	label->resize(95, 20);
	label->setAlignment(Qt::AlignTop);
	label->setText("Author Email:");
	authorEmail_ = new QLabel(box);
	authorEmail_->move(110, 123);
	authorEmail_->resize(350, 20);
	authorEmail_->setAlignment(Qt::AlignTop);
	authorEmail_->setTextFormat(Qt::RichText);
	authorEmail_->setTextInteractionFlags(Qt::TextBrowserInteraction);
	authorEmail_->setOpenExternalLinks(true);

	//-- Copyright
	label = new QLabel(box);
	label->move(10, 143);
	label->resize(95, 20);
	label->setAlignment(Qt::AlignTop);
	label->setText("Copyright:");
	copyright_ = new QLabel(box);
	copyright_->move(110, 143);
	copyright_->resize(350, 20);
	copyright_->setAlignment(Qt::AlignTop);
	copyright_->setTextInteractionFlags(Qt::TextSelectableByMouse);

	//-- Unique plugin ID
	label = new QLabel(box);
	label->move(10, 163);
	label->resize(95, 20);
	label->setAlignment(Qt::AlignTop);
	label->setText("Unique ID:");
	uuid_ = new QLabel(box);
	uuid_->move(110, 163);
	uuid_->resize(350, 20);
	uuid_->setAlignment(Qt::AlignTop);
	uuid_->setTextInteractionFlags(Qt::TextSelectableByMouse);

	widget->setToolTip("Plugins");

	OptionsPage* page = new OptionsPage;

	page->id = QStringLiteral("Plugins");
	page->title = QStringLiteral("Plugins");
	page->savePage = &PluginLoaderOptions::saveLoaderOptions;
	page->page = widget;

	reinterpret_cast<PageAdder>(pfnPageAdder)(page);

	delete page;

	return 0;
}
