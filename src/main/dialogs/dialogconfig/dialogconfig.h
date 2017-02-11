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

#ifndef DIALOGCONFIG_H
#define DIALOGCONFIG_H

#include <QDialog>
#include <QString>
#include <QSettings>
#include <QFileDialog>

namespace Ui {
class DialogConfig;
}

class DialogConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogConfig(QWidget *parent = 0);
    ~DialogConfig();

    void setDocRootRef    (QString * p_str_docroot);
    void setHttpAddressRef(QString * p_str_http_address);
    void setHttpPortRef   (QString * p_str_http_port);

    void setPhoneWidthRef   (int * p_int_phone_width);
    void setTabletWidthRef  (int * p_int_tablet_width);
    void setLaptopWidthRef  (int * p_int_laptop_width);
    void setComputerWidthRef(int * p_int_computer_width);
#ifdef Q_OS_WIN
	void setIncludeDirRef (QString * p_str_includedir);
	void setLibraryDirRef (QString * p_str_librarydir);
	void setBinaryDirRef  (QString * p_str_binarydir );
#endif
	void setServerPortRef   (QString * p_str_server_port);
	void setWtThemeRef      (QString * p_str_wt_theme);
	void setWtTitleRef      (QString * p_str_wt_title);

    void setupDialogWidgets();

Q_SIGNALS:
    void UpdateSettings();

private Q_SLOTS:
    void on_pushApply_clicked();

    void on_pushCancel_clicked();

    void on_pushOK_clicked();

    void on_spin_http_port_valueChanged(int arg1);

    void on_spin_phone_width_valueChanged(int arg1);

    void on_spin_tablet_width_valueChanged(int arg1);

    void on_spin_laptop_width_valueChanged(int arg1);

    void on_spin_computer_width_valueChanged(int arg1);

#ifdef Q_OS_WIN
    void on_push_includedir_clicked();

    void on_push_librarydir_clicked();

    void on_push_binarydir_clicked();
#endif

    void on_spin_app_server_port_valueChanged(int arg1);

    void on_combo_wtheme_currentTextChanged(const QString &arg1);

    void on_line_wtitle_textEdited(const QString &arg1);

private:
    Ui::DialogConfig *ui;

	QString str_docroot       ;
	QString str_http_address  ;
	QString str_http_port     ;
	int     int_phone_width   ;
	int     int_tablet_width  ;
	int     int_laptop_width  ;
	int     int_computer_width;
#ifdef Q_OS_WIN
	QString str_includedir    ;
	QString str_librarydir    ;
	QString str_binarydir     ;
#endif
	QString str_server_port   ;
	QString str_wt_theme      ;
	QString str_wt_title      ;

    QString * m_p_str_docroot       ;
    QString * m_p_str_http_address  ;
    QString * m_p_str_http_port     ;
    int     * m_p_int_phone_width   ;
    int     * m_p_int_tablet_width  ;
    int     * m_p_int_laptop_width  ;
    int     * m_p_int_computer_width;
#ifdef Q_OS_WIN
	QString * m_p_str_includedir    ;
	QString * m_p_str_librarydir    ;
	QString * m_p_str_binarydir     ;
#endif
	QString * m_p_str_server_port   ;
	QString * m_p_str_wt_theme      ;
	QString * m_p_str_wt_title      ;

	QSettings m_settings;

	QString   m_str_last_selected   ;
};

#endif // DIALOGCONFIG_H
