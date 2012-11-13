#ifndef TESTS_H
#define TESTS_H

#include <QtWidgets/QtWidgets>
#include "services.h"
#include "../../api/e_database.h"
#include "modules/options/options.h"
#include "modules/tray.h"


class QTestWindow : public QWidget
{
	Q_OBJECT
private:
	QLabel*			output;
	QProgressBar*	bar;
	QLineEdit*		v1;
	QLineEdit*		v2;
	QLineEdit*		v3;
	QLineEdit*		v4;
	QLineEdit*		v5;
	THook			thTray;
protected:
	//Elise*			parent;
	//virtual void	contextMenuEvent(QContextMenuEvent* e);
public slots:
	//-- Utils
	void			uuidCreate();
	void			testNewPlugin();
	void			showOptions();
	void			setTrayIcon();
	//void			testDB();
	void			changeAcc();
	//-- Database
	void			saveSetting();
	void			readSetting();
	void			delSetting();
	//-- Services
	void			buttonExit();
	void			createService();
	void			checkService();
	void			testtService();
	void			delService();
	//-- Hooks
	void			createHookblEvent();
	void			hookEvent();
	void			notifyEventHooks();
	void			unhookEvent();
	void			deleteHokableEvent();
	//-- Long test
	void			testPointerCalls();
	void			testNameCalls();
	void			testQStringCalls();
	void			simpleTest();
signals:
	//int				testSignal(uintptr_t,uintptr_t);
public:
	QTestWindow();
	~QTestWindow();
	void			setBarValue(int val);
	void			setOutput(QString text);
	bool			vis;
};

#endif // TESTS_H
