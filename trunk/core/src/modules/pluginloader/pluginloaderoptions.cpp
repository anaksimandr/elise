#include <QtWidgets/QtWidgets>
#include "pluginloaderoptions.h"
#include "../../services.h"
#include "../../../../api/e_options.h"
#include "../../../../api/e_pluginapi.h"
#include "pluginstreemodel.h"
#include "pluginloader.h"

QTreeView*	PluginLoaderOptions::treeView = 0;
QLabel* PluginLoaderOptions::description = 0;
QLabel* PluginLoaderOptions::author = 0;
QLabel* PluginLoaderOptions::authorEmail = 0;
QLabel* PluginLoaderOptions::copyright = 0;
QLabel* PluginLoaderOptions::homepage = 0;

void PluginLoaderOptions::saveLoaderOptions()
{
	//return 1;
}

void PluginLoaderOptions::loadSelectedPluginInfo(const QModelIndex& current, const QModelIndex&)
{
	PluginInfo* pluginInfo;
	PluginsTreeModel* model = dynamic_cast<PluginsTreeModel*>(treeView->model());
	pluginInfo = PluginLoader::getElisePluginInfo(model->data(model->index(current.row(), 1),
															  Qt::DisplayRole).toString());
	description->setText(pluginInfo->description);
	homepage->setText("<a href=\"" + pluginInfo->homepage + "\">" + pluginInfo->homepage + "</a>");
	author->setText(pluginInfo->author);
	authorEmail->setText("<a href=mailto:\"" + pluginInfo->authorEmail + "\">"
						 + pluginInfo->authorEmail + "</a>");
	copyright->setText(pluginInfo->copyright);
}

int PluginLoaderOptions::createLoaderOptionsPage(intptr_t pfnPageAdder, intptr_t)
{
	QWidget* widget = new QWidget();

	widget->setStyleSheet("QTreeView::item {padding-left: 3px;}");
	treeView = new QTreeView(widget);
	treeView->resize(472, 270);
	treeView->move(3, 3);
	PluginsTreeModel* model = new PluginsTreeModel(treeView);
	treeView->setModel(model);
	treeView->setColumnWidth(0, 18);
	treeView->setColumnWidth(1, 168);
	treeView->setColumnWidth(2, 207);
	treeView->setColumnWidth(3, 60);
	treeView->setHeaderHidden(true);
	treeView->setRootIsDecorated(false);
	CheckBoxDelegate* dl = new CheckBoxDelegate(treeView);
	treeView->setItemDelegate(dl);

	QObject::connect(treeView->selectionModel(), &QItemSelectionModel::currentChanged,
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
	box->move(3, 282);
	box->resize(472, 163);

	//-- Description
	QLabel* label = new QLabel(box);
	label->move(10, 10);
	label->resize(95, 20);
	label->setAlignment(Qt::AlignTop);
	label->setText("Description:");
	description = new QLabel(box);
	description->resize(350, 65);
	description->move(110, 10);
	description->setWordWrap(true);
	description->setAlignment(Qt::AlignTop);
	description->setTextInteractionFlags(Qt::TextSelectableByMouse);

	//-- Homepage
	label = new QLabel(box);
	label->move(10, 83);
	label->resize(95, 20);
	label->setAlignment(Qt::AlignTop);
	label->setText("Homepage:");
	homepage = new QLabel(box);
	homepage->move(110, 83);
	homepage->resize(350, 20);
	homepage->setAlignment(Qt::AlignTop);
	homepage->setTextFormat(Qt::RichText);
	homepage->setTextInteractionFlags(Qt::TextBrowserInteraction);
	homepage->setOpenExternalLinks(true);

	//-- Author
	label = new QLabel(box);
	label->move(10, 103);
	label->resize(95, 20);
	label->setAlignment(Qt::AlignTop);
	label->setText("Author:");
	author = new QLabel(box);
	author->move(110, 103);
	author->resize(350, 20);
	author->setAlignment(Qt::AlignTop);
	author->setTextInteractionFlags(Qt::TextSelectableByMouse);

	//-- Author Email
	label = new QLabel(box);
	label->move(10, 123);
	label->resize(95, 20);
	label->setAlignment(Qt::AlignTop);
	label->setText("Author Email:");
	authorEmail = new QLabel(box);
	authorEmail->move(110, 123);
	authorEmail->resize(350, 20);
	authorEmail->setAlignment(Qt::AlignTop);
	authorEmail->setTextFormat(Qt::RichText);
	authorEmail->setTextInteractionFlags(Qt::TextBrowserInteraction);
	authorEmail->setOpenExternalLinks(true);

	//-- Copyright
	label = new QLabel(box);
	label->move(10, 143);
	label->resize(95, 20);
	label->setAlignment(Qt::AlignTop);
	label->setText("Copyright:");
	copyright = new QLabel(box);
	copyright->move(110, 143);
	copyright->resize(350, 20);
	copyright->setAlignment(Qt::AlignTop);
	copyright->setTextInteractionFlags(Qt::TextSelectableByMouse);

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
