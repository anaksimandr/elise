#ifndef ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADEROPTIONS_H_
#define ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADEROPTIONS_H_

class PluginLoaderOptions
{
private:
	static QTreeView*	treeView_;
	static QLabel*	description_;
	static QLabel*	author_;
	static QLabel*	authorEmail_;
	static QLabel*	copyright_;
	static QLabel*	homepage_;
	static QLabel*	uuid_;
public:
	static void saveLoaderOptions();
	static int	createLoaderOptionsPage(intptr_t pfnPageAdder, intptr_t);
	static void	loadSelectedPluginInfo(const QModelIndex& current, const QModelIndex&);
};

#endif // ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADEROPTIONS_H_
