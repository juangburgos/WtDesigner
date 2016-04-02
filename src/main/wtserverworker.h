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

#ifndef WTSERVERWORKER_H
#define WTSERVERWORKER_H

#include <QThread>
#include <QVector>
#include <QString>
#include <QHash>
#include <QHashIterator>
#include <Wt/WApplication>
#include <Wt/WEnvironment>

class MainWindow;

class WtServerWorker : public QThread
{
	Q_OBJECT

public:
	WtServerWorker(MainWindow *mainwindow);
	~WtServerWorker();

	void run();

	Wt::WServer *m_pserver;

	void ConfigureWtServer(QVector<QString> vectargs);

private:
	int              m_argc;
	char           **m_argv;


};

#endif // WTSERVERWORKER_H
