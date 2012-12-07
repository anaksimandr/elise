#include <QtWidgets/QtWidgets>
#include "pluginstreeitem.h"
#include "pluginstreemodel.h"

PluginsTreeItem::PluginsTreeItem(const QString& pluginModuleNameExt, const QString& pluginNameExt,
								 const QString& pluginVersionExt, PluginsTreeModel* modelExt,
								 PluginsTreeItem* parentExt)
{
	pluginModuleName = pluginModuleNameExt;
	pluginName = pluginNameExt;
	pluginVersion = pluginVersionExt;
	parentItem = parentExt;
	model = modelExt;
	if (pluginModuleName.isEmpty())
		return;
	QCheckBox* cb = new QCheckBox();
	loadControl = cb;
	if (isPluginLoaded())
		cb->setChecked(true);
	cb->setEnabled(isControlActive());
	connect(cb, &QCheckBox::toggled, this, &PluginsTreeItem::dataChange);
	//loadControl->setAutoFillBackground(true);
}

void PluginsTreeItem::dataChange(bool checked)
{
	PluginLoader::savePluginStateOrDelete(pluginModuleName, !checked);

	if (checked)
		PluginLoader::loadPlugin(pluginModuleName);
	else
		PluginLoader::unloadPlugin(pluginModuleName);

	//-- Param "true" means update of controls, not create
	model->updateLoadControls(true);
}

bool PluginsTreeItem::insertChild(const QString& pluginModuleNameExt, const QString& pluginNameExt,
								  const QString& pluginVersionExt, PluginsTreeModel* modelExt)
{
	PluginsTreeItem* item = new PluginsTreeItem(pluginModuleNameExt, pluginNameExt,
												pluginVersionExt, modelExt, this);

	if (childItems.count() == 0)
		childItems.insert(0, item);
	else {
		QList<PluginsTreeItem*>::iterator i = childItems.begin();
		QList<PluginsTreeItem*>::iterator iEnd = childItems.end();
		while (i != iEnd) {
			if ((*i)->getPluginModuleName().toUpper() > pluginModuleNameExt.toUpper()) {
				childItems.insert(i, item);
				break;
			}
			++i;
		}
		childItems.insert(i, item);
	}

	return true;
}

bool PluginsTreeItem::removeChild(int position)
{
	if (position < 0 || position > childItems.size())
		return false;

	delete childItems.takeAt(position);

	return true;
}


/*PluginsTreeItem::~PluginsTreeItem()
{
	qDeleteAll(childItems);
}

PluginsTreeItem* PluginsTreeItem::parent()
{
	return parentItem;
}

PluginsTreeItem* PluginsTreeItem::child(int number)
{
	return childItems.value(number);
}

int PluginsTreeItem::childCount() const
{
	return childItems.count();
}

int PluginsTreeItem::childNumber() const
{
	if (parentItem)
		return parentItem->childItems.indexOf(const_cast<PluginsTreeItem*>(this));

	return 0;
}*/

/*QVariant PluginsTreeItem::getData(int column) const
{
	QVariant result;
	switch (column) {
		case 0:
			result = pluginName;
			break;
		case 1:
			result = pluginVersion;
			break;
#ifndef NDEBUG
		default:
			QMessageBox::critical(0, "Error", "Wrong item index!", QMessageBox::Cancel);
			break;
#endif
	}

	return result;
}*/

/*QString PluginsTreeItem::getPluginName() const
{
	return pluginName;
}

QString PluginsTreeItem::getPluginVersion() const
{
	return pluginVersion;
}

QString PluginsTreeItem::getPluginUuid() const
{
	return pluginUuid;
}

bool PluginsTreeItem::isPluginLoaded() const
{
	return pluginLoaded;
}

void PluginsTreeItem::setPluginLoaded(bool pluginLoadedExt)
{
	pluginLoaded = pluginLoadedExt;
}*/
