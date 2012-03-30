#ifndef NEWPLUGIN_H
#define NEWPLUGIN_H

#include "commonheaders.h"

class DBPlugin : public QObject, IDBPlugin
{
	Q_OBJECT
	Q_INTERFACES(IDBPlugin)
private:
	int							openSysDB();
	QDir						getProfileDir();
	QDir						getProfileDir(const QString& name);
	int							loadProfile(const QString& name, const QString& passwd);
public:
	PLUGININFO*					ElisePluginInfo(EVersion);
	const QUuid*				ElisePluginInterfaces(void);
	int							Load(const PLUGINLINK* link);
	int							Unload(void);
	QMap<QString, ACCOUNT*>*	GetAccounts();
	int							Login(const QString& name, const QString& password,
									  bool savePassword, bool loginDefault);
	int							CreateAccount(const QString& name, const QString& password);
};


#endif // NEWPLUGIN_H
