/*  Elise IM, free crossplatform IM client
	Copyright (C) 2012  Elise project

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ELISE_API_E_DBPLUGINAPI_H_
#define ELISE_API_E_DBPLUGINAPI_H_

#include "e_pluginapi.h"

typedef struct {
	bool	savePassword;
	bool	defaultProfile;
	QString	password;
} Profile;

class IDBPlugin
{
public:
	virtual						~IDBPlugin() {}

	//-- Return list of profiles names that already exists
	//-- The key in returned QMap is a name of profile.
	//-- NOTE: core will destroy list after loading it's elements
	virtual QMap<QString, Profile*>*	GetProfiles() = 0;

	//-- int Login(name, password, savePas, loginDefault);
	//-- Login in profile with gived name and password.
	//-- First parametr is name, second is password.
	//-- Thrid param indicates that password will be restored automatically on next login.
	//-- If both thrid and fourth param is set to 1, then this account will be loaded automatically
	//-- on startup.
	//-- Return 0 on success, non-zero on failure.
	virtual int					Login(const QString&, const QString&, bool, bool) = 0;

	//-- Creates new profile with gived name and password.
	//-- First parametr is name, second is password.
	//-- Return 0 on success, non-zero on failure.
	virtual int					CreateProfile(const QString&, const QString&) = 0;

	//-- Return 0 on success, non-zero on failure.
	//virtual int					ImportProfile(const QString&, const QString&) = 0;
};

Q_DECLARE_INTERFACE(IDBPlugin, "Elise.basicDBPluginInterface/1.0")

#endif // ELISE_API_E_DBPLUGINAPI_H_
