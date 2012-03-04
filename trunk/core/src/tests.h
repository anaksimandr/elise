#ifndef TESTS_H
#define TESTS_H

#include "commonheaders.h"

class QTestWindow : public QWidget
{
	Q_OBJECT
private:
	QLabel*			output;
	QProgressBar*	bar;
protected:
	//Elise*			parent;
	//virtual void	contextMenuEvent(QContextMenuEvent* e);
public slots:
	//-- Utils
	void			uuidCreate();
	void			testNewPlugin();
	void			testDB();
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
