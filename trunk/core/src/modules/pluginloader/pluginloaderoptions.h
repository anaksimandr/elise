#ifndef ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADEROPTIONS_H_
#define ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADEROPTIONS_H_

class PluginLoaderOptions
{
private:
	static QTreeView*	treeView;
	static QLabel* description;
	static QLabel* author;
	static QLabel* authorEmail;
	static QLabel* copyright;
	static QLabel* homepage;
public:
	static void saveLoaderOptions();
	static int	createLoaderOptionsPage(intptr_t pfnPageAdder, intptr_t);
	static void	loadSelectedPluginInfo(const QModelIndex& current, const QModelIndex&);
};

#endif // ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADEROPTIONS_H_
