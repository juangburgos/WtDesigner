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

#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QObject>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QFileInfo>
#include <QEventLoop>
#include <QList>
#include <QDirIterator>
#include <QMap>
#include <QPair>

#include "wtwithqtlib/WQApplication.h"
#include <Wt/WServer>

#include "mixedclasses.h"

class MainWindow;
class SignalEmiter;

QObject* FindElementById(QObject *parent, QString &strid);

class MainPage : public Wt::WQApplication
{
public:
	struct CSSFileInfo
	{
		QString   strLocalPath;
		Wt::WLink wlinkFile;
	};

	MainPage(const Wt::WEnvironment& env, MainWindow *mainwindow);
	~MainPage();

	virtual void create();
	virtual void destroy();

	void SetupUi(Wt::WContainerWidget *wtroot);

	void LoadRecursiveTree(QDomElement element, Wt::WContainerWidget *wparent=0, QObject *qparent=0, int irow=-1);

	void AddStyleSheetFile(QString &strPath);
	void AddJavaScriptFile(QString &strPath);

	QObject * CreateWtQtInstance(QDomElement * element, int irow, Wt::WContainerWidget * wparent, QObject * qparent);

	void CreateConnectionKey(QString &strConnectionId, Wt_ConnectionConfig * conConfig);

	bool PerformConnection(Wt_ConnectionConfig *conConfig);
	bool PerformDisconnection(Wt_ConnectionConfig *conConfig);

    SignalEmiter * GetQtRoot()
    {
        return m_qtroot;
    }
private:
    MainWindow *m_mainwindow;
    SignalEmiter * m_qtroot;

    bool thisQtConnected;
    static bool qtConnected;
};

class SignalEmiter : public WtQtInteractWidget // instead of inheriting WContainerWidget, it contains a ref to one
{
	Q_OBJECT
public:
	SignalEmiter(QObject *parent = 0, MainPage * owner = 0);
	MainPage                  * mp_owner;
	QString                     m_currentState;
	QString                     m_strProjRootPath;
	QList<QString>              m_strCssFilesList; // [NOTE] Used to print used stylesheets in atuogen cpp
	QList<QString>              m_strJsFilesList;  // [NOTE] Used to print used javascripts in atuogen cpp
	QList<Wt_ConnectionConfig*> m_listConnections;
	QString                     m_strWtTitle;
	QString                     m_strWtTheme;

	Wt::WContainerWidget * getWtRoot();

	virtual QString Cpp_instantiate();

	QString Wt_className();

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

	QString Wt_htmlTagName();
	void    Wt_setHtmlTagName(QString tagname);

Q_SIGNALS:
	void RequestCurrentConfig();
	void LoadCurrentConfig();
	void ReloadPage();
	void SendAutoGenCpp(QByteArray config);
	void SendAllProperties(QByteArray allprops);
	void NewClientConneted();
	
public Q_SLOTS:
	void on_ReceivedCurrentConfig(QString currentConfig);

	void on_ElementEliminated  (QString strElemId);
	void on_PropertyChanged    (QString strElemChanged, QString strPropChanged, QString strNewPropVal);
	void on_ConnectionChanged  (QString strSender, QString strSignal, QString strReceiver, QString strSlot, QString strParameter );
	void on_DisconnectOldSignal(QString strSender, QString strSignal, QString strReceiver, QString strSlot, QString strParameter );
	void on_InsertConfig       (QByteArray strConfigFraction, QString strParentName, int irow);	

	void on_GenerateAutoGenCpp(QString strProjName);
	void on_GenerateAllProperties(QString strParentId);
	void on_UpdateAllProperties();

};

#endif // MAINPAGE_H






