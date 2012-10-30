#ifndef E_DBPLUGINAPI_H
#define E_DBPLUGINAPI_H

#include "e_pluginapi.h"

typedef struct {
	bool	savePassword;
	bool	defaultProfile;
	QString	password;
} PROFILE;

class IDBPlugin
{
public:
	virtual						~IDBPlugin() {}

	//-- If this function return not NULL then the plugin is valid and can be load.
	virtual	PLUGININFO*			ElisePluginInfo(EVersion) = 0;

	//--
	virtual	const QUuid*		ElisePluginInterfaces(void) = 0;

	//-- Load plugin.
	//-- Must be called after login function.
	//-- Return 0 on success, non-zero on failure.
	//-- NOTE: if this function return non-zero then Elise loading wiil be aborted.
	virtual	int					Load(const PLUGINLINK*) = 0;

	//-- Unload plugin.
	//-- Return 0 on success, non-zero on failure.
	virtual	int					Unload(void) = 0;

	//-- Return list of accounts names that already exists
	//-- The key in returned QMap is a name of account.
	//-- NOTE: core will destroy list after loading it's elements
	virtual QMap<QString, PROFILE*>*	GetProfiles() = 0;

	//-- int Login(name, password, savePas, loginDefault);
	//-- Login in account with gived name and password.
	//-- First parametr is name, second is password.
	//-- Thrid param indicates that password will be restored automatically on next login.
	//-- If both thrid and fourth param is set to 1, then this account will be loaded automatically
	//-- on startup.
	//-- Return 0 on success, non-zero on failure.
	virtual int					Login(const QString&, const QString&, bool, bool) = 0;

	//-- Creates new account with gived name and password.
	//-- First parametr is name, second is password.
	//-- Return 0 on success, non-zero on failure.
	virtual int					CreateProfile(const QString&, const QString&) = 0;
};

Q_DECLARE_INTERFACE(IDBPlugin, "Elise.basicDBPluginInterface/1.0")

#endif // E_DBPLUGINAPI_H
