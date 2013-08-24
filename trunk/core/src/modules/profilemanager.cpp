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
#include "folders/folders.h"

#define DataSteamVer (QDataStream::Qt_5_1)

ProfileManager::ProfileManager()
{
	profiles_ = 0;
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
	cmbProfiles_ = new QComboBox(this);
	cmbProfiles_->setMinimumSize(120, 18);
	cmbProfiles_->setEditable(true);
	vboxACC->addWidget(cmbProfiles_);
	connect(cmbProfiles_,
			static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
			this,
			&ProfileManager::loadProfileDetails);

	//-- Password
	lePassword_ = new QLineEdit(this);
	lePassword_->setPlaceholderText(QStringLiteral("password"));
	lePassword_->setEchoMode(QLineEdit::Password);
	vboxACC->addWidget(lePassword_);
	connect (lePassword_, &QLineEdit::returnPressed, this, &ProfileManager::ok);

	//-- Checkboxes, don't ask password on login
	cbSavePassword_ = new QCheckBox(this);
	cbSavePassword_->setText(QStringLiteral("Save password"));
	vboxACC->addWidget(cbSavePassword_);
	connect(cbSavePassword_, &QCheckBox::stateChanged, this, &ProfileManager::setCBEnable);

	//-- Use as default profile
	cbDefaultProfile_ = new QCheckBox(this);
	cbDefaultProfile_->setText("Default profile");
	cbDefaultProfile_->setEnabled(false);
	vboxACC->addWidget(cbDefaultProfile_);

	//-- OK, load profile
	QPushButton* button = new QPushButton(this);
	button->setText(QStringLiteral("OK"));
	connect(button, &QPushButton::clicked, this, &ProfileManager::ok);
	vboxACC->addWidget(button);

	//-- Creating list of dbPlugins
	cmbDBPlugins_ = new QComboBox(this);
	cmbDBPlugins_->setMinimumSize(120, 18);
	vboxDB->addWidget(cmbDBPlugins_);
	cmbDBPlugins_->addItems(PluginLoader::getDBPlugins());

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

	lePassword_->setFocus();

	hboxMain->setSizeConstraint(QLayout::SetFixedSize);

	loadProfiles();
	loadProfileDetails(cmbProfiles_->currentText());
}

ProfileManager::~ProfileManager()
{
	if (profiles_) {
		QDir* profilesDir = Folders::getProfileDir();
		QFile file(profilesDir->absoluteFilePath("profiles.dat"));
		if (file.open(QIODevice::WriteOnly)) {
			QDataStream out(&file);
			out.setVersion(DataSteamVer);
			Profile profile;
			QMap<QString, Profile>::const_iterator i = profiles_->constBegin();
			QMap<QString, Profile>::const_iterator iEnd = profiles_->constEnd();
			while (i != iEnd) {
				//-- Name -> Password -> DBPluginName -> save pass -> default profile
				writeStrToFile(out, i.key());
				profile = i.value();
				writeStrToFile(out, profile.password);
				writeStrToFile(out, profile.dbPluginName);
				writeBoolToFile(out, profile.savePassword);
				writeBoolToFile(out, profile.defaultProfile);
				i++;
			}
		}
		file.close();

		delete profilesDir;
	}
	delete profiles_;
}

void ProfileManager::setCBEnable(int state)
{
	if (!state) {
		cbDefaultProfile_->setEnabled(false);
		cbDefaultProfile_->setCheckState(Qt::Unchecked);
	} else
		cbDefaultProfile_->setEnabled(true);
}

QString ProfileManager::readStrFromFile(QDataStream& in)
{
	qint32 size;
	QByteArray data;
	in >> size;
	data.resize(size);
	in >> data;
	return QString::fromLatin1(QByteArray::fromBase64(QByteArray::fromHex(data)));
}

void ProfileManager::writeStrToFile(QDataStream& out, const QString& text)
{
	QByteArray data;
	QDataStream toDataBlock(&data, QIODevice::WriteOnly);

	toDataBlock.setVersion(DataSteamVer);
	toDataBlock << quint32(0) << text.toLatin1().toBase64().toHex();
	toDataBlock.device()->seek(0);
	toDataBlock << quint32(data.size() - sizeof(quint32));

	out << data;
}

bool ProfileManager::readBoolFromFile(QDataStream& in)
{
	bool result;
	in >> result;

/*#ifndef NDEBUG
	QString str;
	if (result)
		str = "read true";
	else
		str = "read false";

	qDebug(str.toLatin1());
#endif*/

	return result;
}

void ProfileManager::writeBoolToFile(QDataStream& out, bool val)
{
/*#ifndef NDEBUG
	QString str;
	if (val)
		str = "write true";
	else
		str = "write false";

	qDebug(str.toLatin1());
#endif*/

	out << val;
}


void ProfileManager::loadProfiles()
{
	cmbProfiles_->clear();
	if (!profiles_)
		profiles_ = new QMap<QString, Profile>();

	Profile item;
	QString name;

	QDir* profilesDir = Folders::getProfileDir();
	QFile file(profilesDir->absoluteFilePath("profiles.dat"));
	if (file.open(QIODevice::ReadOnly)) {
		QDataStream in(&file);
		in.setVersion(DataSteamVer);
		while (!in.atEnd()) {
			//-- Name -> Password -> DBPluginName -> save pass -> default profile
			name = readStrFromFile(in);
			item.password = readStrFromFile(in);
			item.dbPluginName = readStrFromFile(in);
			item.savePassword = readBoolFromFile(in);
			item.defaultProfile = readBoolFromFile(in);

			profiles_->insert(name, item);
			cmbProfiles_->addItem(name);

			if (item.defaultProfile)
				qsDefaultProfile_ = name;
		}
		file.close();
	}
	if (profiles_->isEmpty())
		cmbProfiles_->addItem(QStringLiteral("No profile"));

	delete profilesDir;
}

void ProfileManager::loadProfileDetails(const QString& name)
{
	if (profiles_ && profiles_->contains(name)) {
		Profile p = profiles_->value(name);
		//QMessageBox::critical(0, QStringLiteral("Debug"), name, QMessageBox::Cancel);

		if (p.savePassword) {
			lePassword_->setText(p.password);
			cbSavePassword_->setCheckState(Qt::Checked);
		} else {
			lePassword_->setText("");
			cbSavePassword_->setCheckState(Qt::Unchecked);
		}

		if (p.defaultProfile)
			cbDefaultProfile_->setCheckState(Qt::Checked);
		else
			cbDefaultProfile_->setCheckState(Qt::Unchecked);
		//delete p;

		cmbDBPlugins_->setCurrentIndex(cmbDBPlugins_->findText(p.dbPluginName));
	}
}

int ProfileManager::loadDefaultProfile()
{
	if (qsDefaultProfile_.isEmpty())
		//-- No default profile found
		return 1;

	if (!cmbDBPlugins_->count()) {
		QMessageBox::critical(0, QStringLiteral("Login error"),
							  QStringLiteral("Failed to login.\nThere is no one DB plugin."),
							  QMessageBox::Cancel);
		return 1;
	}

	Profile profile = profiles_->value(qsDefaultProfile_);

	//-- Try to load DB plugin and login
	if (cmbDBPlugins_->findText(profile.dbPluginName) == -1) {
		QMessageBox::critical(0, QStringLiteral("Login error"),
							  "Failed to login.\nCan't find DB plugin "
							  + profile.dbPluginName
							  + " .",
							  QMessageBox::Cancel);
		return 1;
	}

	IDBPlugin* plugin = qobject_cast<IDBPlugin*>(PluginLoader::loadPlugin(profile.dbPluginName));

	if (!plugin) {
		QMessageBox::critical(0, QStringLiteral("Login error"),
			"Failed to login.\nCan't load DB plugin " + profile.dbPluginName + " .",
			QMessageBox::Cancel);
		return 1;
	}

	if (plugin->Login(qsDefaultProfile_, profile.password)) {
		QMessageBox::critical(0, QStringLiteral("Login error"),
			QStringLiteral("Failed to login.\nProfile does not exist or assword does not match."),
			QMessageBox::Cancel);
		PluginLoader::unloadPlugin(profile.dbPluginName);
		return 1;
	}

	return 0;
}

void ProfileManager::abort()
{
	//-- Abort Elise loading and exit
	done(1);
}

void ProfileManager::makeDefault(const QString& profile)
{
	QMap<QString, Profile>::iterator i = profiles_->begin();
	QMap<QString, Profile>::iterator iEnd = profiles_->end();
	while (i != iEnd) {
		if (i.key() != profile)
			i.value().defaultProfile = false;
		else
			i.value().defaultProfile = true;
		i++;
	}
}

void ProfileManager::ok()
{
	if (!cmbDBPlugins_->count()) {
		QMessageBox::critical(0, QStringLiteral("Login error"),
							  QStringLiteral("Failed to login.\nThere is no one DB plugin."),
							  QMessageBox::Cancel);
		done(1);
		return;
	}

	Profile profile = profiles_->value(cmbProfiles_->currentText());

	//-- Try to load DB plugin and login
	if (cmbDBPlugins_->findText(profile.dbPluginName) == -1) {
		QMessageBox::critical(0, QStringLiteral("Login error"),
							  "Failed to login.\nCan't find DB plugin "
							  + profile.dbPluginName
							  + " .",
							  QMessageBox::Cancel);
		return;
	}

	IDBPlugin* plugin = qobject_cast<IDBPlugin*>(PluginLoader::loadPlugin(profile.dbPluginName));

	if (!plugin) {
		QMessageBox::critical(0, QStringLiteral("Login error"),
			"Failed to login.\nCan't load DB plugin " + profile.dbPluginName + " .",
			QMessageBox::Cancel);
		return;
	}

	if (lePassword_->text() != profile.password) {
		QMessageBox::critical(0, QStringLiteral("Login error"),
							  "Failed to login.\nWrong password.",
							  QMessageBox::Cancel);
		return;
	}

	if (plugin->Login(cmbProfiles_->currentText(), lePassword_->text())) {
		QMessageBox::critical(0, QStringLiteral("Login error"),
			QStringLiteral("Failed to login.\nProfile does not exist or assword does not match."),
			QMessageBox::Cancel);
		PluginLoader::unloadPlugin(profile.dbPluginName);
		return;
	}

	Profile* p = &(*profiles_)[cmbProfiles_->currentText()];
	if (cbSavePassword_->isChecked())
		p->savePassword = true;
	else
		p->savePassword = false;

	if (cbDefaultProfile_->isChecked())
		makeDefault(cmbProfiles_->currentText());
	else
		p->defaultProfile = false;

	done(0);
}

void ProfileManager::createProfile()
{
	if (!cmbDBPlugins_->count()) {
		QMessageBox::critical(0, QStringLiteral("Create profile error"),
							  QStringLiteral("Failed to create profile.\nThere is no one DB plugin."),
							  QMessageBox::Cancel);
		done(1);
		return;
	}
	QString dbPluginName = cmbDBPlugins_->currentText();
	IDBPlugin* plugin = qobject_cast<IDBPlugin*>(PluginLoader::loadPlugin(dbPluginName));
	if (!plugin) {
		QMessageBox::critical(0, QStringLiteral("Create profile error"),
			"Failed to create profile.\nCan't load DB plugin " + dbPluginName + " .",
			QMessageBox::Cancel);
		return;
	}

	bool ok;
	QString pass = QInputDialog::getText(this, tr("Create profile"),
										 tr("Enter the password again:"), QLineEdit::Password,
										 QString(), &ok);
	if (!ok || pass.isEmpty())
		return;

	if (pass != lePassword_->text())
		QMessageBox::critical(0, QStringLiteral("Create profile error"),
							  QStringLiteral("Failed to create profile.\nPassword does not match."),
							  QMessageBox::Cancel);

	QString name = cmbProfiles_->currentText();
	if (plugin->CreateProfile(name, pass)) {
		//QMessageBox::critical(0, QStringLiteral("Create profile error"),
		//	QStringLiteral("Failed to create profile.\nProfile already exist or another internal plugin error."),
		//	QMessageBox::Cancel);
		PluginLoader::unloadPlugin(dbPluginName);
		return;
	}

	Profile item;
	item.password = pass;
	item.savePassword = cbSavePassword_->isChecked();
	item.defaultProfile = false;
	item.dbPluginName = dbPluginName;

	profiles_->insert(name, item);

	bool isDefault = (cbDefaultProfile_->isChecked() || profiles_->count() == 1);
	if (item.savePassword && isDefault)
		makeDefault(name);

	done(0);
}

void ProfileManager::importProfile()
{
	QMessageBox::critical(0, QStringLiteral("Error"),
						  QStringLiteral("Sorry, this function is not supported yet."),
						  QMessageBox::Cancel);
}
