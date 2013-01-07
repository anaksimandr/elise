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
#ifndef ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADER_H_
#define ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADER_H_

#include <QtWidgets/QtWidgets>
#include "../../../../api/e_pluginapi.h"

extern const QLatin1String	kCoreIsPluginLoaded;
extern const QLatin1String	kCoreGetPluginInterfaces;

class IPlugin;
class IDBPlugin;
class CoreAPI;

typedef struct {
	bool		loaded;
	QObject*	instance;
	QUuid*		uuid;
} Plugin;

class PluginLoader
{
private:
	static QDir						pluginsDir_;
	static QMap<QString, Plugin>*	plugins_;
	static QMap<QUuid, QString>*	interfaces_;

	static bool			isLoadingPluginDisabled(const QString& pluginModuleName);
public:
	static QDir			getPluginsDir();
	static PluginInfo*	getElisePluginInfo(const QString& pluginModuleName);
	static intptr_t		getElisePluginInterfaces(intptr_t id, intptr_t);
	static int			loadDBPlugin(const QString& pluginModuleName,
									 const QMap<QString, IDBPlugin*>* dbPlugins);
	static int			loadPlugins();
	static int			loadPlugin(const QString& pluginModuleName);
	static int			loadPluginLoader();
	static int			unloadPluginLoader();
	static int			unloadPlugin(const QString& pluginModuleName);
	static int			unloadAllPlugins();
	static int			savePluginStateOrDelete(const QString& pluginModuleName,
												bool disableOrDelete);
	static bool			isPluginUnloadable(const QString& pluginModuleName);
	static bool			isPluginLoadable(const QString& pluginModuleName);
	static bool			isPluginLoaded(const QString& pluginModuleName);
	static intptr_t		isPluginLoaded(intptr_t id, intptr_t);
	static const QMap<QString, Plugin>*	getAvailablePlugins();
	static const QMap<QString, IDBPlugin*>*	getDBPlugins();
};

#endif // ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADER_H_
