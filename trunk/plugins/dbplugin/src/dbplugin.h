#ifndef ELISE_PLUGINS_DBPLUGIN_DBPLUGIN_H_
#define ELISE_PLUGINS_DBPLUGIN_DBPLUGIN_H_

//#include "commonheaders.h"
#include <QtWidgets/QtWidgets>
#include <QtSql>
#include "../../../api/e_dbpluginapi.h"
#include "../../../api/e_pluginapi.h"

class DBPlugin : public QObject, IDBPlugin
{
	Q_OBJECT
	Q_INTERFACES(IDBPlugin)
	//Q_INTERFACES(IPlugin)
	Q_PLUGIN_METADATA(IID "elise.IDBPlugin")
private:
	int							openSysDB();
	QDir						getProfileDir();
	QDir						getProfileDir(const QString& name);
	int							loadProfile(const QString& name, const QString& passwd);
	static QSet<QUuid>*			interfaces;
public:
	PluginInfo*					ElisePluginInfo();
	const QSet<QUuid>*			ElisePluginInterfaces(void);
	int							Load(ICore* coreAPI);
	int							Unload(void);
	QMap<QString, Profile*>*	GetProfiles();
	int							Login(const QString& name, const QString& password,
									  bool savePassword, bool loginDefault);
	int							CreateProfile(const QString& name, const QString& password);
};


#endif // ELISE_PLUGINS_DBPLUGIN_DBPLUGIN_H_
