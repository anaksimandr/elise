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

#include "tests.h"

const unsigned char intType		=	0;	//-- int
const unsigned char realType	=	1;	//-- double
const unsigned char textType	=	2;	//-- QString
const unsigned char blobType	=	3;	//-- QByteArray

int hideMainWindow(intptr_t, intptr_t);

#include <QtSql>

QTestWindow* wii;

QTestWindow::QTestWindow()
{
	vis = false;

	this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	QVBoxLayout* layoutV = new QVBoxLayout(this);
	QHBoxLayout* layoutH = new QHBoxLayout(this);
	QVBoxLayout* layoutUtil = new QVBoxLayout(this);
	QVBoxLayout* layoutDB = new QVBoxLayout(this);
	QVBoxLayout* layoutServ = new QVBoxLayout(this);
	QVBoxLayout* layoutHook = new QVBoxLayout(this);
	QVBoxLayout* layoutTest = new QVBoxLayout(this);
	layoutV->addLayout(layoutH);
	layoutH->addLayout(layoutUtil);
	layoutH->addLayout(layoutDB);
	layoutH->addLayout(layoutServ);
	layoutH->addLayout(layoutHook);
	layoutH->addLayout(layoutTest);
	QHBoxLayout* layoutHInput = new QHBoxLayout(this);

	QLabel* lbl = new QLabel(this);
	layoutUtil->addWidget(lbl);
	lbl->setText("Utils");
	lbl->setMaximumHeight(15);
	layoutUtil->setAlignment(lbl, Qt::AlignTop | Qt::AlignHCenter);

	lbl = new QLabel(this);
	layoutDB->addWidget(lbl);
	lbl->setText("Database");
	lbl->setMaximumHeight(15);
	layoutDB->setAlignment(lbl, Qt::AlignTop | Qt::AlignHCenter);

	lbl = new QLabel(this);
	layoutServ->addWidget(lbl);
	lbl->setText("Services");
	lbl->setMaximumHeight(15);
	layoutServ->setAlignment(lbl, Qt::AlignTop | Qt::AlignHCenter);

	lbl = new QLabel(this);
	layoutHook->addWidget(lbl);
	lbl->setText("Hooks");
	lbl->setMaximumHeight(15);
	layoutHook->setAlignment(lbl, Qt::AlignTop | Qt::AlignHCenter);

	lbl = new QLabel(this);
	layoutTest->addWidget(lbl);
	lbl->setText("Long tests");
	lbl->setMaximumHeight(15);
	layoutTest->setAlignment(lbl, Qt::AlignTop | Qt::AlignHCenter);

	bar = new QProgressBar(this);
	layoutV->addWidget(bar);
	bar->setRange(0, 1000000);

	layoutV->addLayout(layoutHInput);
	v1 = new QLineEdit(this);
	layoutHInput->addWidget(v1);
	v2 = new QLineEdit(this);
	layoutHInput->addWidget(v2);
	v3 = new QLineEdit(this);
	layoutHInput->addWidget(v3);
	v4 = new QLineEdit(this);
	layoutHInput->addWidget(v4);
	v5 = new QLineEdit(this);
	layoutHInput->addWidget(v5);

	v1->setPlaceholderText("module");
	v2->setPlaceholderText("setting");
	v3->setPlaceholderText("value");
	v4->setPlaceholderText("type");
	v5->setPlaceholderText("");

	output = new QLabel(this);
	layoutV->addWidget(output);
	layoutV->setAlignment(output, Qt::AlignHCenter);
	output->setTextInteractionFlags(output->textInteractionFlags() | Qt::TextSelectableByMouse);

	this->resize(100, 200);

	//-- Utils
	QPushButton* button = new QPushButton(this);
	button->setText("Create UUID");
	connect(button, SIGNAL(clicked()), this, SLOT(uuidCreate()));
	layoutUtil->addWidget(button);
	button->setMinimumWidth(100);

	button = new QPushButton(this);
	button->setText("Test plugin");
	connect(button, SIGNAL(clicked()), this, SLOT(testNewPlugin()));
	layoutUtil->addWidget(button);

	button = new QPushButton(this);
	button->setText("Options");
	connect(button, SIGNAL(clicked()), this, SLOT(showOptions()));
	layoutUtil->addWidget(button);

	button = new QPushButton(this);
	button->setText("Set icon");
	connect(button, &QPushButton::clicked, this, &QTestWindow::setTrayIcon);
	layoutUtil->addWidget(button);

	button = new QPushButton(this);
	button->setText("Change account");
	connect(button, SIGNAL(clicked()), this, SLOT(changeAcc()));
	layoutUtil->addWidget(button);

	layoutUtil->setAlignment(button, Qt::AlignTop);

	//-- Database
	button = new QPushButton(this);
	button->setText("Save setting");
	connect(button, SIGNAL(clicked()), this, SLOT(saveSetting()));
	layoutDB->addWidget(button);
	button->setMinimumWidth(100);

	button = new QPushButton(this);
	button->setText("Read setting");
	connect(button, SIGNAL(clicked()), this, SLOT(readSetting()));
	layoutDB->addWidget(button);

	button = new QPushButton(this);
	button->setText("Delete setting");
	connect(button, SIGNAL(clicked()), this, SLOT(delSetting()));
	layoutDB->addWidget(button);

	layoutDB->setAlignment(button, Qt::AlignTop);

	//-- Services
	button = new QPushButton(this);
	button->setText("Add service");
	connect(button, SIGNAL(clicked()), this, SLOT(createService()));
	layoutServ->addWidget(button);
	button->setMinimumWidth(100);

	button = new QPushButton(this);
	button->setText("Check service");
	connect(button, SIGNAL(clicked()), this, SLOT(checkService()));
	layoutServ->addWidget(button);

	button = new QPushButton(this);
	button->setText("Test service");
	connect(button, SIGNAL(clicked()), this, SLOT(testtService()));
	layoutServ->addWidget(button);

	button = new QPushButton(this);
	button->setText("Delete service");
	connect(button, SIGNAL(clicked()), this, SLOT(delService()));
	layoutServ->addWidget(button);
	layoutServ->setAlignment(button, Qt::AlignTop);

	//-- Hooks
	button = new QPushButton(this);
	button->setText("Create hookable event");
	connect(button, SIGNAL(clicked()), this, SLOT(createHookblEvent()));
	layoutHook->addWidget(button);
	button->setMinimumWidth(100);

	button = new QPushButton(this);
	button->setText("Hook event");
	connect(button, SIGNAL(clicked()), this, SLOT(hookEvent()));
	layoutHook->addWidget(button);

	button = new QPushButton(this);
	button->setText("Notify event hooks");
	connect(button, SIGNAL(clicked()), this, SLOT(notifyEventHooks()));
	layoutHook->addWidget(button);

	button = new QPushButton(this);
	button->setText("Unhook event");
	connect(button, SIGNAL(clicked()), this, SLOT(unhookEvent()));
	layoutHook->addWidget(button);

	button = new QPushButton(this);
	button->setText("Delete hookable event");
	connect(button, SIGNAL(clicked()), this, SLOT(deleteHokableEvent()));
	layoutHook->addWidget(button);

	layoutHook->setAlignment(button, Qt::AlignTop);

	//-- Long tests
	button = new QPushButton(this);
	button->setText("Test pointer calls");
	connect(button, SIGNAL(clicked()), this, SLOT(testPointerCalls()));
	layoutTest->addWidget(button);
	button->setMinimumWidth(100);

	button = new QPushButton(this);
	button->setText("Test name calls");
	connect(button, SIGNAL(clicked()), this, SLOT(testNameCalls()));
	layoutTest->addWidget(button);

	button = new QPushButton(this);
	button->setText("Test QString calls");
	connect(button, SIGNAL(clicked()), this, SLOT(testQStringCalls()));
	layoutTest->addWidget(button);

	button = new QPushButton(this);
	button->setText("Simple test");
	connect(button, SIGNAL(clicked()), this, SLOT(simpleTest()));
	layoutTest->addWidget(button);

	layoutTest->setAlignment(button, Qt::AlignTop);

	//-- Exit
	button = new QPushButton(this);
	button->setText("Exit");
	connect(button, SIGNAL(clicked()), this, SLOT(buttonExit()));
	layoutV->addWidget(button);
	layoutV->setAlignment(button, Qt::AlignBottom);

	//-- Work with tray
	thTray.num =  core->hookEvent(&TRAY_SINGLECLICK, &hideMainWindow);
	thTray.name = new QLatin1String(TRAY_SINGLECLICK);

	wii = this;
	this->show();
	vis = true;

	QAction* action = new QAction("Call testQStringCalls", 0);
	connect(action, SIGNAL(triggered()), this, SLOT(testQStringCalls()));
	core->callService(&TRAY_ADD_MENUITEM, (uintptr_t)action, 0);

	action = new QAction("Exit", 0);
	connect(action, SIGNAL(triggered()), this, SLOT(buttonExit()));
	core->callService(&TRAY_ADD_MENUITEM, (uintptr_t)action, 0);
}

int hideMainWindow(intptr_t, intptr_t)
{
	if (wii->vis) {
		wii->hide();
		wii->vis = false;
	}
	else {
		wii->show();
		wii->vis = true;
	}

	return 0;
}

QTestWindow::~QTestWindow()
{
	core->unhookEvent(thTray);
}

void QTestWindow::setBarValue(int val)
{
	bar->setValue(val);
	QApplication::processEvents();
}

//-- Utils

void QTestWindow::uuidCreate()
{
	setOutput(QUuid::createUuid().toString());
}

const QLatin1String testplugin_service = QLatin1String("TESTPLUGIN_SERVICE");
void QTestWindow::testNewPlugin()
{
	core->callService(&testplugin_service, 0, 0);
}

void QTestWindow::changeAcc()
{
	this->~QTestWindow();
	if (core->callService(&kChangeProfile_service, 0, 0) == -2)
		QMessageBox::critical(this, "Error", "Service not found.", QMessageBox::Ok);
}

void QTestWindow::showOptions()
{
	core->callService(&kOptionsShow_service, 0, 0);
}

void QTestWindow::setTrayIcon()
{
	QString filename = QFileDialog::getOpenFileName(this, "Open file", "", "SVG (*.svg);; Files (*.*)");
	//QMessageBox::critical(this, "Debug", "2", QMessageBox::Ok);
	QIcon* icon = new QIcon(filename);
	core->callService(&TRAY_SET_ICON, (uintptr_t)icon, 0);
	delete icon;
}

//-- Database

void QTestWindow::saveSetting()
{
	QString module = QString(v1->text().toLatin1());
	QString setting = QString(v2->text().toLatin1());
	Setting* set = new Setting;
	set->contact = 0;
	set->qsModule = &module;
	set->qsSetting = &setting;
	set->var = new DBVariant;
	set->var->type = (unsigned char)v4->text().toInt();
	QString value;
	switch (set->var->type) {
		case intType:
			set->var->intValue = v3->text().toInt();
			break;
		case realType:
			set->var->realValue = v3->text().toDouble();
			break;
		case textType:
		{
			value = v3->text();
			set->var->textValue = &value;
		}
			break;
		case blobType:
		{
			value = v3->text();
			set->var->textValue = &value;
		}
			break;
	}

	core->callService(&kDBWriteSetting_service, reinterpret_cast<intptr_t>(set), 0);

	delete set->var;
	delete set;
}

void QTestWindow::readSetting()
{
	QString module = QString(v1->text().toLatin1());
	QString setting = QString(v2->text().toLatin1());
	Setting* set = new Setting;
	set->contact = 0;
	set->qsModule = &module;
	set->qsSetting = &setting;
	set->var = new DBVariant;
	set->var->type = (unsigned char)v4->text().toInt();
	if (set->var->type == textType)
		set->var->textValue = new QString;
	else if (set->var->type == blobType)
		set->var->blobValue = new QByteArray;
	if (!core->callService(&kDBReadSetting_service, reinterpret_cast<intptr_t>(set), 0)) {
		switch (set->var->type) {
			case intType:
				setOutput(QString::number(set->var->intValue));
				break;
			case realType:
				setOutput(QString::number(set->var->realValue));
				break;
			case textType:			
				setOutput(*set->var->textValue);
				break;
			case blobType:
			{
				setOutput(*set->var->blobValue);
			}
				break;
		}
	}
	else
		setOutput("Error");

	delete set->var;
	delete set;
}

void QTestWindow::delSetting()
{
	QString module = QString(v1->text().toLatin1());
	QString setting = QString(v2->text().toLatin1());
	Setting* set = new Setting;
	set->contact = 0;
	set->qsModule = &module;
	set->qsSetting = &setting;
	set->var = new DBVariant;
	set->var->type = (unsigned char)v4->text().toInt();

	if (!core->callService(&kDBDellSetting_service, reinterpret_cast<intptr_t>(set), 0))
		setOutput("Setting deleted");
	else
		setOutput("Error");

	delete set->var;
	delete set;
}

static QLatin1String name = QLatin1String("TEST_SERVISE");
static QLatin1String hkevName = QLatin1String("TEST_HOOKABLE_EVENT");
static THook hookEv;

//-- Hooks

int testHook(intptr_t, intptr_t lParam)
{
	QMessageBox qmes;
	qmes.setText("Test hook called! lParam is " + QString::number(lParam));
	qmes.exec();
	return 0;
}

void QTestWindow::createHookblEvent()
{
	if (!core->createHookableEvent(&hkevName))
		setOutput("Hookable event created");
	else
		setOutput("Create hookable event FAIL!");
}

void QTestWindow::hookEvent()
{
	hookEv.num = core->hookEvent(&hkevName, &testHook);
	hookEv.name = &hkevName;
	if (hookEv.num != -1)
		setOutput("Hook event success, num is " + QString::number(hookEv.num));
	else
		setOutput("Hook event FAILED!");
}

void QTestWindow::notifyEventHooks()
{
	int ret = core->notifyEventHooks(&hkevName, 0, 137);
	if (ret)
		setOutput("Notify event FAILED!");
	else
		setOutput("Notify event success!");
}

void QTestWindow::unhookEvent()
{
	int ret = core->unhookEvent(hookEv);
	if (ret != -1)
		setOutput("Unhook event success, num was " + QString::number(hookEv.num));
	else
		setOutput("Unhook event FAILED!");
}

void QTestWindow::deleteHokableEvent()
{
	if (!core->destroyHookableEvent(&hkevName))
		setOutput("Hookable event destroed");
	else
		setOutput("Destroy hookable event FAIL!");
}

//-- Services
int testService(intptr_t wParam, intptr_t)
{
	QMessageBox qmes;
	qmes.setText("Test service working");
	qmes.exec();
	return wParam;
}

void QTestWindow::createService()
{
	if (!core->createServiceFunction(&name, &testService))
		setOutput("Service created");
	else
		setOutput("Create service FAIL!");
}

void QTestWindow::checkService()
{
	if (core->serviceExists(&name))
		setOutput("Test service exists");
	else
		setOutput("Test service not found");
}

void QTestWindow::testtService()
{
	int res = core->callService(&name, 111, 0);
	if (res == 111)
		setOutput("Test service returned valid result");
	else if (res == -2)
		setOutput("SERVICE_NOTFOUND");
	else
		setOutput("Test service returned wrong result");
}

void QTestWindow::delService()
{
	int res = core->destroyServiceFunction(&name);
	if (!res)
		setOutput("Test service destroed");
	else if (res == -2)
		setOutput("SERVICE_NOTFOUND");
	else
		setOutput("Destroy service fail");
}

//-- Long test

static QMap <QLatin1String, THook*> qmapHooks;
static void* hookableEvent;

int testFunction(void* ho, int val)
{
	THook* p;
	p = (THook*)ho;
	return val;
}

int testFunction2(const QLatin1String* name2, int val)
{
	THook* p;
	qmapHooks[*name2] = (THook*)hookableEvent;
	p = qmapHooks[*name2];
	return val;
}

int testFunction3(const QLatin1String name2, int val)
{
	THook* p;
	qmapHooks[name2] = (THook*)hookableEvent;
	p = qmapHooks[name2];
	return val;
}

void QTestWindow::testPointerCalls()
{
	QTime time;
	time.start();
	for (int i = 0; i < 1000000; i++) {
		setBarValue(testFunction(hookableEvent, i));
	}
	setOutput(QString::number(time.elapsed()));
}

void QTestWindow::testNameCalls()
{
	QTime time;
	time.start();
	for (int i = 0; i < 1000000; i++) {
		setBarValue(testFunction2(&name, i));
	}
	setOutput(QString::number(time.elapsed()));
}

void QTestWindow::testQStringCalls()
{
	QTime time;
	time.start();
	for (int i = 0; i < 1000000; i++) {
		setBarValue(testFunction3(name, i));
	}
	setOutput(QString::number(time.elapsed()));
}

const QLatin1String	kCoreIsPluginLoaded	=	QLatin1String("Core/IsPluginLoaded");
void QTestWindow::simpleTest()
{
	QUuid uuid = v1->text();
	intptr_t id = reinterpret_cast<intptr_t>(&uuid);
	if (core->callService(&kCoreIsPluginLoaded, id, 0))
		setOutput("loaded");
	else
		setOutput("not loaded");
}

//-- Exit
void QTestWindow::buttonExit()
{
	core->callService(&kShutdown_service, 0, 0);
}

void QTestWindow::setOutput(QString text)
{
	output->setText(text);
	QApplication::processEvents();
}
