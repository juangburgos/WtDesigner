// Copyright (c) 2016 Juan Gonzalez Burgos
// 
// This file is part of WtDesigner.
// 
// WtDesigner is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// WtDesigner is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with WtDesigner.  If not, see <http://www.gnu.org/licenses/>.

#include "dialogconfig.h"
#include "ui_dialogconfig.h"
#include "helperfunctions.h"
#include "./../myglobals.h"
#include <QDebug>

DialogConfig::DialogConfig(QWidget *parent) :
QDialog(parent),
ui(new Ui::DialogConfig)
{
	ui->setupUi(this);

#ifndef Q_OS_WIN
	ui->groupBox_4->setVisible(false); // library find groubox
#endif

	m_p_str_docroot = NULL;
	m_p_str_http_address = NULL;
	m_p_str_http_port = NULL;
	m_p_int_phone_width = NULL;
	m_p_int_tablet_width = NULL;
	m_p_int_laptop_width = NULL;
	m_p_int_computer_width = NULL;
#ifdef Q_OS_WIN
	m_p_str_includedir = NULL;
	m_p_str_librarydir = NULL;
	m_p_str_binarydir = NULL;
#endif
	m_p_str_server_port = NULL;
	m_p_str_wt_theme = NULL;
	m_p_str_wt_title = NULL;

	str_docroot = "N/A";
	str_http_address = "N/A";
	str_http_port = "N/A";
	int_phone_width = -1;
	int_tablet_width = -1;
	int_laptop_width = -1;
	int_computer_width = -1;
#ifdef Q_OS_WIN
	str_includedir = "N/A";
	str_librarydir = "N/A";
	str_binarydir = "N/A";
#endif
	str_server_port = "N/A";
	str_wt_theme = "N/A";
	str_wt_title = "N/A";

	m_str_last_selected = QDir::homePath();

	// setup wt theme combobox
	ui->combo_wtheme->addItems(QStringList() << g_strThemeNone << g_strThemeDefault << g_strThemePolished << g_strThemeBootstrat2 << g_strThemeBootstrat3);
}

DialogConfig::~DialogConfig()
{
	delete ui;
}

void DialogConfig::setDocRootRef(QString *p_str_docroot)
{
	if (p_str_docroot) { m_p_str_docroot = p_str_docroot; }
	else { qDebug() << "[ERROR] Invalid DocRoot reference in Settings Dialog"; }
}

void DialogConfig::setHttpAddressRef(QString *p_str_http_address)
{
	if (p_str_http_address) { m_p_str_http_address = p_str_http_address; }
	else { qDebug() << "[ERROR] Invalid HttpAddress reference in Settings Dialog"; }
}

void DialogConfig::setHttpPortRef(QString *p_str_http_port)
{
	if (p_str_http_port) { m_p_str_http_port = p_str_http_port; }
	else { qDebug() << "[ERROR] Invalid HttpPort reference in Settings Dialog"; }
}

void DialogConfig::setPhoneWidthRef(int *p_int_phone_width)
{
	if (p_int_phone_width) { m_p_int_phone_width = p_int_phone_width; }
	else { qDebug() << "[ERROR] Invalid PhoneWidth reference in Settings Dialog"; }
}

void DialogConfig::setTabletWidthRef(int *p_int_tablet_width)
{
	if (p_int_tablet_width) { m_p_int_tablet_width = p_int_tablet_width; }
	else { qDebug() << "[ERROR] Invalid TabletWidth reference in Settings Dialog"; }
}

void DialogConfig::setLaptopWidthRef(int *p_int_laptop_width)
{
	if (p_int_laptop_width) { m_p_int_laptop_width = p_int_laptop_width; }
	else { qDebug() << "[ERROR] Invalid LaptopWidth reference in Settings Dialog"; }
}

void DialogConfig::setComputerWidthRef(int *p_int_computer_width)
{
	if (p_int_computer_width) { m_p_int_computer_width = p_int_computer_width; }
	else { qDebug() << "[ERROR] Invalid ComputerWidth reference in Settings Dialog"; }
}

#ifdef Q_OS_WIN
void DialogConfig::setIncludeDirRef(QString * p_str_includedir)
{
	if (p_str_includedir) { m_p_str_includedir = p_str_includedir; }
	else { qDebug() << "[ERROR] Invalid IncludeDir reference in Settings Dialog"; }
}

void DialogConfig::setLibraryDirRef(QString * p_str_librarydir)
{
	if (p_str_librarydir) { m_p_str_librarydir = p_str_librarydir; }
	else { qDebug() << "[ERROR] Invalid LibraryDir reference in Settings Dialog"; }
}

void DialogConfig::setBinaryDirRef(QString * p_str_binarydir)
{
	if (p_str_binarydir) { m_p_str_binarydir = p_str_binarydir; }
	else { qDebug() << "[ERROR] Invalid BinaryDir reference in Settings Dialog"; }
}
#endif

void DialogConfig::setServerPortRef(QString * p_str_server_port)
{
	if (p_str_server_port) { m_p_str_server_port = p_str_server_port; }
	else { qDebug() << "[ERROR] Invalid ServerPort reference in Settings Dialog"; }
}

void DialogConfig::setWtThemeRef(QString * p_str_wt_theme)
{
	if (p_str_wt_theme) { m_p_str_wt_theme = p_str_wt_theme; }
	else { qDebug() << "[ERROR] Invalid WtTheme reference in Settings Dialog"; }
}

void DialogConfig::setWtTitleRef(QString * p_str_wt_title)
{
	if (p_str_wt_title) { m_p_str_wt_title = p_str_wt_title; }
	else { qDebug() << "[ERROR] Invalid WtTitle reference in Settings Dialog"; }
}

void DialogConfig::setupDialogWidgets()
{

	if (m_p_str_docroot)        { str_docroot = *m_p_str_docroot; }
	else { qDebug() << "[ERROR] DocRoot is NULL in setupDialogWidgets";        return; }
	if (m_p_str_http_address)   { str_http_address = *m_p_str_http_address; }
	else { qDebug() << "[ERROR] HttpAddress is NULL in setupDialogWidgets";    return; }
	if (m_p_str_http_port)      { str_http_port = *m_p_str_http_port; }
	else { qDebug() << "[ERROR] HttpPort is NULL in setupDialogWidgets";       return; }
	if (m_p_int_phone_width)    { int_phone_width = *m_p_int_phone_width; }
	else { qDebug() << "[ERROR] PhoneWidth is NULL in setupDialogWidgets";     return; }
	if (m_p_int_tablet_width)   { int_tablet_width = *m_p_int_tablet_width; }
	else { qDebug() << "[ERROR] TabletWidth is NULL in setupDialogWidgets";    return; }
	if (m_p_int_laptop_width)   { int_laptop_width = *m_p_int_laptop_width; }
	else { qDebug() << "[ERROR] LaptopWidth is NULL in setupDialogWidgets";    return; }
	if (m_p_int_computer_width) { int_computer_width = *m_p_int_computer_width; }
	else { qDebug() << "[ERROR] ComputerWidth is NULL in setupDialogWidgets";  return; }
#ifdef Q_OS_WIN
	if (m_p_str_includedir)    { str_includedir = *m_p_str_includedir; }
	else { qDebug() << "[ERROR] IncludeDir is NULL in setupDialogWidgets";     return; }
	if (m_p_str_librarydir)    { str_librarydir = *m_p_str_librarydir; }
	else { qDebug() << "[ERROR] LibraryDir is NULL in setupDialogWidgets";     return; }
	if (m_p_str_binarydir)     { str_binarydir = *m_p_str_binarydir; }
	else { qDebug() << "[ERROR] BinaryDir is NULL in setupDialogWidgets";      return; }
#endif
	if (m_p_str_server_port)      { str_server_port = *m_p_str_server_port; }
	else { qDebug() << "[ERROR] ServerPort is NULL in setupDialogWidgets";     return; }
	if (m_p_str_wt_theme)      { str_wt_theme = *m_p_str_wt_theme; }
	else { qDebug() << "[ERROR] WtTheme is NULL in setupDialogWidgets";        return; }
	if (m_p_str_wt_title)      { str_wt_title = *m_p_str_wt_title; }
	else { qDebug() << "[ERROR] WtTheme is NULL in setupDialogWidgets";        return; }

	ui->line_docroot->setText(str_docroot);
	ui->line_http_address->setText(str_http_address);
	ui->spin_http_port->setValue(str_http_port.toInt());
	ui->spin_phone_width->setValue(int_phone_width);
	ui->spin_tablet_width->setValue(int_tablet_width);
	ui->spin_laptop_width->setValue(int_laptop_width);
	ui->spin_computer_width->setValue(int_computer_width);
#ifdef Q_OS_WIN
	ui->ledit_includedir->setText(str_includedir);
	ui->ledit_librarydir->setText(str_librarydir);
	ui->ledit_binarydir->setText(str_binarydir);
#endif
	ui->spin_app_server_port->setValue(str_server_port.toInt());
	ui->combo_wtheme->setCurrentText(str_wt_theme);
	ui->line_wtitle->setText(str_wt_title);

	ui->tabSettings->setCurrentIndex(0);
}

void DialogConfig::on_pushApply_clicked()
{
	// overwrite variables
	if (m_p_str_http_port)      { *m_p_str_http_port = str_http_port; }
	else { qDebug() << "[ERROR] HttpPort is NULL in setupDialogWidgets";        return; }
	if (m_p_int_phone_width)     { *m_p_int_phone_width = int_phone_width; }
	else { qDebug() << "[ERROR] PhoneWidth is NULL in setupDialogWidgets";      return; }
	if (m_p_int_tablet_width)   { *m_p_int_tablet_width = int_tablet_width; }
	else { qDebug() << "[ERROR] TabletWidth is NULL in setupDialogWidgets";     return; }
	if (m_p_int_laptop_width)   { *m_p_int_laptop_width = int_laptop_width; }
	else { qDebug() << "[ERROR] LaptopWidth is NULL in setupDialogWidgets";     return; }
	if (m_p_int_computer_width) { *m_p_int_computer_width = int_computer_width; }
	else { qDebug() << "[ERROR] ComputerWidth is NULL in setupDialogWidgets";   return; }
#ifdef Q_OS_WIN
	if (m_p_str_includedir)     { *m_p_str_includedir = str_includedir; }
	else { qDebug() << "[ERROR] IncludeDir is NULL in setupDialogWidgets";      return; }
	if (m_p_str_librarydir)     { *m_p_str_librarydir = str_librarydir; }
	else { qDebug() << "[ERROR] LibraryDir is NULL in setupDialogWidgets";      return; }
	if (m_p_str_binarydir)      { *m_p_str_binarydir = str_binarydir; }
	else { qDebug() << "[ERROR] BinaryDir is NULL in setupDialogWidgets";       return; }
#endif
	if (m_p_str_server_port)    { *m_p_str_server_port = str_server_port; }
	else { qDebug() << "[ERROR] ServerPort is NULL in setupDialogWidgets";      return; }
	if (m_p_str_wt_theme)       { *m_p_str_wt_theme    = str_wt_theme; }
	else { qDebug() << "[ERROR] WtTheme is NULL in setupDialogWidgets";         return; }
	if (m_p_str_wt_title)       { *m_p_str_wt_title = str_wt_title; }
	else { qDebug() << "[ERROR] WtTitle is NULL in setupDialogWidgets";         return; }

	// save peristent qsettings
	// begin server config settings
	m_settings.beginGroup("server");
	m_settings.setValue("wt_valhttpport", *m_p_str_http_port);
	m_settings.endGroup();

	// begin responsive design default sizes
	m_settings.beginGroup("responsive");
	m_settings.setValue("m_phone_width", *m_p_int_phone_width);
	m_settings.setValue("m_tablet_width", *m_p_int_tablet_width);
	m_settings.setValue("m_laptop_width", *m_p_int_laptop_width);
	m_settings.setValue("m_computer_width", *m_p_int_computer_width);
	m_settings.endGroup();

#ifdef Q_OS_WIN
	// begin cmake config settings
	m_settings.beginGroup("cmake");
	m_settings.setValue("m_strincludedir", *m_p_str_includedir);
	m_settings.setValue("m_strlibrarydir", *m_p_str_librarydir);
	m_settings.setValue("m_strbinarydir", *m_p_str_binarydir);
	m_settings.endGroup();
#endif

	// begin application config settings
	m_settings.beginGroup("application");
	m_settings.setValue("app_serverport", *m_p_str_server_port);
	// NOTE : wt_theme and wt_title are saved in *wui config file, in a per-project fashion
	m_settings.endGroup();

	// reload server to display new settings
	Q_EMIT UpdateSettings();
}

void DialogConfig::on_pushCancel_clicked()
{
	reject();
}

void DialogConfig::on_pushOK_clicked()
{
	on_pushApply_clicked();
	accept();
}

void DialogConfig::on_spin_http_port_valueChanged(int arg1)
{
	str_http_port = QString("%1").arg(arg1);
}

void DialogConfig::on_spin_phone_width_valueChanged(int arg1)
{
	int_phone_width = arg1;
}

void DialogConfig::on_spin_tablet_width_valueChanged(int arg1)
{
	int_tablet_width = arg1;
}

void DialogConfig::on_spin_laptop_width_valueChanged(int arg1)
{
	int_laptop_width = arg1;
}

void DialogConfig::on_spin_computer_width_valueChanged(int arg1)
{
	int_computer_width = arg1;
}

#ifdef Q_OS_WIN
void DialogConfig::on_push_includedir_clicked()
{
	if (QDir(str_includedir).exists()) { m_str_last_selected = str_includedir; }
	QString dir = QFileDialog::getExistingDirectory(this, tr("Select Wt Include Directory"), m_str_last_selected, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (dir.isEmpty()) { return; }
	// validate dir
	if (!ValidateWtIncludeDir(dir))
	{
		int ret = QMessageBox::warning(this, tr("WtDesigner"),
			tr("WtDesigner could not validate this path as the include directory.\n"
			"Do you want to overwrite it anyway?"),
			QMessageBox::Yes | QMessageBox::No,
			QMessageBox::No);
		if (ret == QMessageBox::No)
		{
			return;
		}
	}
	// update variable
	str_includedir      = dir;
	ui->ledit_includedir->setText(dir);
	// update last selected
	QDir tmpDir(dir);
	tmpDir.cdUp();
	m_str_last_selected = tmpDir.path();
}

void DialogConfig::on_push_librarydir_clicked()
{
	if (QDir(str_librarydir).exists()) { m_str_last_selected = str_librarydir; }
	QString dir = QFileDialog::getExistingDirectory(this, tr("Select Wt Library Directory"), m_str_last_selected, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (dir.isEmpty()) { return; }
	// validate dir
	if (!ValidateWtLibraryDir(dir))
	{
		int ret = QMessageBox::warning(this, tr("WtDesigner"),
			tr("WtDesigner could not validate this path as the library directory.\n"
			"Do you want to overwrite it anyway?"),
			QMessageBox::Yes | QMessageBox::No,
			QMessageBox::No);
		if (ret == QMessageBox::No)
		{
			return;
		}
	}
	// update variable
	str_librarydir      = dir;
	ui->ledit_librarydir->setText(dir);
	// update last selected
	QDir tmpDir(dir);
	tmpDir.cdUp();
	m_str_last_selected = tmpDir.path();
}

void DialogConfig::on_push_binarydir_clicked()
{
	if (QDir(str_binarydir).exists()) { m_str_last_selected = str_binarydir; }
	QString dir = QFileDialog::getExistingDirectory(this, tr("Select Wt Binary Directory"), m_str_last_selected, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (dir.isEmpty()) { return; }
	// validate dir
	if (!ValidateWtBinaryDir(dir))
	{
		int ret = QMessageBox::warning(this, tr("WtDesigner"),
			tr("WtDesigner could not validate this path as the binrary directory.\n"
			"Do you want to overwrite it anyway?"),
			QMessageBox::Yes | QMessageBox::No,
			QMessageBox::No);
		if (ret == QMessageBox::No)
		{
			return;
		}
	}
	// update variable
	str_binarydir       = dir;
	ui->ledit_binarydir->setText(dir);
	// update last selected
	QDir tmpDir(dir);
	tmpDir.cdUp();
	m_str_last_selected = tmpDir.path();
}
#endif

void DialogConfig::on_spin_app_server_port_valueChanged(int arg1)
{
	str_server_port = QString("%1").arg(arg1);
}

void DialogConfig::on_combo_wtheme_currentTextChanged(const QString &arg1)
{
	str_wt_theme = arg1;
}

void DialogConfig::on_line_wtitle_textEdited(const QString &arg1)
{
	str_wt_title = arg1;
}
