#ifndef ELISE_PLUGINS_DBPLUGIN_ELISEDB_H_
#define ELISE_PLUGINS_DBPLUGIN_ELISEDB_H_

//#include "commonheaders.h"
#include <QtWidgets/QtWidgets>
#include <QtSql>

extern QString qsProfile;

extern int WriteSettingToBase(intptr_t  wParam, intptr_t);
extern int ReadSettingFromBase(intptr_t  wParam, intptr_t);
extern int DelteSettingFromBase(intptr_t  wParam, intptr_t);
#endif // ELISE_PLUGINS_DBPLUGIN_ELISEDB_H_
