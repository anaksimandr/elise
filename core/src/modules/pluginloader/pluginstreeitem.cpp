#include <QtWidgets/QtWidgets>
#include "pluginstreeitem.h"

PluginsTreeItem::PluginsTreeItem(bool pluginLoadedExt, const QString& pluginNameExt,
								 const QString& pluginVersionExt, const QString pluginUuidExt,
								 PluginsTreeItem* parentExt)
{
	this->pluginLoaded = pluginLoadedExt;
	this->pluginName = pluginNameExt;
	this->pluginVersion = pluginVersionExt;
	this->pluginUuid = pluginUuidExt;
	this->parentItem = parentExt;
}

bool PluginsTreeItem::insertChild(bool pluginLoadedExt, const QString& pluginNameExt,
								  const QString& pluginVersionExt, const QString pluginUuidExt)
{
	PluginsTreeItem* item = new PluginsTreeItem(pluginLoadedExt, pluginNameExt, pluginVersionExt,
												pluginUuidExt, this);

	if (childItems.count() == 0)
		childItems.insert(0, item);
	else {
		QList<PluginsTreeItem*>::iterator i;
		for (i = childItems.begin(); i != childItems.end(); ++i) {
			if ((*i)->getPluginName().toUpper() > pluginNameExt.toUpper()) {
				childItems.insert(i, item);
				return true;
			}
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
