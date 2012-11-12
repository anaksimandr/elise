#ifndef NEWPLUGIN_H
#define NEWPLUGIN_H

//#include "commonheaders.h"
#include <QtWidgets/QtWidgets>
#include <QtSql>
#include "../../../api/e_dbpluginapi.h"
#include "../../../api/e_pluginapi.h"

class DBPlugin : public QObject, IDBPlugin
{
	Q_OBJECT
	Q_INTERFACES(IDBPlugin)
	Q_PLUGIN_METADATA(IID "elise.IDBPlugin")
private:
	int							openSysDB();
	QDir						getProfileDir();
	QDir						getProfileDir(const QString& name);
	int							loadProfile(const QString& name, const QString& passwd);
public:
	PluginInfo*					ElisePluginInfo();
	const QUuid*				ElisePluginInterfaces(void);
	int							Load(ICore* coreAPI);
	int							Unload(void);
	QMap<QString, Profile*>*	GetProfiles();
	int							Login(const QString& name, const QString& password,
									  bool savePassword, bool loginDefault);
	int							CreateProfile(const QString& name, const QString& password);
};


#endif // NEWPLUGIN_H
