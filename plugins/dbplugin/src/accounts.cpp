
#include "commonheaders.h"

AccountManager::AccountManager()
{
	this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	this->setWindowModality(Qt::ApplicationModal);

	QVBoxLayout* layoutV = new QVBoxLayout(this);
	QHBoxLayout* layoutH = new QHBoxLayout(this);

	//-- Account
	QTextEdit* account = new QTextEdit();
	layoutV->addWidget(account);
	//account->setMaximumHeight(15);
	account->resize(100, 15);
	//-- Password
	QTextEdit* password = new QTextEdit();
	layoutV->addWidget(password);
	//password->setMaximumHeight(15);
	password->resize(100, 15);

	layoutV->addLayout(layoutH);

	//-- Import account button
	QPushButton* button = new QPushButton(this);
	button->setText("Import");
	//connect(button, SIGNAL(clicked()), this, SLOT(uuidCreate()));
	layoutH->addWidget(button);
	button->resize(60, 15);
	//-- Create account button
	button = new QPushButton(this);
	button->setText("Create");
	//connect(button, SIGNAL(clicked()), this, SLOT(testNewPlugin()));
	layoutH->addWidget(button);
	button->resize(60, 15);
	//-- OK, load profile
	button = new QPushButton(this);
	button->setText("OK");
	//connect(button, SIGNAL(clicked()), this, SLOT(testNewPlugin()));
	layoutH->addWidget(button);
	button->resize(60, 15);
	//-- Cancel
	button = new QPushButton(this);
	button->setText("Cancel");
	connect(button, SIGNAL(clicked()), this, SLOT(abort()));
	layoutH->addWidget(button);
	button->resize(60, 15);

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
