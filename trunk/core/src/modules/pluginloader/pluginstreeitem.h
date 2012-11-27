#ifndef ELISE_CORE_MODULES_PLUGINLOADER_PLUGINSTABLEITEM_H_
#define ELISE_CORE_MODULES_PLUGINLOADER_PLUGINSTABLEITEM_H_

#include "pluginloader.h"

class QString;
template <typename T> class QList;
class QVariant;

class PluginsTreeItem
{
private:
	QString					pluginModuleName;
	QString					pluginName;
	QString					pluginVersion;
	QList<PluginsTreeItem*>	childItems;
	PluginsTreeItem*		parentItem;
public:
	PluginsTreeItem(const QString& pluginModuleNameExt, const QString& pluginNameExt,
					const QString& pluginVersionExt, PluginsTreeItem* parentExt = 0);
	~PluginsTreeItem() { qDeleteAll(childItems); }

	inline QString		getPluginModuleName() const { return pluginModuleName; }
	inline QString		getPluginName() const { return pluginName; }
	inline QString		getPluginVersion() const { return pluginVersion; }
	inline bool		isPluginLoaded() { return PluginLoader::isPluginLoaded(pluginModuleName); }
	inline bool		isPluginLoadable() { return PluginLoader::isPluginLoadable(pluginModuleName); }
	inline void		setPluginLoaded(bool pluginLoadedExt)
	{
		if (pluginLoadedExt)
			PluginLoader::loadPlugin(pluginModuleName);
		else
			PluginLoader::unloadPlugin(pluginModuleName);
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
							const QString& pluginVersionExt);
	bool		removeChild(int position);
};

#endif // ELISE_CORE_MODULES_PLUGINLOADER_PLUGINSTABLEITEM_H_
