#include "pluginloaderoptions.h"
#include "../../services.h"
#include "../../../../api/e_options.h"
#include "checkboxdelegate.h"


void PluginLoaderOptions::saveLoaderOptions()
{
	//return 1;
}

int PluginLoaderOptions::createLoaderOptionsPage(intptr_t pfnPageAdder, intptr_t)
{
	QWidget* widget = new QWidget();


	/*QStandardItemModel* model = new QStandardItemModel(4, 2);
	QTableView* tableView = new QTableView(widget);
	tableView->setModel(model);

	CheckBoxDelegate* delegate = new CheckBoxDelegate();
	tableView->setItemDelegate(delegate);

	tableView->horizontalHeader()->setStretchLastSection(true);

	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 2; ++column) {
			QModelIndex index = model->index(row, column, QModelIndex());
			model->setData(index, QVariant((row+1) * (column+1)));
		}
	}*/

	QTableWidget* tableWidget =  new QTableWidget(4, 2, widget);
	QAbstractItemModel* model = tableWidget->model();
	//tableWidget->setItemDelegate(new CheckBoxDelegate(tableWidget));
	tableWidget->verticalHeader()->hide();
	tableWidget->verticalHeader()->setDefaultSectionSize(20);
	tableWidget->horizontalHeader()->hide();
	//tableWidget->setGridStyle();
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 2; ++column) {
			QModelIndex index = model->index(row, column, QModelIndex());
			model->setData(index, QVariant("(dfgfkdj hgd jfglkdj gdfg lkjfdlk ghfdlkghj d"));
		}
	}
	for (int row =0; row < 4; ++row) {
		QTableWidgetItem* item = tableWidget->item(row, 0);
		item->setCheckState(Qt::Unchecked);
	}

	widget->setToolTip("Plugins");

	OptionsPage* page = new OptionsPage;

	page->id = QStringLiteral("Plugins");
	page->title = QStringLiteral("Plugins");
	page->savePage = &PluginLoaderOptions::saveLoaderOptions;
	page->page = widget;

	reinterpret_cast<PageAdder>(pfnPageAdder)(page);

	return 0;
}
