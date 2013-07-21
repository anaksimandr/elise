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
#ifndef ELISE_CORE_MODULES_PLUGINLOADER_PLUGINSTABLEITEM_H_
#define ELISE_CORE_MODULES_PLUGINLOADER_PLUGINSTABLEITEM_H_

#include "pluginloader.h"
#include <QObject>

class QString;
template <typename T> class QList;
class QVariant;
class PluginsTreeModel;

class PluginsTreeItem : public QObject
{
	Q_OBJECT
private:
	QString					pluginModuleName_;
	QString					pluginName_;
	QString					pluginVersion_;
	QList<PluginsTreeItem*>	childItems_;
	PluginsTreeItem*		parentItem_;
	PluginsTreeModel*		model_;
	QWidget*				loadCheckBox_;
public:
	PluginsTreeItem(const QString& pluginModuleNameExt, const QString& pluginNameExt,
					const QString& pluginVersionExt, PluginsTreeModel* modelExt,
					PluginsTreeItem* parentExt = 0);
	~PluginsTreeItem() { qDeleteAll(childItems_); }

	inline QWidget*		getLoadControl() const { return loadCheckBox_; }
	inline QString		getPluginModuleName() const { return pluginModuleName_; }
	inline QString		getPluginName() const { return pluginName_; }
	inline QString		getPluginVersion() const { return pluginVersion_; }
	inline bool			isPluginLoaded()
	{
		bool loaded = PluginLoader::isPluginLoaded(pluginModuleName_);
		dynamic_cast<QCheckBox*>(loadCheckBox_)->setChecked(loaded);
		return loaded;
	}
	inline bool		isControlActive()
	{
		bool isActive = (isPluginLoaded() || PluginLoader::isPluginLoadable(pluginModuleName_))
				//-- We must disable control if this plugin is not unloadable
				&& PluginLoader::isPluginUnloadable(pluginModuleName_);
		dynamic_cast<QCheckBox*>(loadCheckBox_)->setEnabled(isActive);
		return isActive;
	}
	inline PluginsTreeItem*	parent() { return parentItem_; }
	inline PluginsTreeItem*	child(int number) { return childItems_.value(number); }
	inline int			childCount() const { return childItems_.count(); }
	inline int			childNumber() const
	{
		if (parentItem_)
			return parentItem_->childItems_.indexOf(const_cast<PluginsTreeItem*>(this));

		return 0;
	}

	//QVariant	getData(int column) const;
	bool		insertChild(const QString& pluginModuleNameExt, const QString& pluginNameExt,
							const QString& pluginVersionExt, PluginsTreeModel* modelExt);
	bool		removeChild(int position);
	void		dataChange(bool checked);
};

#endif // ELISE_CORE_MODULES_PLUGINLOADER_PLUGINSTABLEITEM_H_
