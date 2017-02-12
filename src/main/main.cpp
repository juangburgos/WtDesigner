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

// Qt Includes
#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QPixmap>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
	// create qt application
	QApplication a(argc, argv);

#ifdef Q_OS_UNIX
    QApplication::addLibraryPath("./plugins");
#endif

	// show splash screen
	QPixmap pixmap(":/logo_loading"); // NOTE : had to convert text to path in inkscape for text to show
	QSplashScreen splash(pixmap);
	splash.show();

	// configure company and appname
	QCoreApplication::setOrganizationName("Burgos Software");
	QCoreApplication::setOrganizationDomain("wtdesigner.com");
	QCoreApplication::setApplicationName("WtDesigner");

	// set darkstyle CSS
	QFile f(":qdarkstyle/style.qss");
	if (!f.exists())
	{
		qDebug() << "[ERROR] Unable to set stylesheet, file not found.";
	}
	else
	{
		f.open(QFile::ReadOnly | QFile::Text);
		QTextStream ts(&f);
		a.setStyleSheet(ts.readAll());
	}

	// create main window and show it
	MainWindow w(argc, argv);
	w.show();
	// found this bug in debug mode when remove and then undo main.css file : https://bugreports.qt.io/browse/QTBUG-35663
	// close splash screen
	splash.finish(&w);
	// start qts event loop
	return a.exec();
}
