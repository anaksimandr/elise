#include <QtWidgets/QtWidgets>
#include "pluginloaderoptions.h"
#include "../../services.h"
#include "../../../../api/e_options.h"
#include "pluginstreemodel.h"


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


	//QString str = "test";
	//model->insert(0, str, str, str);


	widget->setToolTip("Plugins");

	OptionsPage* page = new OptionsPage;

	page->id = QStringLiteral("Plugins");
	page->title = QStringLiteral("Plugins");
	page->savePage = &PluginLoaderOptions::saveLoaderOptions;
	page->page = widget;

	reinterpret_cast<PageAdder>(pfnPageAdder)(page);

	return 0;
}
