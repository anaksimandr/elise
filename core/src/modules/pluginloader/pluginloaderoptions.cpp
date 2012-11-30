#include <QtWidgets/QtWidgets>
#include "pluginloaderoptions.h"
#include "../../services.h"
#include "../../../../api/e_options.h"
#include "../../../../api/e_pluginapi.h"
#include "pluginstreemodel.h"
#include "pluginloader.h"


void PluginLoaderOptions::saveLoaderOptions()
{
	//return 1;
}

int PluginLoaderOptions::createLoaderOptionsPage(intptr_t pfnPageAdder, intptr_t)
{
	QWidget* widget = new QWidget();


	QTreeView* treeView = new QTreeView(widget);
	PluginsTreeModel* model = new PluginsTreeModel(treeView);
	treeView->setModel(model);
	treeView->setHeaderHidden(true);
	treeView->setRootIsDecorated(false);
	treeView->setItemDelegate(new CheckBoxDelegate(treeView));

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
