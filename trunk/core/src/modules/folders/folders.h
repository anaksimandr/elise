#ifndef ELISE_CORE_MODULES_FOLDERS_FOLDERS_H_
#define ELISE_CORE_MODULES_FOLDERS_FOLDERS_H_

class QDir;

class Folders
{
public:
	static int		loadFolders();
	static int		unloadFolders();
	static QDir		getProfileDir();
	static intptr_t	getProfileDir(intptr_t, intptr_t);
};

#endif // ELISE_CORE_MODULES_FOLDERS_FOLDERS_H_
