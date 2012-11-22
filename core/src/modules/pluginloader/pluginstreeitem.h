#ifndef ELISE_CORE_MODULES_PLUGINLOADER_PLUGINSTABLEITEM_H_
#define ELISE_CORE_MODULES_PLUGINLOADER_PLUGINSTABLEITEM_H_

class QString;
template <typename T> class QList;
class QVariant;

class PluginsTreeItem
{
private:
	bool					pluginLoaded;
	QString					pluginName;
	QString					pluginVersion;
	QString					pluginUuid;
	QList<PluginsTreeItem*>	childItems;
	PluginsTreeItem*		parentItem;
public:
	PluginsTreeItem(bool pluginLoadedExt, const QString& pluginNameExt,
					const QString& pluginVersionExt, const QString pluginUuidExt,
					PluginsTreeItem* parentExt = 0);
	~PluginsTreeItem() { qDeleteAll(childItems); }

	inline QString		getPluginName() const { return pluginName; }
	inline QString		getPluginVersion() const { return pluginVersion; }
	inline QString		getPluginUuid() const { return pluginUuid; }
	inline bool		isPluginLoaded() const { return pluginLoaded; }
	inline void		setPluginLoaded(bool pluginLoadedExt) { pluginLoaded = pluginLoadedExt; }
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
	bool		insertChild(bool pluginLoadedExt, const QString& pluginNameExt,
							const QString& pluginVersionExt, const QString pluginUuidExt);
	bool		removeChild(int position);
};

#endif // ELISE_CORE_MODULES_PLUGINLOADER_PLUGINSTABLEITEM_H_
