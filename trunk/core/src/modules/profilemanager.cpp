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

#include "profilemanager.h"
#include "pluginloader/pluginloader.h"


ProfileManager::ProfileManager(const QMap<QString, IDBPlugin*>* availableDBPlugins)
{
	DBPlugins = availableDBPlugins;
	profiles = 0;
	this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);

	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	QHBoxLayout* hboxMain = new QHBoxLayout(this);

	//-- Right staff
	QGroupBox* groupDB = new QGroupBox(QStringLiteral("DB plugins"), this);
	QVBoxLayout* vboxDB = new QVBoxLayout(this);
	groupDB->setLayout(vboxDB);

	//-- Left staff
	QGroupBox* groupACC = new QGroupBox(QStringLiteral("Profile"), this);
	QVBoxLayout* vboxACC = new QVBoxLayout(this);
	groupACC->setLayout(vboxACC);

	hboxMain->addWidget(groupDB);
	hboxMain->addWidget(groupACC);

	groupDB->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	hboxMain->setAlignment(groupDB, Qt::AlignTop);

	//-- Profile
	cmbProfiles = new QComboBox(this);
	cmbProfiles->setMinimumSize(120, 18);
	cmbProfiles->setEditable(true);
	vboxACC->addWidget(cmbProfiles);
	connect(cmbProfiles,
			static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
			this,
			&ProfileManager::loadProfileDetails);

	//-- Password
	lePassword = new QLineEdit(this);
	lePassword->setPlaceholderText(QStringLiteral("password"));
	lePassword->setEchoMode(QLineEdit::Password);
	vboxACC->addWidget(lePassword);
	connect (lePassword, &QLineEdit::returnPressed, this, &ProfileManager::ok);

	//-- Checkboxes, don't ask password on login
	cbSavePassword = new QCheckBox(this);
	cbSavePassword->setText(QStringLiteral("Save password"));
	vboxACC->addWidget(cbSavePassword);
	connect(cbSavePassword, &QCheckBox::stateChanged, this, &ProfileManager::setCBEnable);

	//-- Use as default profile
	cbDefaultProfile = new QCheckBox(this);
	cbDefaultProfile->setText("Default profile");
	cbDefaultProfile->setEnabled(false);
	vboxACC->addWidget(cbDefaultProfile);

	//-- OK, load profile
	QPushButton* button = new QPushButton(this);
	button->setText(QStringLiteral("OK"));
	connect(button, &QPushButton::clicked, this, &ProfileManager::ok);
	vboxACC->addWidget(button);

	//-- Creating list of dbPlugins
	cmbDBPlugins = new QComboBox(this);
	cmbDBPlugins->setMinimumSize(120, 18);
	vboxDB->addWidget(cmbDBPlugins);
	QMapIterator<QString, IDBPlugin*> iter(*DBPlugins);
	while (iter.hasNext()) {
		iter.next();
		cmbDBPlugins->addItem(iter.key());
	}
	connect(cmbDBPlugins,
			static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
			this,
			&ProfileManager::loadProfiles);

	//-- Create profile button
	button = new QPushButton(this);
	button->setText(QStringLiteral("Create"));
	connect(button, &QPushButton::clicked, this, &ProfileManager::createProfile);
	vboxDB->addWidget(button);

	//-- Import profile button
	button = new QPushButton(this);
	button->setText(QStringLiteral("Import"));
	connect(button, &QPushButton::clicked, this, &ProfileManager::importProfile);
	vboxDB->addWidget(button);
	vboxDB->setAlignment(button, Qt::AlignTop);

	//-- Cancel
	button = new QPushButton(this);
	button->setText(QStringLiteral("Cancel"));
	connect(button, &QPushButton::clicked, this, &ProfileManager::abort);
	button->resize(120, 23);
	button->move(21, 132);

	//cmbDBPlugins->setFocus();
	lePassword->setFocus();

	hboxMain->setSizeConstraint(QLayout::SetFixedSize);

	//-- First time initialize cmbAccount programmatically
	loadProfiles(cmbDBPlugins->currentText());
	loadProfileDetails(cmbProfiles->currentText());
}

ProfileManager::~ProfileManager()
{
	//delete DBPlugins;
	//-- Destroy profiles list if exists
	if (profiles != 0) {
		QMapIterator<QString, Profile*> iter(*profiles);
		while (iter.hasNext()) {
			iter.next();
			delete iter.value();
		}
		delete profiles;
	}
}

void ProfileManager::setCBEnable(int state)
{
	if (!state) {
		cbDefaultProfile->setEnabled(false);
		cbDefaultProfile->setCheckState(Qt::Unchecked);
	}
	else
		cbDefaultProfile->setEnabled(true);
}

void ProfileManager::loadProfiles(const QString& text)
{
	cmbProfiles->clear();
	//-- Destroy old list if exists
	if (profiles != 0) {
		QMapIterator<QString, Profile*> iter(*profiles);
		while (iter.hasNext()) {
			iter.next();
			delete iter.value();
		}
		delete profiles;
	}
	//-- Getting list of profiles from plugin
	profiles = DBPlugins->value(text)->GetProfiles();
	if (profiles != 0) {
		if (profiles->count() != 0) {
			QMapIterator<QString, Profile*> iter(*profiles);
			while (iter.hasNext()) {
				iter.next();
				cmbProfiles->addItem(iter.key());
				if (iter.value()->defaultProfile)
					qsDefaultProfile = iter.key();
			}
		}
		else
			//-- No one profile was found
			cmbProfiles->addItem(QStringLiteral("Create profile first"));
	}
	else
		cmbProfiles->addItem(QStringLiteral("Internal plugin error"));
}

void ProfileManager::loadProfileDetails(const QString& name)
{
	if (profiles->contains(name)) {
		Profile* p = profiles->value(name);
		//QMessageBox::critical(0, QStringLiteral("Debug"), name, QMessageBox::Cancel);

		if (p->savePassword) {
			lePassword->setText(p->password);
			cbSavePassword->setCheckState(Qt::Checked);
		}
		else {
			lePassword->setText("");
			cbSavePassword->setCheckState(Qt::Unchecked);
		}
		if (p->defaultProfile)
			cbDefaultProfile->setCheckState(Qt::Checked);
		else
			cbDefaultProfile->setCheckState(Qt::Unchecked);
		//delete p;
	}
}

int ProfileManager::loadDefaultProfile()
{
	if (qsDefaultProfile.isEmpty())
		//-- No default profile found
		return 1;

	cmbProfiles->setCurrentIndex(cmbProfiles->findText(qsDefaultProfile));
	loadProfileDetails(qsDefaultProfile);

	IDBPlugin* dbPlugin = DBPlugins->value(cmbDBPlugins->currentText());
	//-- Try to login
	if (dbPlugin->Login(cmbProfiles->currentText(), lePassword->text(),
			cbSavePassword->isChecked(), cbDefaultProfile->isChecked()))
	{
		QMessageBox::critical(0, QStringLiteral("Login error"),
					QStringLiteral("Failed to default login.\nAccount does not exist or password does not match."),
					QMessageBox::Cancel);
		return 1;
	}

	return PluginLoader::loadDBPlugin(cmbDBPlugins->currentText(), DBPlugins);
}

void ProfileManager::abort()
{
	//-- Abort Elise loading and exit
	done(1);
}

void ProfileManager::ok()
{
	IDBPlugin* dbPlugin = DBPlugins->value(cmbDBPlugins->currentText());
	//-- Try to login
	if (dbPlugin->Login(cmbProfiles->currentText(), lePassword->text(),
			cbSavePassword->isChecked(), cbDefaultProfile->isChecked()))
	{
		QMessageBox::critical(0, QStringLiteral("Login error"),
							  QStringLiteral("Failed to login.\nProfile does not exist or assword does not match."),
							  QMessageBox::Cancel);
		return;
	}

	//-- if success then try fully load the plugin	
	done(PluginLoader::loadDBPlugin(cmbDBPlugins->currentText(), DBPlugins));
}

void ProfileManager::createProfile()
{
	IDBPlugin* dbPlugin = DBPlugins->value(cmbDBPlugins->currentText());
	if (dbPlugin->CreateProfile(cmbProfiles->currentText(), lePassword->text()))
		QMessageBox::critical(0, QStringLiteral("Create profile error"),
				QStringLiteral("Failed to create profile.\nProfile already exist or another internal plugin error."),
				QMessageBox::Cancel);
	else {
		QMessageBox::information(0, QStringLiteral("Information"),
								 QStringLiteral("Profile created successfully."),
								 QMessageBox::Ok);
		loadProfiles(cmbDBPlugins->currentText());
	}
}

void ProfileManager::importProfile()
{
	QMessageBox::critical(0, QStringLiteral("Error"),
						  QStringLiteral("Sorry, this function is not supported yet."),
						  QMessageBox::Cancel);
}
