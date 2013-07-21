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
#include "pluginstreeitem.h"
#include "pluginstreemodel.h"

PluginsTreeItem::PluginsTreeItem(const QString& pluginModuleNameExt, const QString& pluginNameExt,
								 const QString& pluginVersionExt, PluginsTreeModel* modelExt,
								 PluginsTreeItem* parentExt)
{
	pluginModuleName_ = pluginModuleNameExt;
	pluginName_ = pluginNameExt;
	pluginVersion_ = pluginVersionExt;
	parentItem_ = parentExt;
	model_ = modelExt;
	if (pluginModuleName_.isEmpty())
		return;
	QCheckBox* cb = new QCheckBox();
	loadCheckBox_ = cb;
	if (isPluginLoaded())
		cb->setChecked(true);
	cb->setEnabled(isControlActive());
	connect(cb, &QCheckBox::toggled, this, &PluginsTreeItem::dataChange);
	//loadControl->setAutoFillBackground(true);
}

void PluginsTreeItem::dataChange(bool checked)
{
	if (PluginLoader::updatePluginState(pluginModuleName_, !checked))
		return;

	if (checked)
		PluginLoader::loadPlugin(pluginModuleName_);
	else
		PluginLoader::unloadPlugin(pluginModuleName_);

	//-- Param "true" means update of controls, not create
	model_->updateLoadControls(true);
}

bool PluginsTreeItem::insertChild(const QString& pluginModuleNameExt, const QString& pluginNameExt,
								  const QString& pluginVersionExt, PluginsTreeModel* modelExt)
{
	PluginsTreeItem* item = new PluginsTreeItem(pluginModuleNameExt, pluginNameExt,
												pluginVersionExt, modelExt, this);

	if (childItems_.count() == 0)
		childItems_.insert(0, item);
	else {
		QList<PluginsTreeItem*>::iterator i = childItems_.begin();
		QList<PluginsTreeItem*>::iterator iEnd = childItems_.end();
		while (i != iEnd) {
			if ((*i)->getPluginModuleName().toUpper() > pluginModuleNameExt.toUpper()) {
				childItems_.insert(i, item);
				break;
			}
			++i;
		}
		childItems_.insert(i, item);
	}

	return true;
}

bool PluginsTreeItem::removeChild(int position)
{
	if (position < 0 || position > childItems_.size())
		return false;

	delete childItems_.takeAt(position);

	return true;
}
