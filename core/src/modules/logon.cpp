
#include "../commonheaders.h"

AccountManager::AccountManager(QMap<QString, IDBPlugin *>* dbPlugins)
{
	this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	//this->setWindowModality(Qt::ApplicationModal);

	QGridLayout* layout = new QGridLayout(this);

	//-- Account
	QLineEdit* account = new QLineEdit(this);
	layout->addWidget(account, 0, 1, 1, 3);
	//account->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	account->resize(100, 15);

	//-- Password
	QLineEdit* password = new QLineEdit(this);
	layout->addWidget(password, 1, 1, 1, 3);
	//password->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	password->setEchoMode(QLineEdit::Password);
	password->resize(100, 15);

	//-- OK, load profile
	QPushButton* button = new QPushButton(this);
	button->setText("OK");
	//connect(button, SIGNAL(clicked()), this, SLOT(testNewPlugin()));
	layout->addWidget(button, 2, 1, 1, 3);
	//button->resize(60, 15);

	//-- Import account button
	button = new QPushButton(this);
	button->setText("Import");
	//connect(button, SIGNAL(clicked()), this, SLOT(uuidCreate()));
	layout->addWidget(button, 3, 0, 1, 2);
	//button->resize(60, 15);

	//-- Create account button
	button = new QPushButton(this);
	button->setText("Create");
	//connect(button, SIGNAL(clicked()), this, SLOT(testNewPlugin()));
	layout->addWidget(button, 3, 2, 1, 1);
	//button->resize(60, 15);

	//-- Cancel
	button = new QPushButton(this);
	button->setText("Cancel");
	connect(button, SIGNAL(clicked()), this, SLOT(abort()));
	layout->addWidget(button, 3, 3, 1, 2);
	//button->resize(60, 15);

	this->resize(200, 100);

}

AccountManager::~AccountManager()
{

}

void AccountManager::abort()
{
	//-- Abort Elise loading and exit
	done(1);
}
