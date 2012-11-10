#ifndef COMMONHEADERS_H
#define COMMONHEADERS_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets/QtWidgets>
#include "services.h"
#include "../../api/e_database.h"
#include "../../api/e_options.h"
#include "../../api/e_pluginapi.h"
#include "../../api/e_dbpluginapi.h" //-- We need include this after e_pluginapi.h
#include "modules.h"
#include "modules/tray.h"
//#include "modules/logon.h"
#include "modules/profilemanager.h"
#include "modules/options/treeitem.h"
#include "modules/options/treemodel.h"
#include "modules/options/options.h"
#include "modules/pluginloader/pluginloader.h"

namespace Core
{
const EVersion g_eliseVersion = {0,0,0,1};
} //namespace Core

//-- Some stuff from windows
//typedef void* HANDLE;
//typedef HANDLE* PHANDLE;

#endif // COMMONHEADERS_H
