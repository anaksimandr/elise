
#include "../commonheaders.h"


AccountManager::AccountManager(QMap<QString, IDBPlugin*>* dbPlugins)
{
	//-- Check it before call this function
	/*if (dbPlugins == 0 || dbPlugins->count() ==0) {
		QMessageBox::critical(0, "Error",
							  "No one db plugin was found.",
							  QMessageBox::Cancel);
	}*/
	avdbPlugins = dbPlugins;
	listAccounts = 0;
	this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);

	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	QHBoxLayout* hboxMain = new QHBoxLayout(this);

	//-- Right staff
	QGroupBox* groupDB = new QGroupBox("DB plugins", this);
	QVBoxLayout* vboxDB = new QVBoxLayout(this);
	groupDB->setLayout(vboxDB);

	//-- Left staff
	QGroupBox* groupACC = new QGroupBox("Account", this);
	QVBoxLayout* vboxACC = new QVBoxLayout(this);
	groupACC->setLayout(vboxACC);

	hboxMain->addWidget(groupDB);
	hboxMain->addWidget(groupACC);

	groupDB->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	hboxMain->setAlignment(groupDB, Qt::AlignTop);

	//-- Account
	cmbAccount = new QComboBox(this);
	cmbAccount->setMinimumSize(120, 18);
	cmbAccount->setEditable(true);
	vboxACC->addWidget(cmbAccount);
	connect(cmbAccount, SIGNAL(currentIndexChanged(const QString&)),
			this, SLOT(loadAccountDetails(const QString&)));

	//-- Password
	lePassword = new QLineEdit(this);
	lePassword->setPlaceholderText("password");
	lePassword->setEchoMode(QLineEdit::Password);
	vboxACC->addWidget(lePassword);
	connect(lePassword, SIGNAL(returnPressed()), this, SLOT(ok()));

	//-- Checkboxes, don't ask password on login
	cbSavePassword = new QCheckBox(this);
	cbSavePassword->setText("Save password");
	vboxACC->addWidget(cbSavePassword);
	connect(cbSavePassword, SIGNAL(stateChanged(int)), this, SLOT(setCBEnable(int)));

	//-- Use as default account
	cbDefaultAcc = new QCheckBox(this);
	cbDefaultAcc->setText("Default account");
	cbDefaultAcc->setEnabled(false);
	vboxACC->addWidget(cbDefaultAcc);

	//-- OK, load profile
	QPushButton* button = new QPushButton(this);
	button->setText("OK");
	connect(button, SIGNAL(clicked()), this, SLOT(ok()));
	vboxACC->addWidget(button);

	//-- Creating list of dbPlugins
	listDBPlugins = new QComboBox(this);
	listDBPlugins->setMinimumSize(120, 18);
	vboxDB->addWidget(listDBPlugins);
	QMapIterator<QString, IDBPlugin*> iter(*avdbPlugins);
	while (iter.hasNext()) {
		iter.next();
		listDBPlugins->addItem(iter.key());
	}
	connect(listDBPlugins, SIGNAL(currentIndexChanged(const QString&)),
			this, SLOT(loadAccountsList(const QString&)));

	//-- Create account button
	button = new QPushButton(this);
	button->setText("Create");
	connect(button, SIGNAL(clicked()), this, SLOT(createAccount()));
	vboxDB->addWidget(button);

	//-- Import account button
	button = new QPushButton(this);
	button->setText("Import");
	connect(button, SIGNAL(clicked()), this, SLOT(importAccount()));
	vboxDB->addWidget(button);
	vboxDB->setAlignment(button, Qt::AlignTop);

	//-- Cancel
	button = new QPushButton(this);
	button->setText("Cancel");
	connect(button, SIGNAL(clicked()), this, SLOT(abort()));
	button->resize(120, 23);
	button->move(21, 132);

	//listDBPlugins->setFocus();
	lePassword->setFocus();

	hboxMain->setSizeConstraint(QLayout::SetFixedSize);

	//-- First time initialize cmbAccount programmatically
	loadAccountsList(listDBPlugins->currentText());
	loadAccountDetails(cmbAccount->currentText());
}

AccountManager::~AccountManager()
{

}

void AccountManager::setCBEnable(int state)
{
	if (!state) {
		cbDefaultAcc->setEnabled(false);
		cbDefaultAcc->setCheckState(Qt::Unchecked);
	}
	else
		cbDefaultAcc->setEnabled(true);
}

void AccountManager::loadAccountsList(const QString& text)
{
	cmbAccount->clear();
	//-- Destroy old list if exists
	if (listAccounts != 0) {
		QMapIterator<QString, ACCOUNT*> iter(*listAccounts);
		while (iter.hasNext()) {
			iter.next();
			delete iter.value();
		}
		listAccounts->~QMap();
	}
	//-- Getting list of accounts from plugin
	listAccounts = avdbPlugins->value(text)->GetAccounts();
	if (listAccounts != 0) {
		if (listAccounts->count() != 0) {
			QMapIterator<QString, ACCOUNT*> iter(*listAccounts);
			while (iter.hasNext()) {
				iter.next();
				cmbAccount->addItem(iter.key());
				if (iter.value()->defaultAccount)
					defaultAcc = iter.key();
			}
		}
		else
			//-- No one account was found
			cmbAccount->addItem("Create account first");
	}
	else
		cmbAccount->addItem("Internal plugin error");
}

void AccountManager::loadAccountDetails(const QString& name)
{
	ACCOUNT* p = listAccounts->value(name);
	if ( p != 0) {
		int tmp = 0;
		if (p->savePassword)
			tmp = 2;
		cbSavePassword->setCheckState((Qt::CheckState)tmp);
		tmp = 0;
		if (p->defaultAccount)
			tmp = 2;
		cbDefaultAcc->setCheckState((Qt::CheckState)tmp);
		if (p->savePassword)
			lePassword->setText(p->password);
		else
			lePassword->setText("");
	}
}

int AccountManager::loadDefault()
{
	if (defaultAcc.isEmpty())
		//-- No default acc found
		return 1;

	cmbAccount->setCurrentIndex(cmbAccount->findText(defaultAcc));
	loadAccountDetails(defaultAcc);

	IDBPlugin* dbPlugin = avdbPlugins->value(listDBPlugins->currentText());
	//-- Try to login
	if (dbPlugin->Login(cmbAccount->currentText(), lePassword->text(),
			cbSavePassword->isChecked(), cbDefaultAcc->isChecked()))
	{
		QMessageBox::critical(0, "Login error",
					"Failed to default login.\nAccount does not exist or password does not match.",
					QMessageBox::Cancel);
		return 1;
	}
	//-- Destroy list if exists
	if (listAccounts != 0) {
		QMapIterator<QString, ACCOUNT*> iter(*listAccounts);
		while (iter.hasNext()) {
			iter.next();
			delete iter.value();
		}
		listAccounts->~QMap();
	}

	return PluginLoader::loadDBPlugin(listDBPlugins->currentText(), dbPlugin);
}

void AccountManager::abort()
{
	//-- Destroy accounts list if exists
	if (listAccounts != 0) {
		QMapIterator<QString, ACCOUNT*> iter(*listAccounts);
		ACCOUNT* p;
		while (iter.hasNext()) {
			iter.next();
			p = iter.value();
			delete p;
		}
		listAccounts->~QMap();
	}

	//-- Abort Elise loading and exit
	done(1);
}

void AccountManager::ok()
{
	IDBPlugin* dbPlugin = avdbPlugins->value(listDBPlugins->currentText());
	//-- Try to login
	if (dbPlugin->Login(cmbAccount->currentText(), lePassword->text(),
			cbSavePassword->isChecked(), cbDefaultAcc->isChecked()))
	{
		QMessageBox::critical(0, "Login error",
							  "Failed to login.\nAccount does not exist or password does not match.",
							  QMessageBox::Cancel);
		return;
	}
	//-- if success then try fully load the plugin
	//-- Destroy accounts list if exists
	if (listAccounts != 0) {
		QMapIterator<QString, ACCOUNT*> iter(*listAccounts);
		while (iter.hasNext()) {
			iter.next();
			delete iter.value();
		}
		listAccounts->~QMap();
	}
	done(PluginLoader::loadDBPlugin(listDBPlugins->currentText(), dbPlugin));
}

void AccountManager::createAccount()
{
	IDBPlugin* dbPlugin = avdbPlugins->value(listDBPlugins->currentText());
	if (dbPlugin->CreateAccount(cmbAccount->currentText(), lePassword->text()))
		QMessageBox::critical(0, "Create account error",
				"Failed to create account.\nAccount already exist or another internal plugin error.",
				QMessageBox::Cancel);
	else {
		QMessageBox::information(0, "Information",
								 "Account created successfully.",
								 QMessageBox::Ok);
		loadAccountsList(listDBPlugins->currentText());
	}
}

void AccountManager::importAccount()
{
	QMessageBox::critical(0, "Error",
						  "Sorry, this function is not supported yet.",
						  QMessageBox::Cancel);
}


/*void AccountManager::initInputs()
{
	if (!fieldsInited) {
		if (!dialogInited) {
			account->setText("login");
			password->setText("password");
			dialogInited = true;
		}
		else {
			account->setText("");
			password->setText("");
			password->setEchoMode(QLineEdit::Password);
			fieldsInited = true;
		}
	}
}*/
/*
bool FocusMonitor::eventFilter(QObject* obj, QEvent* event)
{
	if (event->type() == QEvent::FocusIn) {
		parent->initInputs();
		return false;
	} else {
		//-- Standard event processing
		return QObject::eventFilter(obj, event);
	}
}

FocusMonitor::FocusMonitor(AccountManager *parent)
{
	this->parent = parent;
}*/

