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
}

int PluginLoaderOptions::createLoaderOptionsPage(intptr_t pfnPageAdder, intptr_t)
{
	QWidget* widget = new QWidget();

	treeView = new QTreeView(widget);
	treeView->resize(474, 200);
	treeView->move(2, 2);
	PluginsTreeModel* model = new PluginsTreeModel(treeView);
	treeView->setModel(model);
	treeView->setColumnWidth(0, 15);
	treeView->setColumnWidth(1, 185);
	treeView->setColumnWidth(2, 212);
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

	description = new QLabel(widget);
	description->move(10, 222);
	description->setText("sd");

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
