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
#ifndef ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADEROPTIONS_H_
#define ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADEROPTIONS_H_

class PluginLoaderOptions
{
private:
	static QTreeView*	treeView_;
	static QLabel*	description_;
	static QLabel*	author_;
	static QLabel*	authorEmail_;
	static QLabel*	copyright_;
	static QLabel*	homepage_;
	static QLabel*	uuid_;
public:
	static void saveLoaderOptions();
	static int	createLoaderOptionsPage(intptr_t pfnPageAdder, intptr_t);
	static void	loadSelectedPluginInfo(const QModelIndex& current, const QModelIndex&);
};

#endif // ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADEROPTIONS_H_
