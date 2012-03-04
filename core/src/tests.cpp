#include "tests.h"

void hideMainWindow(uintptr_t wParam, uintptr_t lParam);

#include <QtSql>

QTestWindow* wii;

QTestWindow::QTestWindow()
{
	vis = false;

	this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	QVBoxLayout* layoutV = new QVBoxLayout(this);
	QHBoxLayout* layoutH = new QHBoxLayout(this);
	QVBoxLayout* layoutUtil = new QVBoxLayout(this);
	QVBoxLayout* layoutServ = new QVBoxLayout(this);
	QVBoxLayout* layoutHook = new QVBoxLayout(this);
	QVBoxLayout* layoutTest = new QVBoxLayout(this);
	layoutV->addLayout(layoutH);
	layoutH->addLayout(layoutUtil);
	layoutH->addLayout(layoutServ);
	layoutH->addLayout(layoutHook);
	layoutH->addLayout(layoutTest);

	QLabel* lbl = new QLabel(this);
	layoutUtil->addWidget(lbl);
	lbl->setText("Utils");
	lbl->setMaximumHeight(15);
	layoutUtil->setAlignment(lbl, Qt::AlignTop | Qt::AlignHCenter);

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
	button->setText("Test crypted DB");
	connect(button, SIGNAL(clicked()), this, SLOT(testDB()));
	layoutUtil->addWidget(button);

	layoutUtil->setAlignment(button, Qt::AlignTop);

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
	HookEvent(&TRAY_SINGLECLICK, (ELISEHOOK)hideMainWindow);

	wii = this;
	this->show();
	vis = true;

	QAction* action = new QAction("Call testQStringCalls", 0);
	connect(action, SIGNAL(triggered()), this, SLOT(testQStringCalls()));
	CallService(&TRAY_ADD_MENUITEM, (uintptr_t)action, 0);

	action = new QAction("Exit", 0);
	connect(action, SIGNAL(triggered()), this, SLOT(buttonExit()));
	CallService(&TRAY_ADD_MENUITEM, (uintptr_t)action, 0);
}

void hideMainWindow(uintptr_t wParam, uintptr_t lParam)
{
	if (wii->vis) {
		wii->hide();
		wii->vis = false;
	}
	else {
		wii->show();
		wii->vis = true;
	}
}

QTestWindow::~QTestWindow()
{

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

const QString testplugin_service = "TESTPLUGIN_SERVICE";
void QTestWindow::testNewPlugin()
{
	CallService(&testplugin_service, 0, 0);
}

void QTestWindow::testDB()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLCIPHER");

	if (!QFile::exists("test.db")) {
		db.setDatabaseName("test.db");
		if (!db.open()) {
			QMessageBox::critical(0, tr("Cannot open database"),
								  tr("Unable to establish a database connection.\n"
									 "This example needs SQLCipher support."),
								  QMessageBox::Cancel);
			return;
		}

		QSqlQuery query;
		query.exec("pragma key = '12345';");
		query.exec("create table person (id int primary key, "
				   "name varchar(20), address varchar(200), typeid int)");
		query.exec("insert into person values(1, 'Alice', "
				   "'<qt>123 Main Street<br/>Market Town</qt>', 101)");
		query.exec("insert into person values(2, 'Bob', "
				   "'<qt>PO Box 32<br/>Mail Handling Service"
				   "<br/>Service City</qt>', 102)");
		query.exec("insert into person values(3, 'Carol', "
				   "'<qt>The Lighthouse<br/>Remote Island</qt>', 103)");
		query.exec("insert into person values(4, 'Donald', "
				   "'<qt>47338 Park Avenue<br/>Big City</qt>', 101)");
		query.exec("insert into person values(5, 'Emma', "
				   "'<qt>Research Station<br/>Base Camp<br/>"
				   "Big Mountain</qt>', 103)");
		//! [Set up the main table]

		//! [Set up the address type table]
		query.exec("create table addresstype (id int, description varchar(20))");
		query.exec("insert into addresstype values(101, 'Home')");
		query.exec("insert into addresstype values(102, 'Work')");
		query.exec("insert into addresstype values(103, 'Other')");
	} else {
		db.setDatabaseName("test.db");
		if (!db.open()) {
			QMessageBox::critical(0, tr("Cannot open database"),
								  tr("Unable to establish a database connection.\n"
									 "This example needs SQLCipher support."),
								  QMessageBox::Cancel);
			return;
		}

		QSqlQuery query;
		query.exec("pragma key = '12345';");

		{//===-Test-===
			query.exec("select * from addresstype;");
			if (query.lastError().type() != QSqlError::NoError) {
				QMessageBox::critical(0, tr("Cannot open database"),
									  tr("%1").arg(query.lastError().text()), QMessageBox::Cancel);
				return;
			}
		}
	}
/*
	model = new QSqlRelationalTableModel(this);
	model->setTable("person");
	model->setEditStrategy(QSqlTableModel::OnManualSubmit);

	typeIndex = model->fieldIndex("typeid");

	model->setRelation(typeIndex,
					   QSqlRelation("addresstype", "id", "description"));
	model->select();*/
}


static QString name = "TEST_SERVISE";
static QString hkevName = "TEST_HOOKABLE_EVENT";
static THook hookEv;

//-- Hooks

int testHook(uintptr_t wParam, uintptr_t lParam)
{
	QMessageBox qmes;
	qmes.setText("Test hook called! lParam is " + QString::number(lParam));
	qmes.exec();
	return 0;
}

void QTestWindow::createHookblEvent()
{
	if (!CreateHookableEvent(&hkevName))
		setOutput("Hookable event created");
	else
		setOutput("Create hookable event FAIL!");
}

void QTestWindow::hookEvent()
{
	hookEv.num = HookEvent(&hkevName, (ELISEHOOK)testHook);
	hookEv.name = &hkevName;
	if (hookEv.num != -1)
		setOutput("Hook event success, num is " + QString::number(hookEv.num));
	else
		setOutput("Hook event FAILED!");
}

void QTestWindow::notifyEventHooks()
{
	int ret = NotifyEventHooks(&hkevName, 0, 137);
	if (ret)
		setOutput("Notify event FAILED!");
	else
		setOutput("Notify event success!");
}

void QTestWindow::unhookEvent()
{
	int ret = UnhookEvent(hookEv);
	if (ret != -1)
		setOutput("Unhook event success, num was " + QString::number(hookEv.num));
	else
		setOutput("Unhook event FAILED!");
}

void QTestWindow::deleteHokableEvent()
{
	if (!DestroyHookableEvent(&hkevName))
		setOutput("Hookable event destroed");
	else
		setOutput("Destroy hookable event FAIL!");
}

//-- Services
int testService(uintptr_t wParam, uintptr_t lParam)
{
	QMessageBox qmes;
	qmes.setText("Test service working");
	qmes.exec();
	return wParam;
}

void QTestWindow::createService()
{
	if (!CreateServiceFunction(&name, (ELISESERVICE)testService))
		setOutput("Service created");
	else
		setOutput("Create service FAIL!");
}

void QTestWindow::checkService()
{
	if (ServiceExists(&name))
		setOutput("Test service exists");
	else
		setOutput("Test service not found");
}

void QTestWindow::testtService()
{
	int res = CallService(&name, 111, 0);
	if (res == 111)
		setOutput("Test service returned valid result");
	else if (res == SERVICE_NOTFOUND)
		setOutput("SERVICE_NOTFOUND");
	else
		setOutput("Test service returned wrong result");
}

void QTestWindow::delService()
{
	int res = DestroyServiceFunction(&name);
	if (!res)
		setOutput("Test service destroed");
	else if (res == SERVICE_NOTFOUND)
		setOutput("SERVICE_NOTFOUND");
	else
		setOutput("Destroy service fail");
}

//-- Long test

static QMap <QString, THook*> qmapHooks;
static void* hookableEvent;

int testFunction(void* ho, int val)
{
	THook* p;
	p = (THook*)ho;
	return val;
}

int testFunction2(const QString* name2, int val)
{
	THook* p;
	qmapHooks[*name2] = (THook*)hookableEvent;
	p = qmapHooks[*name2];
	return val;
}

int testFunction3(const QString name2, int val)
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

void QTestWindow::simpleTest()
{

}

//-- Exit
void QTestWindow::buttonExit()
{
	CallService(&SHUTDOWN_SERVICE, 0, 0);
}

void QTestWindow::setOutput(QString text)
{
	output->setText(text);
	QApplication::processEvents();
}
