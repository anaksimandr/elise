#ifndef ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADEROPTIONS_H_
#define ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADEROPTIONS_H_

class PluginLoaderOptions
{
public:
	static void saveLoaderOptions();
	static int	createLoaderOptionsPage(intptr_t pfnPageAdder, intptr_t);
};

#endif // ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADEROPTIONS_H_
