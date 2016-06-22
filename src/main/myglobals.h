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

#ifndef MY_GLOBALS_H
#define MY_GLOBALS_H

#include <QString>
#include <QIcon>

const QString g_strLocalRootPath      = "./"      ;
const QString g_strLocalResourcesPath = "./resources/";
const QString g_strLocalTempPath      = "./temp/";
const QString g_strLocalWidgetsPath   = "./widgets/";

const QString g_strResourcesPathConst = "resources/";

const QString g_strDefaultProjPath = "default/";

// [NOTE] : If add new tags dont forget to update MyTreeModel::cleanInvalidElems
const QString g_strStylesheetTag   = "WStyleSheet";
const QString g_strJavascriptTag   = "WJavaScript";

const QString g_strDocumTag      = "WtCreator";
const QString g_strWRootTag      = "WRoot";
const QString g_strValidNodeTag  = "WObject";
const QString g_strConnectionTag = "WConnection";

const QString g_strIdAttr        = "Wt_id";
const QString g_strClassAttr     = "Wt_className";
const QString g_strInlineAttr    = "Wt_isInline";

const QString g_strReceiverAttr  = "Wt_Receiver";

const QString g_strStylePathAttr = "Path";
const QString g_strJavasPathAttr = "Path";

const QString g_strThemeNone       = "None";
const QString g_strThemeDefault    = "Default";
const QString g_strThemePolished   = "Polished";
const QString g_strThemeBootstrat2 = "Boostrap 2";
const QString g_strThemeBootstrat3 = "Boostrap 3";

const QString g_strThemeAttr    = "Wt_theme";
const QString g_strTitleAttr    = "Wt_title";

/** Global icon cache.
 *
 * See https://bugreports.qt.io/browse/QTBUG-50829
 */
class Icons
{
public:
    static QMap<QString, QIcon> & GetCache();
};

#endif
