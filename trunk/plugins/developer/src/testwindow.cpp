#include "testwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include "../../../api/e_database.h"

const QLatin1String	g_kCoreShutdown_service		=	QLatin1String(__Core_Shutdown_service);
const QLatin1String	g_kCoreChangeProfile_service	=	QLatin1String(__Core_ChangeProfile_service);
const QLatin1String	kTrayAddMenuItem_service	=	QLatin1String(__Tray_AddMenuItem_service);
const QLatin1String	kTraySetIcon_service		=	QLatin1String(__Tray_SetIcon_service);
const QLatin1String	kTraySingleClick_event	=	QLatin1String(__Tray_SingleClick_event);
const QLatin1String	g_kDBWriteSetting_service	=	QLatin1String(__DB_WriteSetting_service);
const QLatin1String	g_kDBReadSetting_service	=	QLatin1String(__DB_ReadSetting_service);
const QLatin1String	g_kDBDellSetting_service	=	QLatin1String(__DB_DellSetting_service);
const QLatin1String	kOptionsShow_service	=	QLatin1String(__Options_Show_service);
const QLatin1String	kClistShow_service	=	QLatin1String(__CList_Show_service);
const QLatin1String	kClistHide_service	=	QLatin1String(__CList_Hide_service);

TestWindow* mainWindow;

TestWindow::TestWindow() :
	QWidget(NULL)
{
	this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	QVBoxLayout* layoutV = new QVBoxLayout(this);
	debugOutput_ = new QTextEdit(this);
	layoutV->addWidget(debugOutput_);
	debugOutput_->setMinimumHeight(200);
	debugOutput_->setReadOnly(true);

	QHBoxLayout* layoutH = new QHBoxLayout(this);
	QVBoxLayout* layoutUtil = new QVBoxLayout(this);
	QVBoxLayout* layoutDB = new QVBoxLayout(this);
	QVBoxLayout* layoutServ = new QVBoxLayout(this);
	QVBoxLayout* layoutHook = new QVBoxLayout(this);
	//QVBoxLayout* layoutTest = new QVBoxLayout(this);
	layoutV->addLayout(layoutH);
	layoutH->addLayout(layoutUtil);
	layoutH->addLayout(layoutDB);
	layoutH->addLayout(layoutServ);
	layoutH->addLayout(layoutHook);
	//layoutH->addLayout(layoutTest);
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

	bar = new QProgressBar(this);
	layoutV->addWidget(bar);
	//bar->setRange(0, 1000000);

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
	connect(button, &QPushButton::clicked, this, &TestWindow::uuidCreate);
	layoutUtil->addWidget(button);
	button->setMinimumWidth(100);

	button = new QPushButton(this);
	button->setText("Options");
	connect(button, &QPushButton::clicked, this, &TestWindow::showOptions);
	layoutUtil->addWidget(button);

	button = new QPushButton(this);
	button->setText("Set icon");
	connect(button, &QPushButton::clicked, this, &TestWindow::setTrayIcon);
	layoutUtil->addWidget(button);

	button = new QPushButton(this);
	button->setText("Change account");
	connect(button, &QPushButton::clicked, this, &TestWindow::changeAcc);
	layoutUtil->addWidget(button);

	layoutUtil->setAlignment(button, Qt::AlignTop);

	//-- Database
	button = new QPushButton(this);
	button->setText("Save setting");
	connect(button, &QPushButton::clicked, this, &TestWindow::saveSetting);
	layoutDB->addWidget(button);
	button->setMinimumWidth(100);

	button = new QPushButton(this);
	button->setText("Read setting");
	connect(button, &QPushButton::clicked, this, &TestWindow::readSetting);
	layoutDB->addWidget(button);

	button = new QPushButton(this);
	button->setText("Delete setting");
	connect(button, &QPushButton::clicked, this, &TestWindow::delSetting);
	layoutDB->addWidget(button);

	layoutDB->setAlignment(button, Qt::AlignTop);

	//-- Services
	button = new QPushButton(this);
	button->setText("Add service");
	connect(button, &QPushButton::clicked, this, &TestWindow::createService);
	layoutServ->addWidget(button);
	button->setMinimumWidth(100);

	button = new QPushButton(this);
	button->setText("Check service");
	connect(button, &QPushButton::clicked, this, &TestWindow::checkService);
	layoutServ->addWidget(button);

	button = new QPushButton(this);
	button->setText("Test service");
	connect(button, &QPushButton::clicked, this, &TestWindow::testtService);
	layoutServ->addWidget(button);

	button = new QPushButton(this);
	button->setText("Delete service");
	connect(button, &QPushButton::clicked, this, &TestWindow::delService);
	layoutServ->addWidget(button);

	layoutServ->setAlignment(button, Qt::AlignTop);

	//-- Hooks
	button = new QPushButton(this);
	button->setText("Create hookable event");
	connect(button, &QPushButton::clicked, this, &TestWindow::createHookblEvent);
	layoutHook->addWidget(button);
	button->setMinimumWidth(100);

	button = new QPushButton(this);
	button->setText("Hook event");
	connect(button, &QPushButton::clicked, this, &TestWindow::hookEvent);
	layoutHook->addWidget(button);

	button = new QPushButton(this);
	button->setText("Notify event hooks");
	connect(button, &QPushButton::clicked, this, &TestWindow::notifyEventHooks);
	layoutHook->addWidget(button);

	button = new QPushButton(this);
	button->setText("Unhook event");
	connect(button, &QPushButton::clicked, this, &TestWindow::unhookEvent);
	layoutHook->addWidget(button);

	button = new QPushButton(this);
	button->setText("Delete hookable event");
	connect(button, &QPushButton::clicked, this, &TestWindow::deleteHokableEvent);
	layoutHook->addWidget(button);

	layoutHook->setAlignment(button, Qt::AlignTop);

	//-- Exit
	button = new QPushButton(this);
	button->setText("Exit");
	connect(button, &QPushButton::clicked, this, &TestWindow::buttonExit);
	layoutV->addWidget(button);
	layoutV->setAlignment(button, Qt::AlignBottom);

	//-- Work with tray
	g_core->hookEvent(&kTraySingleClick_event, &TestWindow::hideMainWindow);

	this->show();
	vis = true;
}

intptr_t TestWindow::showCList(intptr_t, intptr_t)
{
	if (!mainWindow)
		mainWindow = new TestWindow();
	return 0;
}

intptr_t TestWindow::hideCList(intptr_t, intptr_t)
{
	delete mainWindow;
	mainWindow = 0;
	return 0;
}

int TestWindow::hideMainWindow(intptr_t, intptr_t)
{
	if (mainWindow->vis) {
		mainWindow->hide();
		mainWindow->vis = false;
	}
	else {
		mainWindow->show();
		mainWindow->vis = true;
	}

	return 0;
}

TestWindow::~TestWindow()
{
	g_core->unhookEvent(&kTraySingleClick_event, &TestWindow::hideMainWindow);
}

//-- Utils

void TestWindow::uuidCreate()
{
	setOutput(QUuid::createUuid().toString());
}

void TestWindow::changeAcc()
{
	if (g_core->callService(&g_kCoreChangeProfile_service, 0, 0) == -2)
		QMessageBox::critical(this, "Error", "Service not found.", QMessageBox::Ok);
	delete mainWindow;
	mainWindow = 0;
}

void TestWindow::showOptions()
{
	g_core->callService(&kOptionsShow_service, 0, 0);
}

void TestWindow::setTrayIcon()
{
	QString filename = QFileDialog::getOpenFileName(this, "Open file", "", "SVG (*.svg);; Files (*.*)");
	//QMessageBox::critical(this, "Debug", "2", QMessageBox::Ok);
	QIcon* icon = new QIcon(filename);
	g_core->callService(&kTraySetIcon_service, (uintptr_t)icon, 0);
	delete icon;
}

//-- Database

void TestWindow::saveSetting()
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
		case __Int_Type:
			set->var->intValue = v3->text().toInt();
			break;
		case __Real_Type:
			set->var->realValue = v3->text().toDouble();
			break;
		case __Text_Type:
		{
			value = v3->text();
			set->var->textValue = &value;
		}
			break;
		case __Blob_Type:
		{
			value = v3->text();
			set->var->textValue = &value;
		}
			break;
	}

	g_core->callService(&g_kDBWriteSetting_service, reinterpret_cast<intptr_t>(set), 0);

	delete set->var;
	delete set;
}

void TestWindow::readSetting()
{
	QString module = QString(v1->text().toLatin1());
	QString setting = QString(v2->text().toLatin1());
	Setting* set = new Setting;
	set->contact = 0;
	set->qsModule = &module;
	set->qsSetting = &setting;
	set->var = new DBVariant;
	set->var->type = (unsigned char)v4->text().toInt();
	if (set->var->type == __Text_Type)
		set->var->textValue = new QString;
	else if (set->var->type == __Blob_Type)
		set->var->blobValue = new QByteArray;
	if (!g_core->callService(&g_kDBReadSetting_service, reinterpret_cast<intptr_t>(set), 0)) {
		switch (set->var->type) {
			case __Int_Type:
				setOutput(QString::number(set->var->intValue));
				break;
			case __Real_Type:
				setOutput(QString::number(set->var->realValue));
				break;
			case __Text_Type:
				setOutput(*set->var->textValue);
				break;
			case __Blob_Type:
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

void TestWindow::delSetting()
{
	QString module = QString(v1->text().toLatin1());
	QString setting = QString(v2->text().toLatin1());
	Setting* set = new Setting;
	set->contact = 0;
	set->qsModule = &module;
	set->qsSetting = &setting;
	set->var = new DBVariant;
	set->var->type = (unsigned char)v4->text().toInt();

	if (!g_core->callService(&g_kDBDellSetting_service, reinterpret_cast<intptr_t>(set), 0))
		setOutput("Setting deleted");
	else
		setOutput("Error");

	delete set->var;
	delete set;
}

static QLatin1String name = QLatin1String("TEST_SERVISE");
static QLatin1String hkevName = QLatin1String("TEST_HOOKABLE_EVENT");

//-- Hooks

int testHook(intptr_t, intptr_t lParam)
{
	QMessageBox qmes;
	qmes.setText("Test hook called! lParam is " + QString::number(lParam));
	qmes.exec();
	return 0;
}

void TestWindow::createHookblEvent()
{
	if (!g_core->createHookableEvent(&hkevName))
		setOutput("Hookable event created");
	else
		setOutput("Create hookable event FAIL!");
}

void TestWindow::hookEvent()
{
	if (!g_core->hookEvent(&hkevName, &testHook))
		setOutput("Hook event success");
	else
		setOutput("Hook event FAILED!");
}

void TestWindow::notifyEventHooks()
{
	int ret = g_core->notifyEventHooks(&hkevName, 0, 137);
	if (ret)
		setOutput("Notify event FAILED!");
	else
		setOutput("Notify event success!");
}

void TestWindow::unhookEvent()
{
	if (!g_core->unhookEvent(&hkevName, &testHook))
		setOutput("Unhook event success");
	else
		setOutput("Unhook event FAILED!");
}

void TestWindow::deleteHokableEvent()
{
	if (!g_core->destroyHookableEvent(&hkevName))
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

void TestWindow::createService()
{
	if (!g_core->createServiceFunction(&name, &testService))
		setOutput("Service created");
	else
		setOutput("Create service FAIL!");
}

void TestWindow::checkService()
{
	if (g_core->serviceExists(&name))
		setOutput("Test service exists");
	else
		setOutput("Test service not found");
}

void TestWindow::testtService()
{
	int res = g_core->callService(&name, 111, 0);
	if (res == 111)
		setOutput("Test service returned valid result");
	else if (res == -2 || res == -1)
		setOutput("SERVICE_NOTFOUND");
	else
		setOutput("Test service returned wrong result");
}

void TestWindow::delService()
{
	int res = g_core->destroyServiceFunction(&name);
	if (!res)
		setOutput("Test service destroed");
	else if (res == -2)
		setOutput("SERVICE_NOTFOUND");
	else
		setOutput("Destroy service fail");
}

