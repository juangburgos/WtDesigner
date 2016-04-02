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

#include <QCoreApplication>
#include <Wt/WServer>
#include "mainwindow.h"
#include "mainpage.h"

QHash<int, MainPage*> m_hashpages;
MainWindow      *m_mainwindow;

Wt::WApplication * createApplication(const Wt::WEnvironment& env)
{
	static int counter = 0;
	MainPage * page = new MainPage(env, m_mainwindow);
	//m_hashpages.insert(counter, page);
	//counter++;
	return page;
}

WtServerWorker::WtServerWorker(MainWindow *mainwindow) : QThread(mainwindow)
{
	// make copy of pointer to main window to be able to pass it to Wt::WApplication (MainPage class instance)
	m_mainwindow = mainwindow;
}

WtServerWorker::~WtServerWorker()
{
	// delete allocated memory 
	free(m_argv);
}

void WtServerWorker::run()
{		
	// configure wt server
	Wt::WServer server(m_argc, m_argv, WTHTTP_CONFIGURATION);
	server.addEntryPoint(Wt::Application, createApplication);
	// start wt server
	try
	{
		server.start();
	}
	catch (...)
	{
		qDebug() << "[ERROR] Exception occurred while starting Wt Server in WtServerWorker::run";
	}
	// eter event loop and wait
	exec();
	// stop wt server
	try
	{
		server.stop();
	}
	catch (...)
	{
		qDebug() << "[ERROR] Exception occurred while shutting down Wt Server in WtServerWorker::run";
	}
}

void WtServerWorker::ConfigureWtServer(QVector<QString> vectargs)
{
	// make local copies in class members
	m_argc = vectargs.size();
	// allocate memory for copying c strings
	m_argv = (char**)malloc(m_argc*sizeof(char**));
	for (int i = 0; i < m_argc; i++)
	{
		m_argv[i] = (char*)calloc(vectargs[i].size(), sizeof(char));
		// copy each qstring in list as c string
		strcpy(m_argv[i], vectargs[i].toStdString().c_str());
	}
}
