#ifndef ELISEDB_H
#define ELISEDB_H

//#include "commonheaders.h"
#include <QtWidgets/QtWidgets>
#include <QtSql>

extern QString qsProfile;

extern int WriteSettingToBase(intptr_t  wParam, intptr_t);
extern int ReadSettingFromBase(intptr_t  wParam, intptr_t);
extern int DelteSettingFromBase(intptr_t  wParam, intptr_t);
#endif // ELISEDB_H
