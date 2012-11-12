#ifndef ELISEDB_H
#define ELISEDB_H

//#include "commonheaders.h"
#include <QtWidgets/QtWidgets>
#include <QtSql>

extern QString qsProfile;

extern int WriteSettingToBase(intptr_t  wParam, intptr_t lParam);
extern int ReadSettingFromBase(intptr_t  wParam, intptr_t lParam);
extern int DelteSettingFromBase(intptr_t  wParam, intptr_t lParam);
#endif // ELISEDB_H
