#ifndef ELISEDB_H
#define ELISEDB_H

#include "commonheaders.h"

extern QString qsProfile;

extern int WriteSettingToBase(uintptr_t  wParam, uintptr_t lParam);
extern int ReadSettingFromBase(uintptr_t  wParam, uintptr_t lParam);

#endif // ELISEDB_H
