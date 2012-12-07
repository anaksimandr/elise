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
	QString					pluginModuleName;
	QString					pluginName;
	QString					pluginVersion;
	QList<PluginsTreeItem*>	childItems;
	PluginsTreeItem*		parentItem;
	PluginsTreeModel*		model;
	QWidget*				loadControl;
public:
	PluginsTreeItem(const QString& pluginModuleNameExt, const QString& pluginNameExt,
					const QString& pluginVersionExt, PluginsTreeModel* modelExt,
					PluginsTreeItem* parentExt = 0);
	~PluginsTreeItem() { qDeleteAll(childItems); }

	inline QWidget*		getLoadControl() const { return loadControl; }
	inline QString		getPluginModuleName() const { return pluginModuleName; }
	inline QString		getPluginName() const { return pluginName; }
	inline QString		getPluginVersion() const { return pluginVersion; }
	inline bool			isPluginLoaded()
	{
		bool loaded = PluginLoader::isPluginLoaded(pluginModuleName);
		dynamic_cast<QCheckBox*>(loadControl)->setChecked(loaded);
		return loaded;
	}
	inline bool		isControlActive()
	{
		bool isActive = (isPluginLoaded() || PluginLoader::isPluginLoadable(pluginModuleName))
				//-- We must disable control if this plugin is not unloadable
				&& PluginLoader::isPluginUnloadable(pluginModuleName);
		dynamic_cast<QCheckBox*>(loadControl)->setEnabled(isActive);
		return isActive;
	}
	inline PluginsTreeItem*	parent() { return parentItem; }
	inline PluginsTreeItem*	child(int number) { return childItems.value(number); }
	inline int			childCount() const { return childItems.count(); }
	inline int			childNumber() const
	{
		if (parentItem)
			return parentItem->childItems.indexOf(const_cast<PluginsTreeItem*>(this));

		return 0;
	}

	//QVariant	getData(int column) const;
	bool		insertChild(const QString& pluginModuleNameExt, const QString& pluginNameExt,
							const QString& pluginVersionExt, PluginsTreeModel* modelExt);
	bool		removeChild(int position);
	void		dataChange(bool checked);
};

#endif // ELISE_CORE_MODULES_PLUGINLOADER_PLUGINSTABLEITEM_H_
