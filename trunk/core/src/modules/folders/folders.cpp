#include "../../../../api/defines.h"
#include "folders.h"
#include "../../core.h"
//#include <QtCore>

const QLatin1String	kCoreGetPluginsDir	=	QLatin1String(__Core_GetPluginsDir_service);
const QLatin1String	kCoreGetProfilesDir	=	QLatin1String(__Core_GetProfilesDir_service);

int Folders::loadFolders()
{
	core->createServiceFunction(&kCoreGetPluginsDir, &Folders::getPluginsDir);
	core->createServiceFunction(&kCoreGetProfilesDir, &Folders::getProfileDir);

	return 0;
}

int Folders::unloadFolders()
{
	core->destroyServiceFunction(&kCoreGetPluginsDir);
	core->destroyServiceFunction(&kCoreGetProfilesDir);

	return 0;
}

QDir* Folders::getProfileDir(const QString& name)
{
	QDir* curDir = new QDir(qApp->applicationDirPath());
	if (!curDir->exists("Profiles"))
		curDir->mkdir("Profiles");
	curDir->cd("Profiles");

	if (!name.isEmpty()) {
		if (!curDir->exists(name))
			curDir->mkdir(name);
		curDir->cd(name);
	}

	return curDir;
}

intptr_t Folders::getProfileDir(intptr_t ptrName, intptr_t)
{
	QString name;
	if (ptrName)
		name = *(reinterpret_cast<QString*>(ptrName));

	return reinterpret_cast<intptr_t>(Folders::getProfileDir(name));
}

QDir* Folders::getPluginsDir()
{
	QDir* curDir = new QDir(qApp->applicationDirPath());
	if (!curDir->exists("Plugins"))
		curDir->mkdir("Plugins");
	curDir->cd("Plugins");
	return curDir;
}

intptr_t Folders::getPluginsDir(intptr_t, intptr_t)
{
	return reinterpret_cast<intptr_t>(Folders::getPluginsDir());
}
