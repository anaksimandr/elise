#include "commonheaders.h"
#include <cstdlib>
#include <ctime>

//-- Temporary module for tests
#include "tests.h"

int InitialiseModularEngine(void);

int shutDown(intptr_t result, intptr_t lParam)
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
	return 0;
}

int main(int argc, char* argv[]) {

	QApplication app(argc, argv);
	app.setQuitOnLastWindowClosed(false);
	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));

	bool failed = false;

	//-- Initialise random number generator
	qsrand(uint(std::time(0)) ^ (qHash(&app)));
	//-- It looks like Qt doesn't always use srand as backend of qsrand
	srand(uint(qrand()));

	//-- Initialise modular engine
	InitialiseModularEngine();

	//-- Load default modules; shut down if failed
	if (LoadDefaultModules())
		failed = true;

	//CreateServiceFunction(&SHUTDOWN_SERVICE, (ELISESERVICE)shutDown);

	//-- Notify about success loading
	//NotifyEventHooks(hModulesLoadedEvent,0,0);

	//-- Ensure that the kernel hooks the SystemShutdownProc() after all plugins
	//HookEvent(ME_SYSTEM_SHUTDOWN,SystemShutdownProc);

	//-- Here, may be, should create some things to work with memory like this
	//forkthread(compactHeapsThread,0,NULL);

	//-- Idle system
	//CreateServiceFunction(MS_SYSTEM_SETIDLECALLBACK,SystemSetIdleCallback);
	//CreateServiceFunction(MS_SYSTEM_GETIDLE, SystemGetIdle);
	//dwEventTime=GetTickCount();

	//-- Remember process pid
	//myPid=GetCurrentProcessId();

	//-- For test
	//QTestWindow* window = new QTestWindow();
	if (failed)
		shutDown(-1, 0);
	else
		new QTestWindow();

	return app.exec();
}
