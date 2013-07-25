#ifndef ELISE_CORE_MODULES_FOLDERS_FOLDERS_H_
#define ELISE_CORE_MODULES_FOLDERS_FOLDERS_H_

#include "QtCore"

class QDir;

class Folders
{
public:
	static int		loadFolders();
	static int		unloadFolders();
	static QDir*	getProfileDir(const QString &name = QString());
	static intptr_t	getProfileDir(intptr_t ptrName, intptr_t);
	static QDir*	getPluginsDir();
	static intptr_t	getPluginsDir(intptr_t, intptr_t);
};

#endif // ELISE_CORE_MODULES_FOLDERS_FOLDERS_H_
