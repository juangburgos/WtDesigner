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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Wt/WServer>

#include <QMainWindow>
#include <QScrollArea>
#include <QFileDialog>
#include <QFile>
#include <QStandardPaths>
#include <QTextStream>
#include <QMessageBox>
#include <QStack>
#include <QDomDocument>
#include <QUndoStack>
#include <QClipboard>
#include <QMenu>
#include <QSettings>
#include <QSet>
#include <QProgressDialog>

#include "ui_mainwindow.h"

#include "mywebview.h"
#include "./modelview/mytreemodel.h"
#include "./modelview/mypropertymodel.h"
#include "./modelview/mysignalslotsmodel.h"
#include "./modelview/mywidgetmodel.h"

#include "myundocommands.h"

#include "myglobals.h"

#include "./dialogs/dialogconfig/dialogconfig.h"
#include "./dialogs/dialogprojselect/dialogprojselect.h"
#include "./dialogs/dialogprojsave/dialogprojsave.h"
#include "./dialogs/dialogabout/dialogabout.h"

enum WtParams
{
	WT_EXEPATH     ,
	WT_PARDOCROOT  ,
	WT_VALDOCROOT  ,
	WT_PARHTTPADDR ,
	WT_VALHTTPADDR ,
	WT_PARHTTPPORT ,
	WT_VALHTTPPORT ,
	WT_MAXNUMPARAMS
};

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(int argc, char **argv, QWidget *parent = 0);
	~MainWindow();

	Ui::MainWindow     ui;

	MyTreeModel        m_treemodel; // [NOTE] this one encapsulates the domdocument that contains the config that is manipulated by the other models
	MyPropertyModel    m_propertymodel;
	MySignalSlotModel  m_sigslotmodel;
	QFileSystemModel   m_resourcesmodel;
	MyWidgetModel    * m_pwidgetmodel;

	void SetAllIcons(QString svgcolor);

	void LoadProjectPath(QString strProjPathToLoad);
	void LoadDefaultConfig();
	void StartWtServer();
	void StopWtServer();
	void LoadFileConfig(QByteArray config);

	void InitActions();
	void SetTreeModel();

	void UpdateStatusBar();
	void UpdatePropertyTree(QDomElement elem);
	void UpdateSignalSlots(QDomElement elem);

	bool isWidgetFocused(QString strWidgetName);

	void ShowNewFileSystemMenu(QPoint pos, bool isRightClick = false);

	void CheckIfStylesheet(QString strFileName, bool isAdd);
	void CheckIfStylesheetPath(QString strPath, bool isAdd);

	void CheckIfJavascript(QString strFileName, bool isAdd);
	void CheckIfJavascriptPath(QString strPath, bool isAdd);

	void SetupPersistSettings();

#ifdef Q_OS_WIN
	void FindLibFileNames();
#endif

public Q_SLOTS:
	void on_MouseMovedOverElem(QString elem, QString strClass);

	void on_mouseLeftClickedOverElem(QString elem);
	void on_LoadPage();
	void on_ReloadPage();

	void on_RequestCurrentConfig();
	void on_ReceivedAutoGenCpp(QByteArray config);
	void on_ReceivedAllProperties(QByteArray allprops);

	void on_actionNew_triggered();
	void on_actionSave_triggered();
	void on_actionSave_as_triggered();
	void on_actionOpen_triggered();

    void on_actionExport_to_CMake_triggered();
    void on_actionImport_from_HTML_triggered();

	void on_actionUndo_triggered();
	void on_actionRedo_triggered();
	void on_actionCut_triggered();
	void on_actionCopy_triggered();
	void on_actionPaste_triggered();
	void on_actionDelete_triggered();

    void on_actionHtml_Code_triggered();
    void on_actionC_Code_triggered();
    void on_actionAbout_triggered();
    void on_actionTutorials_triggered();
    void on_actionQuit_triggered();
    void on_actionClose_triggered();

    void on_actionPreferences_triggered();

	void on_treeviewWtTree_clicked(const QModelIndex &index);
	void on_treeviewWtTree_contextMenu(const QPoint &point);

    void on_treeviewWtWidgets_doubleClicked(const QModelIndex &index);

	void on_treeviewWtResources_contextMenu(const QPoint &point);

    void on_push_signalslot_remove_clicked();
    void on_push_signalslot_add_clicked();

	void on_propertyChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles = QVector<int> ());
	void on_signalSlotChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles = QVector<int> ());

    void on_treeviewWtResources_doubleClicked(const QModelIndex &index);

    void on_push_resource_remove_clicked();
    void on_push_resource_add_clicked();
    void on_push_resource_reload_clicked();
    void on_push_resource_edit_clicked();

    void on_push_phone_clicked();
    void on_push_tablet_clicked();
    void on_push_laptop_clicked();
    void on_push_computer_clicked();

    void on_push_refresh_clicked();

	void on_ReloadWtServer();

	void on_openExistingProject(QString path);
	void on_saveNewProject(QString path, QString projname);

	void on_receivedDragOnWebview(QByteArray baconfigChunk, QString strParentElemId);

	void on_NewClientConneted();

	void on_cleanChanged(bool clean);

	void on_trackedFileWasMoved(QString strRelSourceFilePath, QString strRelDestinationFilePath);

    void on_listWtJavascripts_itemDoubleClicked(QListWidgetItem *item);
    void on_listWtStylesheets_itemDoubleClicked(QListWidgetItem *item);

	void on_removedTrackedCssFile(QFileInfo fileRef);
	void on_appendedTrackedCssFile(QFileInfo fileRef);
	void on_removedTrackedJsFile(QFileInfo fileRef);
	void on_appendedTrackedJsFile(QFileInfo fileRef);

protected:
	void closeEvent(QCloseEvent * event);

private:

	int                m_argc;
	char             **m_argv;
	QVector<QString>   m_vectWtParams;
	std::unique_ptr<Wt::WServer> m_server;
					  
	MyWebView         *m_WebView;
	QString            m_Address;

	QString            m_strProjRootPath; // user defined project path (to load from and save to)
	QString            m_strProjName;

	QString            m_strCurrentHoveredElemId;
	QString            m_strCurrentHoveredElemClass;
	QString            m_strCurrentSelectedElemId;

	QUndoStack         m_commandHistory;

	int                m_phone_width;
	int                m_tablet_width;
	int                m_laptop_width;
	int                m_computer_width;

	DialogConfig       m_dialogConfig;
	DialogProjSelect   m_dialogProjSel;
	DialogProjSave     m_dialogProjSave;
	DialogAbout        m_dialogAbout;

	QProgressDialog  * mp_ProcessDialog = NULL;

#ifdef Q_OS_WIN
	QString            m_strIncludeDir;
	QString            m_strLibraryDir;
	QString            m_strBinaryDir ;
	QStringList        m_listLibFileNames;
#endif
	QString            m_strServerPort ;
	QString            m_strWtTitle    ;
	QString            m_strWtTheme    ;

	QSettings          m_settings;

	bool               m_boolTemporaryCpp = false;

Q_SIGNALS:
	void SendCurrentConfig(QString currentConfig);
	void HighlightTreeSelectedElem(QString name);
	// update webpage
	void ElementEliminated(QString strElemId);
	void PropertyChanged(QString strElemChanged, QString strPropChanged, QString strNewPropVal);
	void ConnectionChanged(QString strSender, QString strSignal, QString strReceiver, QString strSlot, QString strParameter );
	void InsertConfig(QByteArray strConfigFraction, QString strParentName, int irow);	

	void RequestAutoGenCpp(QString strProjName);
	void RequestAllProperties(QString strParentId);
	void UpdateAllProperties();


};

#endif // MAINWINDOW_H
