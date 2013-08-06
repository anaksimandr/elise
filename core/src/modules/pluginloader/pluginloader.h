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
//extern const QLatin1String	kCoreGetPluginInterfaces;

class IPlugin;
class IDBPlugin;
class CoreAPI;
class QStringList;

typedef struct {
	int				type;
	QPluginLoader*	loader;
} Plugin;

class PluginLoader
{
private:
	static QDir*					pluginsDir_;
	static QMap<QString, Plugin>*	plugins_;
	static QSet<int>*				loadedPluginsTypes_;

	static bool			isPluginDisabled(const QString& pluginModuleName);
public:
	static QJsonObject*	getPluginInfo(const QString& pluginModuleName);
	static int			loadPlugins();
	static QObject*		loadPlugin(const QString& pluginModuleName);
	static int			loadPluginLoader();
	static int			unloadPluginLoader();
	static int			unloadPlugin(const QString& pluginModuleName);
	static int			unloadPlugins();
	static int			updatePluginState(const QString& pluginModuleName,
												bool disable);
	static bool			isPluginUnloadable(const QString& pluginModuleName);
	static bool			isPluginLoadable(const QString& pluginModuleName);
	static bool			isPluginLoaded(const QString& pluginModuleName);
	static inline bool	isPluginLoaded(int id) { return loadedPluginsTypes_->contains(id); }
	static intptr_t		isPluginLoaded(intptr_t name, intptr_t type);
	static const QMap<QString, Plugin>*		getAvailablePlugins();
	static const QStringList				getDBPlugins();

	static inline int	confirmPluginModule(const QString &pluginModuleName)
	{
		if (!pluginsDir_->exists(pluginModuleName)) {
			QMessageBox::critical(0, "Error",
								  "Plugin module" + pluginModuleName
								  + " is not found.\nPerhaps it was deleted.",
								  QMessageBox::Ok);
			return 0;
		}
		return 1;
	}
};

#endif // ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADER_H_
