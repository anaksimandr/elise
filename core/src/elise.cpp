#include <cstdlib>
#include <ctime>
#include "elise.h"
#include "modules.h"

//-- Temporary module for tests
#include "tests.h"

namespace core
{

//int InitialiseModularEngine(void);

int shutDown(intptr_t result, intptr_t)
{
	//-- PreShut down stage
	//SetEvent(hMirandaShutdown);
	//NotifyEventHooks(hPreShutdownEvent, 0, 0);

	//-- Kill all threads
	//UnwindThreadWait();

	//-- Shut down stage
	//NotifyEventHooks(hShutdownEvent, 0, 0);
	//-- If the hooks generated any messages, it'll get processed before exit()
	QApplication::processEvents();

	//UnloadNewPluginsModule();
	//DestroyModularEngine();
	//CloseHandle(hStackMutex);
	//CloseHandle(hMirandaShutdown);
	//CloseHandle(hThreadQueueEmpty);
	//DestroyWindow(hAPCWindow);

	//if (pTaskbarInterface)
	//	pTaskbarInterface->Release();

	//if (bufferedPaintUninit) bufferedPaintUninit();

	//window->~QTestWindow();
	UnloadDefaultModules();

	QApplication::exit(result);
	return result;
}

} // namespace core

#ifndef NDEBUG

#include <QFile>
void messageOutput(QtMsgType type, const char *msg)
{
	QFile file("log.txt");
	file.open(QIODevice::Append | QIODevice::Text);
	QTextStream log(&file);
	switch (type) {
		case QtDebugMsg:
			log << "Debug: " << msg << "\n";
			break;
		case QtWarningMsg:
			log << "Warning: " << msg << "\n";
			break;
		case QtCriticalMsg:
			log << "Critical: " << msg << "\n";
			break;
		case QtFatalMsg:
			log << "Fatal: " << msg << "\n";
			//abort();
	}
	log << "\n";
	file.close();
}

#endif //NDEBUG

int main(int argc, char* argv[])
{
#ifndef NDEBUG
	qInstallMsgHandler(messageOutput);
#endif //NDEBUG
	QApplication app(argc, argv);
	app.setQuitOnLastWindowClosed(false);
	//QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
	//QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));

	//-- Initialise random number generator
	qsrand(uint(std::time(0)) ^ (qHash(&app)));
	//-- It looks like Qt doesn't always use srand as backend of qsrand
	srand(uint(qrand()));

	//-- Initialise modular engine
	//InitialiseModularEngine();

	//-- Load default modules; shut down if failed
	if (core::LoadDefaultModules())
		return core::shutDown(-1, 0);

	//CreateServiceFunction(&SHUTDOWN_SERVICE, (ELISESERVICE)shutDown);

	//-- Notify about success loading
	//NotifyEventHooks(hModulesLoadedEvent,0,0);

	//-- Ensure that the kernel hooks the SystemShutdownProc() after all plugins
	//HookEvent(ME_SYSTEM_SHUTDOWN,SystemShutdownProc);

	//-- Idle system
	//CreateServiceFunction(MS_SYSTEM_SETIDLECALLBACK,SystemSetIdleCallback);
	//CreateServiceFunction(MS_SYSTEM_GETIDLE, SystemGetIdle);
	//dwEventTime=GetTickCount();

	//-- Remember process pid
	//myPid=GetCurrentProcessId();

	//-- For test
	//QTestWindow* window = new QTestWindow();

	new QTestWindow();

	return app.exec();
}
