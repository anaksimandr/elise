#ifndef ELISE_PLUGINS_TESTS_TESTWINDOW_H_
#define ELISE_PLUGINS_TESTS_TESTWINDOW_H_

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include "developer.h"

extern const QLatin1String	kCoreShutdown_service;
extern const QLatin1String	kClistShow_service;
extern const QLatin1String	kClistHide_service;

class TestWindow : public QWidget
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
public:
	bool			vis;
	
	TestWindow();
	~TestWindow();

	//-- Utils
	void			uuidCreate();
	void			showOptions();
	void			setTrayIcon();
	void			changeAcc();
	//-- Database
	void			saveSetting();
	void			readSetting();
	void			delSetting();
	//-- Hooks
	void			createHookblEvent();
	void			hookEvent();
	void			notifyEventHooks();
	void			unhookEvent();
	void			deleteHokableEvent();
	//-- Services
	void			createService();
	void			checkService();
	void			testtService();
	void			delService();


	static int		hideMainWindow(intptr_t, intptr_t);
	static intptr_t	showCList(intptr_t, intptr_t);
	static intptr_t	hideCList(intptr_t, intptr_t);

	inline void setBarValue(int val)
	{
		bar->setValue(val);
		QApplication::processEvents();
	}

	inline void setOutput(QString text)
	{
		output->setText(text);
		QApplication::processEvents();
	}

	inline void buttonExit()
	{
		core->callService(&kCoreShutdown_service, 0, 0);
	}
};

#endif // ELISE_PLUGINS_TESTS_TESTWINDOW_H_
