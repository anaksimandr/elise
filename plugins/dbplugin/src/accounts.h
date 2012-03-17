#ifndef CREATEACC_H
#define CREATEACC_H

#include "commonheaders.h"

//-- This class provides account managment functional
class AccountManager : public QDialog
{
	Q_OBJECT
private:

private slots:
	void			abort();

public:
					AccountManager();
					~AccountManager();

};

#endif // CREATEACC_H
