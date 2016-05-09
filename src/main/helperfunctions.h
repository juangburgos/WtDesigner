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

// ------------------------------------------------------ HELPER FUNCTIONS --------------------------------------------------------
#ifndef HELPERFUNCTS_H
#define HELPERFUNCTS_H

#include <QDebug>

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QMetaObject>
#include <QMetaProperty>
#include <QByteArray>
#include <QDomDocument>
#include <QDomElement>
#include <QWebPage>
#include <QWebFrame>
#include <QWebElement>
#include <QMessageBox>
#include <QSvgRenderer>
#include <QPainter>
#include <QtGlobal>
#include <QMap>
#include <QStandardPaths>
#include <QUrl>

#include <Wt/WContainerWidget>

class SignalEmiter;

Wt::WContainerWidget * GetWContainerParent(QObject *qparent);

QByteArray GetAutoGenCpp (SignalEmiter *rootobj, QString strProjName);

void       ListAllIncludes (QObject *object, QStringList &strListIncludes);
void       DeclAllWidgets  (QObject *object, QByteArray  &config);
void       ConfigAllWidgets(QObject *object, QByteArray  &config, QByteArray tabs = "", QByteArray parentid = "");

void       PrintAllStyleSheets(SignalEmiter *rootobj, QByteArray  &config);
void       PrintAllJavaScripts(SignalEmiter *rootobj, QByteArray  &config);
void       PrintAllConnections(SignalEmiter *rootobj, QByteArray  &config);

QByteArray GetAllProperties (QObject *rootobj);
void       HelpUpdateAllProperties (QObject *rootobj); // renamed due to clash with mainwindow method

void       MapAllProperties (QDomDocument *doc, QObject *parent);
QObject  * GetWtQtObjById   (QObject *parent  , QString strId  );
QObject  * FindWtQtRoot     (QObject *child);

QMap<QString,QObject*> GetWtQtSiblings(QObject *sibling);

QByteArray      GetWtFromHtml         (QByteArray &input, QWebElement *element = NULL, QByteArray tabs = "", QByteArray filepath = "");
void            GetStylesFromHtml     (QByteArray &config);

QByteArray ProcessHEADtag  (QWebElement *element, QByteArray filepath = "");

QByteArray ProcessBODYtag  (QWebElement *element);
QByteArray ProcessDIVtag   (QWebElement *element);
QByteArray ProcessAtag     (QWebElement *element);
QByteArray ProcessIMGtag   (QWebElement *element, QByteArray filepath = "");
QByteArray ProcessSPANtag  (QWebElement *element);
QByteArray ProcessINPUTtag (QWebElement *element);
QByteArray ProcessBUTTONtag(QWebElement *element);
QByteArray ProcessItag     (QWebElement *element);

QMetaObject GetMetaObjectByClassName(QString strClassName);

void SetIconColor(QString svgres, QIcon &svgicon, QString svgcolor);
void SetAttrRecur(QDomElement &elem, QString strtagname, QString strattr, QString strattrval);

bool CheckValidProjPath(const QString &strProjPath);
QString GetProjFileName(const QString &strProjPath);

bool CopyAllFilesInPath(QString strSourcePath, QString strTargetPath, QStringList strListTypes);

QString FindWtIncludeDir(QString strRefDir = "", int level = 0);
QString FindWtLibraryDir(QString strRefDir = "", int level = 0);
QString FindWtBinaryDir (QString strRefDir = "", int level = 0);

QStringList FindWtLibFiles(QString strLibDir, QStringList listOfLibNames);

QString ProcessCMakeTemplate(QString strInputFile, QString strOutputFile, QList<QPair<QString, QString> > listStrPairs);

bool    ValidateWtIncludeDir(QString strIncludeDir);
bool    ValidateWtLibraryDir(QString strLibraryDir);
bool    ValidateWtBinaryDir (QString strBinaryDir );

QString EncodeTextXML(QString strInputRaw);
QString DecodeTextXML(QString strInputEncoded);


#endif
