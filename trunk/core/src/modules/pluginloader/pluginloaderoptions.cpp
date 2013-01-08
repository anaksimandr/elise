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

#include <QtWidgets/QtWidgets>
#include "pluginloaderoptions.h"
#include "../../core.h"
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
QLabel*	PluginLoaderOptions::uuid_ = 0;
QString	PluginLoaderOptions::pluginName_;

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
	if (!pluginInfo)
		return;
	description_->setText(pluginInfo->description);
	homepage_->setText("<a href=\"" + pluginInfo->homepage + "\">" + pluginInfo->homepage + "</a>");
	author_->setText(pluginInfo->author);
	authorEmail_->setText("<a href=mailto:\"" + pluginInfo->authorEmail + "\">"
						 + pluginInfo->authorEmail + "</a>");
	copyright_->setText(pluginInfo->copyright);
	uuid_->setText(pluginInfo->uuid.toString());
	pluginName_ = pluginInfo->name;
	delete pluginInfo;
}

void PluginLoaderOptions::showPluginInterfaces()
{
	QUuid uuid = QUuid(uuid_->text());
	QSet<QUuid>* pluginInterfaces = reinterpret_cast<QSet<QUuid>*>(
				core->callService(&kCoreGetPluginInterfaces, reinterpret_cast<intptr_t>(&uuid), 0)
				);

	if (!pluginInterfaces) {
		QMessageBox::information(0,
								 "Information", "There is no interfaces or plugin is not loaded.",
								 QMessageBox::Ok);
		return;
	}

	QListWidget* list = new QListWidget();

	QSet<QUuid>::const_iterator i = pluginInterfaces->constBegin();
	QSet<QUuid>::const_iterator iEnd = pluginInterfaces->constEnd();
	while (i != iEnd) {
		list->addItem((*i).toString());
		++i;
	}

	list->setAttribute(Qt::WA_DeleteOnClose);
	list->setSortingEnabled(true);
	list->setWindowTitle(pluginName_ + " interfaces");
	//-- Hmmm... contentsSize() is protected Oo
	list->resize(list->sizeHintForColumn(0) + 4, list->sizeHintForRow(0) * list->count() + 4);
	list->show();
}

int PluginLoaderOptions::createLoaderOptionsPage(intptr_t pfnPageAdder, intptr_t)
{
	QWidget* widget = new QWidget();

	widget->setStyleSheet("QTreeView::item {padding-left: 3px;}");
	treeView_ = new QTreeView(widget);
	treeView_->resize(472, 250);
	treeView_->move(3, 3);
	PluginsTreeModel* model = new PluginsTreeModel(treeView_, widget);
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
		if (!pluginInfo)
			continue;
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

	//-- Button for list plugin interfaces
	QPushButton* btn = new QPushButton(box);
	btn->resize(129, 22);
	btn->move(340, 158);
	btn->setText("Show interfaces");
	QObject::connect(btn, &QPushButton::clicked, &PluginLoaderOptions::showPluginInterfaces);

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
