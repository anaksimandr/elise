/*  Elise IM, free crossplatform IM client
	Copyright (C) 2012  Elise project

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdlib>
#include <ctime>
#include <QtWidgets/QtWidgets>
#include "core.h"

bool Core::profileLoaded;

int Core::launch()
{
	profileLoaded = false;

	if (loadCore())
		return 1;

	core->createHookableEvent(&kCorePluginsLoaded);
	core->createHookableEvent(&kCorePluginsUnloaded);
	core->createHookableEvent(&kPreshutdown);

	if (loadProfile(0, 0))
		return 1;

	//-- Notify about success loading

	return 0;
}

int Core::shutdown(intptr_t result, intptr_t)
{
	core->notifyEventHooks(&kPreshutdown, 0, 0);
	//QApplication::processEvents();

	unloadCore();

	QApplication::exit(result);
	return result;
}

#ifndef NDEBUG
#include <QFile>
void messageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
	QFile file("log.txt");
	file.open(QIODevice::Append | QIODevice::Text);
	QTextStream log(&file);
	switch (type) {
		case QtDebugMsg:
			log << "Debug: " << msg << "\n";
			break;
		case QtWarningMsg:
		{
			if (msg.indexOf("QLayout: Attempting to add QLayout") == -1)
				log << "Warning: " << msg << "\n";
		}
			break;
		case QtCriticalMsg:
			log << "Critical: " << msg << "\n";
			break;
		case QtFatalMsg:
			log << "Fatal: " << msg << "\n";
	}
	log << "File:		" << context.file << "\n" << "Function:	" << context.function << "\n";
	log << "Line:		" << context.line << "\n\n";
	file.close();
}

#endif //NDEBUG

int main(int argc, char* argv[])
{
#ifndef NDEBUG
	qInstallMessageHandler(messageOutput);
#endif //NDEBUG
	QApplication app(argc, argv);
	app.setQuitOnLastWindowClosed(false);
	//QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
	//QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));

	//-- Initialise random number generator
	qsrand(uint(std::time(0)) ^ (qHash(&app)));
	//-- It looks like Qt doesn't always use srand as backend of qsrand
	srand(uint(qrand()));

	if (Core::launch())
		return Core::shutdown(-1, 0);

	return app.exec();
}
