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

#include <QApplication>
#include "mainwindow.h"
#include "helperfunctions.h"

MainWindow::MainWindow(int argc, char **argv, QWidget *parent)
	: QMainWindow(parent)
{
	// create the wt server
	mp_WtServerWorker = new WtServerWorker(this);
	// setup the main window ui
	ui.setupUi(this);
	// set all icons color
	SetAllIcons("white");
	// configure model-views
	m_pwidgetmodel = new MyWidgetModel(g_strLocalWidgetsPath, this); // widget model should come first because it loads the icons
	ui.treeviewWtWidgets->setModel(m_pwidgetmodel);
	ui.treeviewWtWidgets->expandAll();
	m_treemodel.setMapIconsByClassName(m_pwidgetmodel->getMapIconsByClassName()); // pass icons from widget model to tree model
	ui.treeviewWtTree->setModel(&m_treemodel);
	ui.treeviewWtProperties->setModel(&m_propertymodel);
	ui.treeviewWtSignalsSlots->setModel(&m_sigslotmodel);
	m_resourcesmodel.setReadOnly(false);
	m_resourcesmodel.setRootPath(QUrl::fromLocalFile("./").toString());
	ui.treeviewWtResources->setDragEnabled(true);
	ui.treeviewWtResources->setAcceptDrops(true);
	ui.treeviewWtResources->setDefaultDropAction(Qt::MoveAction);
	ui.treeviewWtResources->setDropIndicatorShown(true);
	ui.treeviewWtResources->setContextMenuPolicy(Qt::CustomContextMenu);
	ui.treeviewWtResources->setModel(&m_resourcesmodel);
	// set current element empty
	m_strCurrentHoveredElemId    = "";
	m_strCurrentHoveredElemClass = "";
	m_strCurrentSelectedElemId   = "";
	// set proj path
	m_strProjRootPath		     = "";
	m_strProjName                = "Untitled";
	// start webserver first
	m_argc					   = argc;
	m_argv					   = argv;
	// replace qt webview with custom inherited webview (it cannot be promoted in qtdesigner)
	ui.webviewBrowser->setVisible(false);
	ui.webviewBrowser->setParent(0);
	ui.webviewBrowser->deleteLater();
	m_WebView = new MyWebView(ui.scrollBrowser);
	ui.scrollBrowser->setWidget(m_WebView);
	// setup peristent settings
	SetupPersistSettings();
	// set initial fixed size for webview and show
	m_WebView->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	m_WebView->setFixedWidth(m_computer_width);
	m_WebView->show();
	// connect signal and slots
	connect(m_WebView         , SIGNAL(mouseMovedOverElem(QString, QString))                                       , this             , SLOT(on_MouseMovedOverElem(QString, QString))       );
	connect(m_WebView         , SIGNAL(mouseLeftClickedOverElem(QString))                                          , this             , SLOT(on_mouseLeftClickedOverElem(QString)) );
	connect(m_WebView         , SIGNAL(receivedDragOnWebview(QByteArray, QString))                                 , this             , SLOT(on_receivedDragOnWebview(QByteArray, QString)) );
	connect(ui.treeviewWtTree , SIGNAL(backspacePressed())                                                         , this             , SLOT(on_actionDelete_triggered())          );
	connect(this              , SIGNAL(HighlightTreeSelectedElem(QString))                                         , m_WebView        , SLOT(on_HighlightTreeSelectedElem(QString)));
	connect(&m_propertymodel  , SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)), this             , SLOT(on_propertyChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)));
	connect(&m_sigslotmodel   , SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)), this             , SLOT(on_signalSlotChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)));
	connect(&m_dialogProjSel  , SIGNAL(openExistingProject(QString))                                               , this             , SLOT(on_openExistingProject(QString)));
	connect(&m_dialogProjSave , SIGNAL(saveNewProject(QString, QString))                                           , this             , SLOT(on_saveNewProject(QString, QString)));
	connect(ui.treeviewWtTree , SIGNAL(customContextMenuRequested(const QPoint &))                                 , this             , SLOT(on_treeviewWtTree_contextMenu(const QPoint &)) );
	connect(&m_commandHistory , SIGNAL(cleanChanged(bool))                                                         , this             , SLOT(on_cleanChanged(bool)) );
	connect(ui.treeviewWtResources, SIGNAL(customContextMenuRequested(const QPoint &))                             , this             , SLOT(on_treeviewWtResources_contextMenu(const QPoint &))      );
	connect(ui.treeviewWtResources, SIGNAL(trackedFileWasMoved(QString, QString))                                  , this             , SLOT(on_trackedFileWasMoved(QString, QString))                );
	connect(&m_treemodel, SIGNAL(removedTrackedCssFile(QFileInfo)), this, SLOT(on_removedTrackedCssFile(QFileInfo)));
	connect(&m_treemodel, SIGNAL(appendedTrackedCssFile(QFileInfo)), this, SLOT(on_appendedTrackedCssFile(QFileInfo)));
	connect(&m_treemodel, SIGNAL(removedTrackedJsFile(QFileInfo)), this, SLOT(on_removedTrackedJsFile(QFileInfo)));
	connect(&m_treemodel, SIGNAL(appendedTrackedJsFile(QFileInfo)), this, SLOT(on_appendedTrackedJsFile(QFileInfo)));
	// init menu actions
	InitActions();
    // show computer webview size selected
	ui.push_computer->setFocus();
    ui.push_computer->setStyleSheet("background-color: #3d8ec9;");
	// set alternating colors on some views for better visibility
	ui.treeviewWtProperties->setAlternatingRowColors(true);
	ui.treeviewWtSignalsSlots->setAlternatingRowColors(true);
	// create dynamically based on m_vectWtParams port
	m_Address = "http://127.0.0.1:" + m_vectWtParams[WT_VALHTTPPORT];
	// setup the settings dialog
	m_dialogConfig.setDocRootRef      (&m_vectWtParams[WT_PARDOCROOT ]);
	m_dialogConfig.setHttpAddressRef  (&m_vectWtParams[WT_VALHTTPADDR]);
	m_dialogConfig.setHttpPortRef     (&m_vectWtParams[WT_VALHTTPPORT]);
	m_dialogConfig.setPhoneWidthRef   (&m_phone_width   );
	m_dialogConfig.setTabletWidthRef  (&m_tablet_width  );
	m_dialogConfig.setLaptopWidthRef  (&m_laptop_width  );
	m_dialogConfig.setComputerWidthRef(&m_computer_width);
#ifdef Q_OS_WIN
	m_dialogConfig.setIncludeDirRef   (&m_strIncludeDir);
	m_dialogConfig.setLibraryDirRef   (&m_strLibraryDir);
	m_dialogConfig.setBinaryDirRef    (&m_strBinaryDir);
#endif
	m_dialogConfig.setServerPortRef   (&m_strServerPort);
	m_dialogConfig.setWtThemeRef      (&m_strWtTheme);
	m_dialogConfig.setWtTitleRef      (&m_strWtTitle);
	m_dialogConfig.setupDialogWidgets();
	connect(&m_dialogConfig, SIGNAL(UpdateSettings()), this, SLOT(on_ReloadWtServer()));
	// disable CMake functionality by default until project is defined
	ui.actionExport_to_CMake->setEnabled(false);

	LoadDefaultConfig();      // STARTS m_pWtServerWorker
	// Hide functionality in developement
	ui.actionImport_from_HTML->setVisible(false);
	ui.actionImport_from_HTML->setEnabled(false);
	ui.actionClose->setVisible(false);
	ui.actionClose->setEnabled(false);
}

MainWindow::~MainWindow()
{
	// Stop Wt server
	StopWtServer();
	// Delete Wt server
	delete mp_WtServerWorker;
	// Delete old local /resources/ and /temp/ folders
	if (QDir(g_strLocalResourcesPath).exists()) { QDir(g_strLocalResourcesPath).removeRecursively(); }
	if (QDir(g_strLocalTempPath).exists())   { QDir(g_strLocalTempPath).removeRecursively();   }
}


void MainWindow::SetupPersistSettings()
{
	// set default server config settings (depends on m_argv)
	m_vectWtParams.resize(WT_MAXNUMPARAMS);
	m_vectWtParams[WT_EXEPATH]     = QString::fromLocal8Bit(m_argv[0]);
	m_vectWtParams[WT_PARDOCROOT]  = "--docroot";
	m_vectWtParams[WT_VALDOCROOT]  = ".";                 // (http://www.webtoolkit.eu/wt/doc/reference/html/overview.html#config_wthttpd)
	m_vectWtParams[WT_PARHTTPADDR] = "--http-address";
	m_vectWtParams[WT_VALHTTPADDR] = "0.0.0.0";           
	m_vectWtParams[WT_PARHTTPPORT] = "--http-port";
	m_vectWtParams[WT_VALHTTPPORT] = "8081";              
	// set responsive design default sizes
	m_phone_width    = 480;
	m_tablet_width   = 768;
	m_laptop_width   = 992;
	m_computer_width = 1200;
#ifdef Q_OS_WIN
	// set default cmake directories
	m_strIncludeDir  = "";
	m_strLibraryDir  = "";
	m_strBinaryDir   = "";
#endif
	//
	m_strServerPort = "8080";  
	m_strWtTheme    = g_strThemeBootstrat3;
	m_strWtTitle    = "Untitled Wt Webapp";

	// see company and appname in main.cpp
	// begin trying to load settings (from settgins dialog)

	// begin server config settings
	m_settings.beginGroup("server");
	m_vectWtParams[WT_VALHTTPPORT] = m_settings.value("wt_valhttpport", m_vectWtParams[WT_VALHTTPPORT]).toString();
	m_settings.endGroup();

	// begin responsive design default sizes
	m_settings.beginGroup("responsive");
	m_phone_width    = m_settings.value("m_phone_width"   , m_phone_width	 ).toInt();
	m_tablet_width   = m_settings.value("m_tablet_width"  , m_tablet_width	 ).toInt();
	m_laptop_width   = m_settings.value("m_laptop_width"  , m_laptop_width	 ).toInt();
	m_computer_width = m_settings.value("m_computer_width", m_computer_width ).toInt();
	m_settings.endGroup();

#ifdef Q_OS_WIN
	// begin cmake config settings
	m_settings.beginGroup("cmake");
	m_strIncludeDir = m_settings.value("m_strincludedir", m_strIncludeDir).toString();
	m_strLibraryDir = m_settings.value("m_strlibrarydir", m_strLibraryDir).toString();
	m_strBinaryDir  = m_settings.value("m_strbinarydir" , m_strBinaryDir ).toString();
	// try to find is empty (empty if cannot find it)
	if (m_strIncludeDir.isEmpty()) 
	{ 
		m_strIncludeDir = FindWtIncludeDir(); 
		if (m_strIncludeDir.isEmpty())
		{
			m_strIncludeDir = "Could not be found";
		}
		m_settings.setValue("m_strincludedir", m_strIncludeDir);
	}
	if (m_strLibraryDir.isEmpty()) 
	{ 
		m_strLibraryDir = FindWtLibraryDir(); 
		if (m_strLibraryDir.isEmpty())
		{
			m_strLibraryDir = "Could not be found";
		}
		m_settings.setValue("m_strlibrarydir", m_strLibraryDir);
	}
	if (m_strBinaryDir.isEmpty())  
	{ 
		m_strBinaryDir  = FindWtBinaryDir() ; 
		if (m_strBinaryDir.isEmpty())
		{
			m_strBinaryDir = "Could not be found";
		}
		m_settings.setValue("m_strbinarydir", m_strBinaryDir);
	}
	m_settings.endGroup();
#endif

	// begin application config settings
	m_settings.beginGroup("application");
	m_strServerPort = m_settings.value("app_serverport", m_strServerPort).toString();
	// NOTE : wt_theme is saved in *wui config file, in a per-project fashion
	m_settings.endGroup();
}

#ifdef Q_OS_WIN
void MainWindow::FindLibFileNames()
{ // list of dependencies according to http://redmine.webtoolkit.eu/projects/wt/wiki/Installing_Wt_on_Ubuntu?version=34
	// NOTE : returns empty if one or more libs are missing (wt, wtd, wthttp, wthttpd must be there if m_strLibraryDir was found)
	m_listLibFileNames = FindWtLibFiles(m_strLibraryDir,
		                                QStringList() << "wt"      // wt, wtd, wthttp, wthttpd, etc
													  << "date_time"
													  << "filesystem"
													  << "program_options"
													  << "regex"
													  << "signals"
													  << "system"
													  << "thread"
													  << "random"
													  );
	// NOTE : later if empty then set on CMake's boost finder
}
#endif

void MainWindow::SetAllIcons(QString svgcolor)
{
	QIcon icon;

	SetIconColor(":/file", icon, svgcolor);
	ui.actionNew->setIcon(icon);

	SetIconColor(":/folder-open", icon, svgcolor);
	ui.actionOpen->setIcon(icon);

	SetIconColor(":/floppy-o", icon, svgcolor);
	ui.actionSave->setIcon(icon);

	SetIconColor(":/floppy-o", icon, svgcolor);
	ui.actionSave_as->setIcon(icon);

	SetIconColor(":/times", icon, svgcolor);
	ui.actionClose->setIcon(icon);

	SetIconColor(":/times-circle", icon, svgcolor);
	ui.actionQuit->setIcon(icon);

	SetIconColor(":/cogs", icon, svgcolor);
	ui.actionPreferences->setIcon(icon);

	SetIconColor(":/book", icon, svgcolor);
	ui.actionTutorials->setIcon(icon);

	SetIconColor(":/info-circle", icon, svgcolor);
	ui.actionAbout->setIcon(icon);

	SetIconColor(":/undo", icon, svgcolor);
	ui.actionUndo->setIcon(icon);

	SetIconColor(":/repeat", icon, svgcolor);
	ui.actionRedo->setIcon(icon);

	SetIconColor(":/scissors", icon, svgcolor);
	ui.actionCut->setIcon(icon);

	SetIconColor(":/files-o", icon, svgcolor);
	ui.actionCopy->setIcon(icon);

	SetIconColor(":/clipboard", icon, svgcolor);
	ui.actionPaste->setIcon(icon);

	SetIconColor(":/trash", icon, svgcolor);
	ui.actionDelete->setIcon(icon);

	SetIconColor(":/file-code-o", icon, svgcolor);
	ui.actionImport_from_HTML->setIcon(icon);

	SetIconColor(":/cogs", icon, svgcolor); // Repeated
	ui.actionExport_to_CMake->setIcon(icon);

	SetIconColor(":/trash", icon, svgcolor); // Repeated
	ui.push_signalslot_remove->setIcon(icon);

	SetIconColor(":/plus-circle", icon, svgcolor);
	ui.push_signalslot_add->setIcon(icon);

	SetIconColor(":/plus-circle", icon, svgcolor); // Repeated
	ui.push_resource_add->setIcon(icon);

	SetIconColor(":/trash", icon, svgcolor); // Repeated
	ui.push_resource_remove->setIcon(icon);

	SetIconColor(":/pencil-square-o", icon, svgcolor);
	ui.push_resource_edit->setIcon(icon);

	SetIconColor(":/refresh", icon, svgcolor); // Repeated
	ui.push_resource_reload->setIcon(icon);

	SetIconColor(":/refresh", icon, svgcolor); // Repeated
	ui.push_refresh->setIcon(icon);

	SetIconColor(":/file-text-o", icon, svgcolor);
	ui.actionC_Code->setIcon(icon);

	SetIconColor(":/file-code-o", icon, svgcolor); // Repeated
	ui.actionHtml_Code->setIcon(icon);

	SetIconColor(":/eye", icon, svgcolor);
	ui.menuPreview->setIcon(icon);

	SetIconColor(":/mobile-phone", icon, svgcolor);
	ui.push_phone->setIcon(icon);
	ui.push_phone->setIconSize(QSize(25, 30));

	SetIconColor(":/tablet", icon, svgcolor);
	ui.push_tablet->setIcon(icon);
	ui.push_tablet->setIconSize(QSize(25, 30));

	SetIconColor(":/laptop", icon, svgcolor);
	ui.push_laptop->setIcon(icon);
	ui.push_laptop->setIconSize(QSize(34, 34));

	SetIconColor(":/desktop", icon, svgcolor);
	ui.push_computer->setIcon(icon);
	ui.push_computer->setIconSize(QSize(28, 30));
}


void MainWindow::on_MouseMovedOverElem(QString elem, QString strClass)
{
	m_strCurrentHoveredElemId    = elem;
	m_strCurrentHoveredElemClass = strClass;
	UpdateStatusBar();
}

void MainWindow::on_mouseLeftClickedOverElem(QString elem)
{
	m_strCurrentSelectedElemId = elem;
	UpdateStatusBar();
	// make clicked element the current indexin tree view
	QModelIndex clickedElemIndex = m_treemodel.getIndexByName(m_strCurrentSelectedElemId);
	ui.treeviewWtTree->setCurrentIndex(clickedElemIndex);
	// update property tree
	WDomElem *welem = m_treemodel.getElemByIndex(clickedElemIndex);
	if (welem)
	{
		UpdatePropertyTree(welem->getElem());
		UpdateSignalSlots(welem->getElem());
	}
	else
	{
		m_propertymodel.clearCurrentConfig();
		m_sigslotmodel.clearCurrentConfig();
	}
	
}

void MainWindow::on_LoadPage()
{
	m_WebView->load(QUrl(m_Address));
}

void MainWindow::on_ReloadPage()
{
	//on_LoadPage();
	m_WebView->reload();
}

void MainWindow::InitActions()
{
	// Init dock widgets actions on view menu
	ui.menuView->addAction(ui.dockWtTree->toggleViewAction());
	ui.menuView->addAction(ui.dockWtProperties->toggleViewAction());
	ui.menuView->addAction(ui.dockWtWidgets->toggleViewAction());
	ui.menuView->addAction(ui.dockWtSignalsSlots->toggleViewAction());
    ui.menuView->addAction(ui.dockWtResources->toggleViewAction());
	ui.menuView->addAction(ui.dockWtStylesheets->toggleViewAction());
	ui.menuView->addAction(ui.dockWtJavascripts->toggleViewAction());
	// Close by default on init
	ui.dockWtSignalsSlots->close();
	ui.dockWtResources->close();
	ui.dockWtStylesheets->close();
	ui.dockWtJavascripts->close();
	// Setup Tooltips
	ui.dockWtResources->setToolTip("Drag-and-drop to restructure the directorty\n\
Use right-click menu to perform more actions like:\n\
-> Add and remove files or directories\n\
-> Add and remove Stylesheets and Javascripts from Wt Application\
");
	ui.dockWtResources->setToolTipDuration(6000);
	ui.dockWtStylesheets->setToolTip("Use the 'Wt Resources Directory' dock to add or remove items of this list.");
	ui.dockWtStylesheets->setToolTipDuration(2500);
	ui.dockWtJavascripts->setToolTip("Use the 'Wt Resources Directory' dock to add or remove items of this list.");
	ui.dockWtJavascripts->setToolTipDuration(2500);
}


void MainWindow::LoadDefaultConfig()
{
	// shutdown Wt server
	StopWtServer();
	// set default config in m_currentState
	LoadProjectPath(g_strLocalRootPath + g_strDefaultProjPath);
	// start Wt server
	StartWtServer();
}

void MainWindow::StartWtServer()
{
	// (re)configure ip address for webview
	m_Address = "http://127.0.0.1:" + m_vectWtParams[WT_VALHTTPPORT];
	// set settings related attrs that might have changed in settings dialog
	QDomElement docElem = m_treemodel.getDocument().documentElement();
	docElem.setAttribute(g_strThemeAttr, m_strWtTheme);
	docElem.setAttribute(g_strTitleAttr, m_strWtTitle);
	// start Wt server
	mp_WtServerWorker->ConfigureWtServer(m_vectWtParams);
	mp_WtServerWorker->start();
	// load page [IMPORTANT] : DO NOT DELETE LINE BELOW OR PAGE WILL NEVER LOAD
	on_LoadPage();
}

void MainWindow::StopWtServer() 
{
	// load something else
	m_WebView->load(QUrl("http://www.webtoolkit.eu/wt"));
	// shutdown Wt server
	if (mp_WtServerWorker->isRunning())
	{
		mp_WtServerWorker->quit();
	}
	// wait if necessary
	int counter = 0;
	while (mp_WtServerWorker->isRunning())
	{
		QThread::msleep(100);
		QApplication::processEvents();
		counter++;
		if (counter == 15)
		{
			break;
		}
	}
	if (counter == 15)
	{
		qDebug() << "[ERROR] Could not stop Wt Server cleanly";
		mp_WtServerWorker->quit();
		mp_WtServerWorker->terminate();
	}
	// [NOTE] very important
	m_WebView->settings()->clearMemoryCaches();
}

void MainWindow::LoadFileConfig(QByteArray config) 
{
	// validate config then if good load it
	QString errorStr;
	int errorLine;
	int errorColumn;
	QDomDocument doc;
	if (!doc.setContent(config, &errorStr, &errorLine, &errorColumn)) 
	{
		QMessageBox::critical(0, QObject::tr("DOM Parser Error"), QObject::tr("Could not load file.\nParse error at line %1, " "column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr));
		return;
	}
	QDomElement root = doc.documentElement();
	if (root.tagName() != g_strDocumTag)
	{
		
		QMessageBox::critical(0, QObject::tr("Invalid File"), QObject::tr("Invalid file contents."));
		return;
	}

	// load into tree model for validation
	if (!m_treemodel.loadNewConfiguration(config))
	{
		QMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.setText(tr("Error while loading the configuration : \n") + config);
		msgBox.exec();
	}
	ui.treeviewWtTree->expandAll();

	// reload resources model
	ui.treeviewWtResources->setRootIndex(m_resourcesmodel.index(g_strLocalResourcesPath));

	// reload the page, create new wt session
	on_ReloadPage();

}

void MainWindow::on_actionNew_triggered()
{
	// check on changes and ask if save
	if (!m_commandHistory.isClean())
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle("You have unsaved changes.");
		msgBox.setText("Would you like to save your current project changes before creating a new project?");
		msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Cancel);
		int ret = msgBox.exec();
		if (ret == QMessageBox::Save)
		{
			on_actionSave_triggered();
		}
		else if(ret == QMessageBox::Cancel)
		{
			return;
		}
	}
	// clear
	m_strProjRootPath = "";
	m_strProjName     = "";
	// set window title
	// Load defaut proj
	LoadDefaultConfig();
}

void MainWindow::on_actionSave_triggered() // TODO : QProgressDialog on SAVE operation STARTS here
{
	if(m_strProjRootPath.isEmpty())
	{
		on_actionSave_as_triggered();
		return;
	}
	// setup progress dialog
	mp_ProcessDialog = new QProgressDialog(QString("Saving project <%1>...").arg(m_strProjName), QString(), 0, 3, this);
	mp_ProcessDialog->setWindowModality(Qt::WindowModal);
	mp_ProcessDialog->setValue(0);
	mp_ProcessDialog->show();
	// copy the full local resources path to path/projname/resources
	CopyAllFilesInPath(g_strLocalResourcesPath, m_strProjRootPath + "/" + g_strResourcesPathConst, QStringList() << "*.*");
	// save
	if (m_strProjName.isEmpty())
	{
		m_strProjName = m_strProjRootPath.split("/").last();
		if (m_strProjName.isEmpty())
		{
			m_strProjName = m_strProjRootPath.split("/").at(m_strProjRootPath.split("/").size()-2);
		}
	}
	// increase progress in dialog
	mp_ProcessDialog->setValue(1);
	// set window title
	setWindowTitle("WtDesigner - " + m_strProjName);
	// enable CMake functionality = project is defined
	ui.actionExport_to_CMake->setEnabled(true);
	// save config file
	QString strConfigFile = m_strProjRootPath + "/" + m_strProjName.toLower() + ".wui";
	QFile file(strConfigFile);
	if (file.open(QIODevice::ReadWrite | QFile::Truncate))
	{
		// generate configuration xml text from object tree and store in file
		QTextStream stream(&file);
		// get config from tree model
		stream << m_treemodel.getDocument().toByteArray();
	}
	else
	{
		QMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.setText(tr("Could not create file ") + strConfigFile);
		msgBox.exec();
	}
	file.close();

	// request cpp config to overwrite autogen cpp code by emit signal to get autogen cpp
	Q_EMIT RequestAutoGenCpp(m_strProjName);

	// clear undo stack
	m_commandHistory.clear();
	// increase progress in dialog
	mp_ProcessDialog->setValue(2);
}

void MainWindow::on_actionSave_as_triggered()
{
	// Show save/as project dialog
	m_dialogProjSave.show();
}

void MainWindow::on_actionOpen_triggered()
{
	// ask if there is stuff to change
	if (!m_commandHistory.isClean())
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle("You have unsaved changes.");
		msgBox.setText("Would you like to save your current project changes before opening a new project?");
		msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Cancel);
		int ret = msgBox.exec();
		if (ret == QMessageBox::Save)
		{
			on_actionSave_triggered();
		}
		else if (ret == QMessageBox::Cancel)
		{
			return;
		}
	}
	// Show select/open project dialog
	m_dialogProjSel.show();
}


void MainWindow::on_openExistingProject(QString path) 
{
	// setup progress dialog
	mp_ProcessDialog = new QProgressDialog(QString("Opening project <%1>...").arg(QDir(path).dirName()), QString(), 0, 2, this);
	mp_ProcessDialog->setWindowModality(Qt::WindowModal);
	mp_ProcessDialog->setValue(0);
	mp_ProcessDialog->show();
	// clear
	m_strProjRootPath = "";
	m_strProjName     = "";
	// If a valid project path was selected in the select/open project dialog
	LoadProjectPath(path);
	// increase progress in dialog
	mp_ProcessDialog->setValue(1);
	// kill the server and reload
	on_ReloadWtServer();
	// increase progress in dialog and kill
	mp_ProcessDialog->setValue(2);
	mp_ProcessDialog->deleteLater();
}


void MainWindow::on_actionImport_from_HTML_triggered()
{
	QByteArray baData;
	QString strNewFile;
	// open new file        :                TODO change to path approach as well
	strNewFile = QFileDialog::getOpenFileName(this, tr("Open file"), (QDir::homePath()), tr("HTML file ") + "(*.html)");
	if (strNewFile.isEmpty()) { return; }

	QFile file(strNewFile);
	if (file.open(QIODevice::ReadOnly))
	{
		// Delete old local /style/ and /images/ folders
		if (QDir(g_strLocalResourcesPath).exists()) { QDir(g_strLocalResourcesPath).removeRecursively(); }
		if (QDir(g_strLocalTempPath).exists())   { QDir(g_strLocalTempPath).removeRecursively();   }
		// read data form file
		QByteArray htmlsource = file.readAll();
		// get wui configuration from html
		QDir fpath = QFileInfo(file).absoluteDir();
		baData = GetWtFromHtml(htmlsource, NULL, "", fpath.absolutePath().toUtf8());
		// Copy each stylesheet and possible resources. Update baData path to local /styles/
		GetStylesFromHtml(baData);
		// load into tree model for validation
		if (!m_treemodel.loadNewConfiguration(baData))
		{
			QMessageBox msgBox;
			msgBox.setIcon(QMessageBox::Critical);
			msgBox.setText(tr("Error while loading the configuration : \n") + baData);
			msgBox.exec();
		}
		ui.treeviewWtTree->expandAll();
		// reload resources model
		ui.treeviewWtResources->setRootIndex(m_resourcesmodel.index(g_strLocalResourcesPath));
	}
	else
	{
		QMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.setText(tr("Could not open ") + strNewFile);
		msgBox.exec();
		baData = "";
	}

	file.close();
	// reload page
	on_ReloadPage();
}

void MainWindow::on_actionUndo_triggered()
{
	m_commandHistory.undo();
}

void MainWindow::on_actionRedo_triggered()
{
	m_commandHistory.redo();
}

void MainWindow::on_actionCut_triggered()
{
	// only cut if one of the tree related widgets is focused
	if (isWidgetFocused("treeviewWtSignalsSlots"))
	{
		return;
	}
	else
	{
		QModelIndex currentindex = ui.treeviewWtTree->currentIndex();
		if (!currentindex.isValid()) { return; }
		// deep copy the dom part to another document
		QDomDocument doc;
		QDomElement rootElem = doc.createElement(g_strValidNodeTag);
		rootElem = m_treemodel.getElemByIndex(currentindex)->getElem().cloneNode().toElement();
		doc.appendChild(rootElem);
		// set tconfig text into clipboard
		QClipboard *clipboard = QApplication::clipboard();
		clipboard->setText(doc.toString());
		// remove from tree and webpage and store in command stack (done here better because in paste crashes with no simple fix)
        QString strTemp = rootElem.attribute(g_strIdAttr);
        MyUndoRemoveElem *remCommand = new MyUndoRemoveElem(&m_treemodel, strTemp, this);
		m_commandHistory.push(remCommand);
	}
}

void MainWindow::on_actionCopy_triggered()
{
	// only copy if one of the tree related widgets is focused
	if (isWidgetFocused("treeviewWtSignalsSlots"))
	{
		return;
	}
	else
	{
		QModelIndex currentindex = ui.treeviewWtTree->currentIndex();
		if (!currentindex.isValid()) { return; }
		// deep copy the dom part to another document
		QDomDocument doc;
		QDomElement rootElem = doc.createElement(g_strValidNodeTag);
		rootElem = m_treemodel.getElemByIndex(currentindex)->getElem().cloneNode().toElement();
		doc.appendChild(rootElem);
		// set tconfig text into clipboard
		QClipboard *clipboard = QApplication::clipboard();
		clipboard->setText(doc.toString());
	}
}

void MainWindow::on_actionPaste_triggered()
{
	// only copy if one of the tree related widgets is focused
	if (isWidgetFocused("treeviewWtSignalsSlots"))
	{
		return;
	}
	else
	{
		// get current selected index of the wt tree
		QModelIndex currentindex = ui.treeviewWtTree->currentIndex();
		if (!currentindex.isValid()) { return; }

		const QClipboard *clipboard = QApplication::clipboard();
		const QMimeData *mimeData   = clipboard->mimeData();

		if (mimeData->hasText()) 
		{
			// get config chunk and validate
			QByteArray baconfigChunk = mimeData->text().toUtf8();
			QString errorStr;
			int     errorLine;
			int     errorColumn;
			QDomDocument doc;
			if (!doc.setContent(baconfigChunk, &errorStr, &errorLine, &errorColumn))
			{
				QMessageBox::critical(0, QObject::tr("Clipboard Paste Error"), QObject::tr("The clipboard does not contain valid configuration.\nParse error at line %1, " "column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr));
				return;
			}
			int        intRow;
			QString    strElemId;
			QString    strParentElemId, strCurrentElemId, strCurrParentElemId;
			MyUndoAppendElem *appCommand;
			// get subtree root id
			strElemId = m_treemodel.getElemByIndex(currentindex)->getElem().attribute(g_strIdAttr, "");
			if (strElemId.isEmpty()) { qDebug() << "[ERROR] Invalid element in clipboard paste"; return; }

			// Find closer valid container
			strParentElemId     = m_WebView->GetCloserContainerId();
			strCurrentElemId    = m_treemodel.getElemByIndex(currentindex)->getElem().attribute(g_strIdAttr, "");
			strCurrParentElemId = m_treemodel.getElemByIndex(currentindex)->getParent()->getElem().attribute(g_strIdAttr, "");
			// Decide how to add
			if (strParentElemId.compare(strCurrentElemId) == 0)
			{ // append as child of current element
				intRow = m_treemodel.getElemByIndex(currentindex)->countChild();
			}
			else if (strParentElemId.compare(strCurrParentElemId) == 0)
			{ // append as a sibling 
				intRow = m_treemodel.getElemByIndex(currentindex)->getRow() + 1;
			}
			else
			{ // append as a child of the parent
				intRow = m_treemodel.getElemByName(strParentElemId)->countChild();
			}
			// append element
			appCommand = new MyUndoAppendElem(&m_treemodel, baconfigChunk, strParentElemId, intRow, this);
			m_commandHistory.push(appCommand);
		}
		else 
		{
			qDebug() << "[ERROR] Clipboard does not contain valid configuration to paste.";
		}
	}
}

void MainWindow::on_actionDelete_triggered()
{
	if (isWidgetFocused("treeviewWtSignalsSlots"))
	{
		on_push_signalslot_remove_clicked();
	}
	else if (isWidgetFocused("treeviewWtResources"))
	{
		on_push_resource_remove_clicked();
	}
	else
	{
		QModelIndex currentindex = ui.treeviewWtTree->currentIndex();
		if (!currentindex.isValid()) { return; }
		// getElemId
		QString strElemId       = m_treemodel.getElemByIndex(currentindex)->getElem().attribute(g_strIdAttr);
		QString strParentElemId = m_treemodel.getElemByIndex(currentindex)->getParent()->getElem().attribute(g_strIdAttr);
		// check whether elem is removable (i.e. not root)
		if (!strElemId.isEmpty() && !strParentElemId.isEmpty())
		{
			// remove from tree and webpage and store in command stack
			MyUndoRemoveElem *remCommand = new MyUndoRemoveElem(&m_treemodel, strElemId, this);
			m_commandHistory.push(remCommand);
		}
		else
		{
			qDebug() << "[ERROR] Cannot remove root element in MainWindow::on_actionDelete_triggered";
		}
	}
	
}

void MainWindow::LoadProjectPath(QString strProjPathToLoad)
{
	// temporarily move resources model
	ui.treeviewWtResources->setRootIndex(m_resourcesmodel.index(g_strLocalRootPath));
	// Delete old local /resources/ and /temp/ folders
	if (QDir(g_strLocalResourcesPath).exists()) { QDir(g_strLocalResourcesPath).removeRecursively(); }
	if (QDir(g_strLocalTempPath).exists())      { QDir(g_strLocalTempPath).removeRecursively();   }
	// Load embedded or from pre-made simple intro project
	QByteArray baData;
	// Check if valid project path 
	if (!CheckValidProjPath(strProjPathToLoad)) { return; }
	// Set internal project path 
	if (strProjPathToLoad.contains("./default"))
	{
		m_strProjRootPath = "";
		m_strProjName     = "Untitled";
		// set window title
		setWindowTitle("WtDesigner - " + m_strProjName + " *");
	}
	else
	{
		m_strProjRootPath = strProjPathToLoad;
		m_strProjName     = m_strProjRootPath.split("/").last();
		if (m_strProjName.isEmpty())
		{
			m_strProjName = m_strProjRootPath.split("/").at(m_strProjRootPath.split("/").size() - 2);
		}
		// set window title
		setWindowTitle("WtDesigner - " + m_strProjName);
		// enable CMake functionality = project is defined
		ui.actionExport_to_CMake->setEnabled(true);
	}
	// Get project file name
	QString strProjFileName = GetProjFileName(strProjPathToLoad);
	// Copy project root /resources/ folder to local root
	QString strProjResourcesPath = strProjPathToLoad + g_strResourcesPathConst;
	// Show message if could not import project (copy files locally)
	while (!CopyAllFilesInPath(strProjResourcesPath, g_strLocalResourcesPath, QStringList() << "*.*"))
	{
		QMessageBox msgBox;
		msgBox.setText("Error while importing project.");
		msgBox.setInformativeText("There were some problems importing your project. Would you like to try again?");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setIcon(QMessageBox::Question);
		msgBox.setDefaultButton(QMessageBox::Yes);
		QMessageBox::StandardButton ret = (QMessageBox::StandardButton)msgBox.exec();
		if (ret == QMessageBox::No) { return; }
	}
	// Create temporary folder
	QDir().mkdir(g_strLocalTempPath);
	// Open and load project file
	QFile file(strProjPathToLoad + strProjFileName);
	if (file.open(QIODevice::ReadOnly))
	{
		// read data from file
		QByteArray baData = file.readAll();
		// load into tree model for validation
		if (!m_treemodel.loadNewConfiguration(baData))
		{
			QMessageBox msgBox;
			msgBox.setIcon(QMessageBox::Critical);
			msgBox.setText(tr("Error while loading the configuration : \n") + baData);
			msgBox.exec();
		}
		//
		QDomElement docElem = m_treemodel.getDocument().documentElement();
		m_strWtTheme = docElem.attribute(g_strThemeAttr, m_strWtTheme);
		m_strWtTitle = docElem.attribute(g_strTitleAttr, m_strWtTitle);
		//
		ui.treeviewWtTree->expandAll();
		// reload resources model
		ui.treeviewWtResources->setRootIndex(m_resourcesmodel.index(g_strLocalResourcesPath));
		// reload stylesheets list
		ui.listWtStylesheets->clear();
        QList<QFileInfo> allStylesheets = m_treemodel.getAllTrackedCssFiles();
		for (int i = 0; i < allStylesheets.count(); i++)
		{
			new QListWidgetItem(allStylesheets.at(i).filePath(), ui.listWtStylesheets);
		}
		// reload javascripts list
		ui.listWtJavascripts->clear();
        QList<QFileInfo> allJavascripts = m_treemodel.getAllTrackedJsFiles();
		for (int i = 0; i < allJavascripts.count(); i++)
		{
			new QListWidgetItem(allJavascripts.at(i).filePath(), ui.listWtJavascripts);
		}
	}
	else
	{
		QMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.setText(tr("Could not open ") + strProjFileName);
		msgBox.exec();
		baData = "";
	}

	file.close();

	// clear undo stack
	m_commandHistory.clear();
}

void MainWindow::on_RequestCurrentConfig()
{
	// send current configuration from tree model
	Q_EMIT SendCurrentConfig(m_treemodel.getDocument().toByteArray());
}

void MainWindow::on_treeviewWtTree_clicked(const QModelIndex &index)
{
	if (!index.isValid())
	{
		qDebug() << "[ERROR] Invalid index in MainWindow::on_treeviewWtTree_clicked.";
		return;
	}
	WDomElem *welem            = m_treemodel.getElemByIndex(index);
	// set as current selected elem
	m_strCurrentSelectedElemId = welem->getElem().attribute(g_strIdAttr);
	UpdateStatusBar();
	// highlight elem in webview
	Q_EMIT HighlightTreeSelectedElem(m_strCurrentSelectedElemId);
	// update property tree
	UpdatePropertyTree(welem->getElem());
	UpdateSignalSlots(welem->getElem());
}

void MainWindow::UpdateStatusBar()
{
		statusBar()->showMessage(tr("Current selected element Id : [") +
		m_strCurrentSelectedElemId +
		"]      -      " +
		tr("Current hovered element Id : [") +
		m_strCurrentHoveredElemId +
		tr("]      Class : [") +
		m_strCurrentHoveredElemClass + "]");
}

void MainWindow::UpdatePropertyTree(QDomElement elem)
{
	//m_propertymodel
	QString strClassName = elem.attribute(g_strClassAttr, "");

	if (strClassName.isEmpty())
	{
		qDebug() << "[ERROR] Invalid class name in MainWindow::UpdatePropertyTree.";
		return;
	}

	// get metaobject given class name
	QMetaObject metaObj = GetMetaObjectByClassName(strClassName);

	QString            strOldClassName = "";
	QString            strPropName     = "";
	const QMetaObject *pmetaSuperObj   = &metaObj;

	// new class type, therefore reset model
	m_propertymodel.clearCurrentConfig();
	// set the QDomElement
	m_propertymodel.setDomElement(elem);
	while (pmetaSuperObj != NULL)
	{
		// get class name
		strClassName = QString("%1").arg(pmetaSuperObj->className());
		// exit if qobject (top level reached)
		if (strClassName.compare("QObject") == 0) { break; }
		strClassName.remove(1, 3);
		// add class to model
        m_propertymodel.appendNode(strClassName, QModelIndex());
		// iterate properties
		for (int i = 0; i < pmetaSuperObj->propertyCount(); i++)
		{
			strPropName = QString("%1").arg(pmetaSuperObj->property(i).name());
			if (!strPropName.contains("Wt")) { continue; }
			// IGNORE Wt_htmlTagName for other than WContainerWidget OR WAnchor
			if ( ( elem.attribute(g_strClassAttr, "").compare("WContainerWidget") != 0 
				 && elem.attribute(g_strClassAttr, "").compare("WAnchor") != 0      ) 
				 && strPropName.compare("Wt_htmlTagName") == 0 )
			{ continue; } // skip
			// remove prop from previous class
			if (!strOldClassName.isEmpty())
			{
				m_propertymodel.removeNodeByName(strPropName, m_propertymodel.getIndexByName(strOldClassName));
			}
			// add prop to current class
			m_propertymodel.appendNode(strPropName, m_propertymodel.getIndexByName(strClassName));
		}

		pmetaSuperObj = pmetaSuperObj->superClass();
		strOldClassName = strClassName;
	}
	// display from parents to children
	m_propertymodel.reverseChildren();
	// remove classes with no properties
	WPropNode *rootnode = m_propertymodel.getRootNode();
	for (int i = 0; i < rootnode->countChild(); i++)
	{
		if (rootnode->getChild(i)->countChild() == 0)
		{
            QString strTemp = rootnode->getChild(i)->getString();
            m_propertymodel.removeNodeByName(strTemp, QModelIndex());
			i--;
		}
	}
	// expand property tree
	ui.treeviewWtProperties->expandAll();

}

void MainWindow::UpdateSignalSlots(QDomElement elem)
{
	m_sigslotmodel.loadNewConfiguration(elem);
	ui.treeviewWtSignalsSlots->expandAll();
}

void MainWindow::on_push_signalslot_add_clicked()
{
	// append to tree and webpage
	MyUndoAppendConnect *appendCommand = new MyUndoAppendConnect(&m_sigslotmodel, this);
	m_commandHistory.push(appendCommand);
}

void MainWindow::on_push_signalslot_remove_clicked()
{
	QModelIndex index = ui.treeviewWtSignalsSlots->currentIndex();
	if (!index.isValid()) { return; }
	// remove from model and webpage and store in command stack 
	MyUndoRemoveConnect *remCommand = new MyUndoRemoveConnect(&m_sigslotmodel, m_sigslotmodel.getElemByIndex(index)->getRow(), this);
	m_commandHistory.push(remCommand);
	
}

void MainWindow::on_propertyChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles /*= QVector<int> ()*/)
{
	Q_UNUSED(bottomRight)
	Q_UNUSED(roles)
	// get objects from model
	QDomElement elem       = m_propertymodel.getRootElem();
	WPropNode *wnode       = m_propertymodel.getNodeByIndex(topLeft);
	// get names from objects
	QString strElemChanged = wnode->strOldId;
	QString strPropChanged = wnode->getString();
	QString strOldPropVal  = wnode->strOldVal;
	QString strNewPropVal  = elem.attribute(strPropChanged);
	// early exit if no change
	if (strOldPropVal.compare(strNewPropVal) == 0)
	{
		return;
	}

	MyUndoEditProperty *editPropCommand = new MyUndoEditProperty(this, strElemChanged, strPropChanged, strOldPropVal, strNewPropVal);
	m_commandHistory.push(editPropCommand);
	
}

void MainWindow::on_signalSlotChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles /*= QVector<int> ()*/)
{
	Q_UNUSED(bottomRight)
	Q_UNUSED(roles)
	// get objects from model
	QDomElement elem       = m_sigslotmodel.getRootItem()->getElem();
	QDomElement signalelem = m_sigslotmodel.getElemByIndex(topLeft)->getElem();
	// get names from objects
	QString strSender      = elem.attribute(g_strIdAttr);
	QString strSignal      = signalelem.attribute("Wt_Signal"   );
	QString strReceiver    = signalelem.attribute("Wt_Receiver" );
	QString strSlot        = signalelem.attribute("Wt_Slot"     );
	QString strParameter   = signalelem.attribute("Wt_Parameter");
	// emit change (validation and book keeping is on mainpage side)
	// also disconnection of previous connection is handled by m_sigslotmodel (Otherwise keeps being buggy)
	Q_EMIT ConnectionChanged(strSender, strSignal, strReceiver, strSlot, strParameter );
}

void MainWindow::on_push_resource_add_clicked()
{
	ShowNewFileSystemMenu(QCursor::pos());
}

void MainWindow::on_push_resource_remove_clicked()
{
	QMessageBox msgBox;
	int ret;

	QModelIndex index = ui.treeviewWtResources->currentIndex();

	if (!index.isValid()) { return; }
	QFileInfo info = m_resourcesmodel.fileInfo(index);

	if (info.isDir())
	{
		msgBox.setText(tr("Selected folder ") + info.filePath());
		msgBox.setInformativeText("Do you want to remove it from your project and all its contents?");
	}
	else if (info.isFile())
	{
		msgBox.setText("Selected file " + info.filePath());
		msgBox.setInformativeText("Do you want to remove it from your project?");
	}
	msgBox.setIcon(QMessageBox::Warning);
	msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Cancel);
	ret = msgBox.exec();

	if(ret == QMessageBox::Ok)
	{
		if (info.isDir())
		{
			// remove CSS recursivelly
			CheckIfStylesheetPath(info.filePath(), false);
			// remove JS recursivelly
			CheckIfJavascriptPath(info.filePath(), false);
			QDir(info.filePath()).removeRecursively();
		}
		else if (info.isFile())
		{
			// remove CSS
			CheckIfStylesheet("./" + QDir(".").relativeFilePath(info.filePath()), false);
			// remove JS
			CheckIfJavascript("./" + QDir(".").relativeFilePath(info.filePath()), false);
			QFile(info.filePath()).remove();
		}
	}

}


void MainWindow::on_push_resource_edit_clicked()
{
	QModelIndex index = ui.treeviewWtResources->currentIndex();

	if (!index.isValid()) { return; }
	QFileInfo info = m_resourcesmodel.fileInfo(index);

	if (info.isFile())
	{
		QDesktopServices::openUrl(QUrl("file:///" + info.filePath(), QUrl::TolerantMode));
	}

}


void MainWindow::on_treeviewWtResources_doubleClicked(const QModelIndex &index)
{
	if (!index.isValid()) { return; }

	on_push_resource_edit_clicked();
}

void MainWindow::on_push_resource_reload_clicked()
{
	on_ReloadWtServer();
}

void MainWindow::on_push_computer_clicked()
{
	ui.push_computer->setStyleSheet("background-color: #3d8ec9;");
	ui.push_laptop->setStyleSheet("");
	ui.push_tablet->setStyleSheet("");
	ui.push_phone->setStyleSheet("");
	m_WebView->setFixedWidth(m_computer_width);
}

void MainWindow::on_push_laptop_clicked()
{
	ui.push_computer->setStyleSheet("");
	ui.push_laptop->setStyleSheet("background-color: #3d8ec9;");
	ui.push_tablet->setStyleSheet("");
	ui.push_phone->setStyleSheet("");
	m_WebView->setFixedWidth(m_laptop_width);
}

void MainWindow::on_push_tablet_clicked()
{
	ui.push_computer->setStyleSheet("");
	ui.push_laptop->setStyleSheet("");
	ui.push_tablet->setStyleSheet("background-color: #3d8ec9;");
	ui.push_phone->setStyleSheet("");
	m_WebView->setFixedWidth(m_tablet_width);
}

void MainWindow::on_push_phone_clicked()
{
	ui.push_computer->setStyleSheet("");
	ui.push_laptop->setStyleSheet("");
	ui.push_tablet->setStyleSheet("");
	ui.push_phone->setStyleSheet("background-color: #3d8ec9;");
	m_WebView->setFixedWidth(m_phone_width);
}

bool MainWindow::isWidgetFocused(QString strWidgetName)
{
	if (QApplication::focusWidget()->objectName().compare(                    strWidgetName, Qt::CaseInsensitive) == 0 ||
		QApplication::focusWidget()->parent()->objectName().compare(          strWidgetName, Qt::CaseInsensitive) == 0 ||
		QApplication::focusWidget()->parent()->parent()->objectName().compare(strWidgetName, Qt::CaseInsensitive) == 0)
	{
		return true;
	}
	return false;
}

void MainWindow::on_ReceivedAutoGenCpp(QByteArray config)
{
	
	if (m_strProjRootPath.isEmpty()) 
	{ 
		qDebug() << "[ERROR] Cannot save ui_ file if project path is not defined.";
	}
	// save ui_webpage header file
	if (m_boolTemporaryCpp)
	{
		m_boolTemporaryCpp = false;
		//
		QTemporaryFile file;
		file.setAutoRemove(false);
		file.setFileTemplate(g_strLocalTempPath + "wtdesigner_html_preview_XXXXXX.txt");
		//
		if (!file.open())
		{
			qDebug() << "[ERROR] Could not open CPP preview in MainWindow::on_ReceivedAutoGenCpp";
			return;
		}
		file.write(config);
		file.flush();
		file.close();
		//
		QDesktopServices::openUrl(QUrl("file:///" + file.fileName(), QUrl::TolerantMode));
	}
	else
	{
		QString strAutoGenCppFile;
		strAutoGenCppFile = m_strProjRootPath + "/ui_" + m_strProjName.toLower() + ".h";
		QFile file(strAutoGenCppFile);
		//
		if (file.open(QIODevice::ReadWrite | QFile::Truncate))
		{
			//// generate configuration xml text from object tree and store in file
			//QTextStream stream(&file);
			//// get config from tree model
			//stream << config;
			// [FIX] Half the problem was solved with this.
			//       Here last 3 come with "?"
			file.write(config);
		}
		else
		{
			QMessageBox msgBox;
			msgBox.setIcon(QMessageBox::Critical);
			msgBox.setText(tr("Could not create file ") + strAutoGenCppFile);
			msgBox.exec();
		}
		file.close(); 
		// increase progress in dialog and kill
		mp_ProcessDialog->setValue(3);
		if (mp_ProcessDialog) { mp_ProcessDialog->deleteLater(); }
	}

}

void MainWindow::on_ReceivedAllProperties(QByteArray allprops)
{
	QDomDocument doc;
	doc.setContent(allprops);
	for (int i = 0; i < doc.documentElement().childNodes().count(); i++)
	{
		QDomElement propElem = doc.documentElement().childNodes().at(i).toElement();
		QString strId        = propElem.attribute("id");
		QString strName      = propElem.attribute("name");
		QString strValue     = propElem.attribute("value");
		
		WDomElem *domElem = m_treemodel.getElemByName(strId);
		if (!domElem)
		{
			qDebug() << "[ERROR] Could not find element id = " << strId << " in MainWindow::on_ReceivedAllProperties.";
			continue;
		}

		QDomElement widgetElem = domElem->getElem();
		// clean for repeated styles
		if (strName.compare("Wt_styleClass") == 0)
		{
			QStringList strStyleList = strValue.split(" ");
			QSet<QString> strSet = strStyleList.toSet();
			strStyleList = QStringList::fromSet(strSet);
			strValue = strStyleList.join(" ");
		}
		widgetElem.setAttribute(strName, strValue);
	}
}

void MainWindow::on_actionPreferences_triggered()
{
	m_dialogConfig.setupDialogWidgets();
	m_dialogConfig.show();
}

void MainWindow::on_ReloadWtServer()
{
	StopWtServer(); 
	StartWtServer();
#ifdef Q_OS_WIN
	// because this is called when config is changed (i.e. the lib path might have changed)
	m_listLibFileNames.clear();
#endif
}

void MainWindow::on_receivedDragOnWebview(QByteArray baconfigChunk, QString strParentElemId)
{
	// validate config
	QString errorStr;
	int     errorLine;
	int     errorColumn;
	QDomDocument doc;
	if (!doc.setContent(baconfigChunk, &errorStr, &errorLine, &errorColumn))
	{
		QMessageBox::critical(0, QObject::tr("Clipboard Paste Error"), QObject::tr("The clipboard does not contain valid configuration.\nParse error at line %1, " "column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr));
		return;
	}
	// put in parent in last row
	if (!strParentElemId.isEmpty())
	{
		WDomElem *wdomelem = m_treemodel.getElemByName(strParentElemId);
		if (!wdomelem)
		{
			qDebug() << "[ERROR] Selected parent " << strParentElemId << " does not exists in configuration (MainWindow::on_receivedDragOnWebview)";
			return;
		}
		int intRow = wdomelem->countChild();
		MyUndoAppendElem *appCommand = new MyUndoAppendElem(&m_treemodel, baconfigChunk, strParentElemId, intRow, this);
		m_commandHistory.push(appCommand);
	}
}

void MainWindow::on_NewClientConneted()
{
	// TODO : increase number of connected clients counter and display it somewhere, also implement disconnection counter
	Q_EMIT UpdateAllProperties();
}

void MainWindow::on_treeviewWtTree_contextMenu(const QPoint &point)
{
	QMenu       rightClickMenu;
	QModelIndex index = ui.treeviewWtTree->indexAt(point);

	// for icons
    QMap<QString, QIcon>  mapIcons = m_pwidgetmodel->getMapIconsByClassName();

	if (!index.isValid())
	{
		qDebug() << "[ERROR] Right click on invalid element in MainWindow::on_treeviewWtTree_contextMenu";
		return;
	}

	WDomElem *welem = m_treemodel.getElemByIndex(index);

	if (welem == NULL)
	{
		qDebug() << "[ERROR] Could not get valid wdomelem in MainWindow::on_treeviewWtTree_contextMenu";
		return;
	}

	QString strClassName = welem->getElem().attribute(g_strClassAttr);

	QAction  * act1, * act2;
	QAction  * res;
	QString    strParentElemId = welem->getElem().attribute(g_strIdAttr);
	QByteArray baconfigChunk   = QByteArray();
	if (strClassName.compare("WTabWidget") == 0)
	{
		act1 = rightClickMenu.addAction(tr("Add ") + "WTabItem child");
        QIcon iconTemp = mapIcons.value("WTabItem");
        act1->setIcon(iconTemp);
		res  = rightClickMenu.exec(ui.treeviewWtTree->mapToGlobal(point));
		if (res == act1)
		{
			baconfigChunk = m_pwidgetmodel->getWidgetConfigByName("WTabItem");
		}
	}
	else if (strClassName.compare("WMenu") == 0)
	{
		act1 = rightClickMenu.addAction(tr("Add ") + "WMenuItem child");
        QIcon iconTemp = mapIcons.value("WMenuItem");
        act1->setIcon(iconTemp);
		res  = rightClickMenu.exec(ui.treeviewWtTree->mapToGlobal(point));
		if (res == act1)
		{
			baconfigChunk = m_pwidgetmodel->getWidgetConfigByName("WMenuItem");
		}
	}
	else if (strClassName.compare("WPopupMenu") == 0)
	{
		act1 = rightClickMenu.addAction(tr("Add ") + "WPopupItem child");
		act1->setIcon(mapIcons.value("WPopupItem"));
		rightClickMenu.addSeparator();
		act2 = rightClickMenu.addAction(tr("Add ") + "WPopupMenu child");
		act2->setIcon(mapIcons.value("WPopupMenu"));
		res  = rightClickMenu.exec(ui.treeviewWtTree->mapToGlobal(point));
		if (res == act1)
		{
			baconfigChunk = m_pwidgetmodel->getWidgetConfigByName("WPopupItem");
		}
		else if (res == act2)
		{
			baconfigChunk = m_pwidgetmodel->getWidgetConfigByName("WPopupMenu");
		}
	}

	if (!baconfigChunk.isEmpty())
	{
		on_receivedDragOnWebview(baconfigChunk, strParentElemId);
	}

}

void MainWindow::on_treeviewWtWidgets_doubleClicked(const QModelIndex &index)
{
	WWidgetNode *item = static_cast<WWidgetNode*>(index.internalPointer());
	if (item)
	{
		QString strCloserContainer = m_treemodel.findCloserContainer(m_strCurrentSelectedElemId);
		if (!strCloserContainer.isEmpty())
		{
			on_receivedDragOnWebview(item->getConfig(), strCloserContainer);
		}
	}
}

void MainWindow::ShowNewFileSystemMenu(QPoint pos, bool isRightClick /*= false*/)
{
	QModelIndex index;
	QFileInfo   info;
	QString     strTargetPath;

	QMenu       rightClickMenu;

	QAction  *addCss, *addJs, *remCss, *remJs, *newFile, *newFolder, *existFile, *existFolder, *remov;
	QAction  * res;

	index = ui.treeviewWtResources->currentIndex();
	if (isRightClick && index.isValid())
	{
		info = m_resourcesmodel.fileInfo(index);
		if (info.suffix().contains("css", Qt::CaseInsensitive))
		{
			if (m_treemodel.isTrackedCssFile(info))
			{
				remCss = rightClickMenu.addAction(tr("Remove CSS from Wt App"));
			}
			else
			{
				addCss = rightClickMenu.addAction(tr("Append CSS to Wt App"));
			}
		}
		else if (info.suffix().contains("js", Qt::CaseInsensitive))
		{
			if (m_treemodel.isTrackedJsFile(info))
			{
				remJs = rightClickMenu.addAction(tr("Remove JS from Wt App"));
			}
			else
			{
				addJs = rightClickMenu.addAction(tr("Append JS to Wt App"));
			}
		}
	}

	QMenu    * newMenu      = rightClickMenu.addMenu("Add New...");
	QMenu    * existingMenu = rightClickMenu.addMenu("Import Existing...");

	if (isRightClick)
	{
		pos   = ui.treeviewWtResources->mapToGlobal(pos);
		remov = rightClickMenu.addAction(tr("Delete"));
	}

	if (index.isValid())
	{
		info = m_resourcesmodel.fileInfo(index);
		strTargetPath = info.absolutePath();
	}
	else
	{
		strTargetPath = QDir(g_strLocalResourcesPath).absolutePath();
	}
	
	qDebug() << strTargetPath;

	newFile       = newMenu->addAction(tr("File..."));
	newFolder     = newMenu->addAction(tr("Folder..."));
	existFile     = existingMenu->addAction(tr("File..."));
	existFolder   = existingMenu->addAction(tr("Folder..."));

	res = rightClickMenu.exec(pos);

	if (res == newFile)
	{
		bool ok;
		QString newFileName = QInputDialog::getText(this, tr("Create New File"), tr("File name : "), QLineEdit::Normal, "newstylesheet.css", &ok);
		if (ok && !newFileName.isEmpty())
		{
			QFile file(strTargetPath + "/" + newFileName);
			if (!file.exists())
			{
				file.open(QIODevice::WriteOnly);
				file.write("");
				file.close();
				// check if CSS file specifically 
				CheckIfStylesheet("./" + QDir(".").relativeFilePath(strTargetPath + "/" + newFileName), true);
				// check if JS file specifically 
				CheckIfJavascript("./" + QDir(".").relativeFilePath(strTargetPath + "/" + newFileName), true);
			}
			else
			{
				QMessageBox msgBox;
				msgBox.setText(tr("Could not create file. A file with name ") + newFileName + tr(" already exists in resources."));
				msgBox.setIcon(QMessageBox::Critical);
				msgBox.exec();
			}
		}
	}
	else if (res == newFolder)
	{
		bool ok;
		QString newFolderName = QInputDialog::getText(this, tr("Create New Folder"), tr("File name : "), QLineEdit::Normal, "My Folder", &ok);
		if (ok && !newFolderName.isEmpty())
		{
			QDir dir(strTargetPath + "/" + newFolderName);
			if (!dir.exists())
			{
				QDir(strTargetPath).mkdir(newFolderName);
			}
			else
			{
				QMessageBox msgBox;
				msgBox.setText(tr("Could not create folder. A folder with name ") + newFolderName + tr(" already exists in resources."));
				msgBox.setIcon(QMessageBox::Critical);
				msgBox.exec();
			}
		}
	}
	else if (res == existFile)
	{
		QString fileNameImport = QFileDialog::getOpenFileName(this, tr("Select File"), QDir::homePath(), tr("Any file type (*.*)"));
		if (!fileNameImport.isEmpty())
		{
			QString strNewLocalFileName = g_strLocalResourcesPath + QFileInfo(fileNameImport).fileName();
			if (!QFile(strNewLocalFileName).exists())
			{
				QFile::copy(fileNameImport, strNewLocalFileName);
				// check if CSS file specifically
				CheckIfStylesheet(strNewLocalFileName, true);
				// check if JS file specifically
				CheckIfJavascript(strNewLocalFileName, true);
			}
			else
			{
				QMessageBox msgBox;
				msgBox.setText(tr("Could not import file. A file with name ") + strNewLocalFileName + tr(" already exists in resources."));
				msgBox.setIcon(QMessageBox::Critical);
				msgBox.exec();
			}
		}
	}
	else if (res == existFolder)
	{
		QString dirPathImport = QFileDialog::getExistingDirectory(this, tr("Select Folder"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
		if (!dirPathImport.isEmpty())
		{
			QStringList strDirsList = dirPathImport.split("/");
			QString     strLocalNewFolderName = g_strLocalResourcesPath + strDirsList.last() + "/";
			if (!QDir().exists(strLocalNewFolderName))
			{
				CopyAllFilesInPath(dirPathImport, strLocalNewFolderName, QStringList() << "*.*");
				// check if CSS file recursivelly in new local folder
				CheckIfStylesheetPath(strLocalNewFolderName, true);
				// check if JS file recursivelly in new local folder
				CheckIfJavascriptPath(strLocalNewFolderName, true);
			}
			else
			{
				QMessageBox msgBox;
				msgBox.setText(tr("Could not import folder. A folder with name ") + strLocalNewFolderName + tr(" already exists in resources."));
				msgBox.setIcon(QMessageBox::Critical);
				msgBox.exec();
			}
		}
	}
	else if (res == remov && isRightClick)
	{
		on_push_resource_remove_clicked();
	}
	else if (res == remCss)
	{
		info = m_resourcesmodel.fileInfo(index);
		m_treemodel.removeTrackedCssFile(info);
		on_ReloadWtServer(); // necessary to append to list on server-side otherwise autogencpp fails
	}
	else if (res == addCss)
	{
		info = m_resourcesmodel.fileInfo(index);
		m_treemodel.appendTrackedCssFile(info);
		on_ReloadWtServer(); // necessary to append to list on server-side otherwise autogencpp fails
	}
	else if (res == remJs)
	{
		info = m_resourcesmodel.fileInfo(index);
		m_treemodel.removeTrackedJsFile(info);
		on_ReloadWtServer(); // necessary to append to list on server-side otherwise autogencpp fails
	}
	else if (res == addJs)
	{
		info = m_resourcesmodel.fileInfo(index);
		m_treemodel.appendTrackedJsFile(info);
		on_ReloadWtServer(); // necessary to append to list on server-side otherwise autogencpp fails
	}
}

void MainWindow::on_treeviewWtResources_contextMenu(const QPoint &point)
{
	ShowNewFileSystemMenu(point, true);
}

void MainWindow::CheckIfStylesheet(QString strFileName, bool isAdd)
{
	// this function appends or removes stylesheet depending on bool isAdd
	if (!QFileInfo(strFileName).suffix().contains("css", Qt::CaseInsensitive))
	{
		return;
	}

	if (isAdd)
	{
		// append to xml config
		QMessageBox msgBox;
		msgBox.setText("Detected Stylesheet File.");
		msgBox.setInformativeText("Would you like to append '" + strFileName + "' to the Wt application stylesheets?");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setIcon(QMessageBox::Question);
		msgBox.setDefaultButton(QMessageBox::Yes);
		QMessageBox::StandardButton ret = (QMessageBox::StandardButton)msgBox.exec();
		if (ret == QMessageBox::No) { return; }
		if (m_treemodel.appendTrackedCssFile(QFileInfo(strFileName)))
		{
			// Reload for all clients including local webview
			on_ReloadWtServer();
		}
	}
	else
	{
		if (m_treemodel.removeTrackedCssFile(QFileInfo(strFileName), true))
		{
			// Reload for all clients including local webview
			on_ReloadWtServer();
		}
	}
}

void MainWindow::CheckIfStylesheetPath(QString strPath, bool isAdd)
{
	// same as CheckIfStylesheet but for all files in a path
	QDir m_Path(strPath);
	QDirIterator it(m_Path.absolutePath().toUtf8(), QStringList() << "*.css", QDir::Files, QDirIterator::Subdirectories);
	// iterate css files
	while (it.hasNext())
	{
		// get next file
		it.next();
		// check file
		CheckIfStylesheet("./" + QDir(".").relativeFilePath(it.filePath()), isAdd);		
	}
}

void MainWindow::CheckIfJavascript(QString strFileName, bool isAdd)
{
	// this function appends or removes javascript depending on bool isAdd
	if (!QFileInfo(strFileName).suffix().contains("js", Qt::CaseInsensitive))
	{
		return;
	}

	if (isAdd)
	{
		// append to xml config
		QMessageBox msgBox;
		msgBox.setText("Detected Javascript File.");
		msgBox.setInformativeText("Would you like to append '" + strFileName + "' to the Wt application javascripts?");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setIcon(QMessageBox::Question);
		msgBox.setDefaultButton(QMessageBox::Yes);
		QMessageBox::StandardButton ret = (QMessageBox::StandardButton)msgBox.exec();
		if (ret == QMessageBox::No) { return; }
		if (m_treemodel.appendTrackedJsFile(QFileInfo(strFileName)))
		{
			on_ReloadWtServer(); // necessary to append to list on server-side otherwise autogencpp fails
		}
	}
	else
	{
		if (m_treemodel.removeTrackedJsFile(QFileInfo(strFileName), true))
		{
			on_ReloadWtServer(); // necessary to append to list on server-side otherwise autogencpp fails
		}
	}
}

void MainWindow::CheckIfJavascriptPath(QString strPath, bool isAdd)
{
	// same as CheckIfJavascript but for all files in a path
	QDir m_Path(strPath);
	QDirIterator it(m_Path.absolutePath().toUtf8(), QStringList() << "*.js", QDir::Files, QDirIterator::Subdirectories);
	// iterate css files
	while (it.hasNext())
	{
		// get next file
		it.next();
		// check file
		CheckIfJavascript("./" + QDir(".").relativeFilePath(it.filePath()), isAdd);
	}
}

void MainWindow::on_saveNewProject(QString path, QString projname)
{
	// create new path path/projname if it does not exists
	QDir dirPath = (path);
	if (!QDir(path+projname).exists())
	{
		if(!dirPath.mkdir(projname))
		{
			QMessageBox msgBox;
			msgBox.setText(tr("Could not create and save project in selected path ") + path + tr("."));
			msgBox.setIcon(QMessageBox::Critical);
			msgBox.exec();
			return;
		}
	}
	// set path as user defined project path form now on (to load from and save to)
	m_strProjRootPath = path+projname;
	m_strProjName     = projname;
	// now that user defined proj path is defined then we can copy stuff
	on_actionSave_triggered();
}

void MainWindow::on_cleanChanged(bool clean)
{
	if (clean)
	{
		// set window title
		setWindowTitle("WtDesigner - " + m_strProjName);
	} 
	else
	{
		if (!windowTitle().contains(" *"))
		{
			// set window title
			setWindowTitle("WtDesigner - " + m_strProjName + " *");
		}
	}
}

void MainWindow::on_actionClose_triggered()
{
	// TODO : does it make sense?
}

void MainWindow::on_actionQuit_triggered()
{
	close();
}


void MainWindow::closeEvent(QCloseEvent * event)
{
	// check on changes and ask if save
	if (!m_commandHistory.isClean())
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle("You have unsaved changes.");
		msgBox.setText("Would you like to save your current project changes before creating a new project?");
		msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Cancel);
		int ret = msgBox.exec();
		if (ret == QMessageBox::Save)
		{
			on_actionSave_triggered();
			event->ignore();
			return;
		}
		else if (ret == QMessageBox::Cancel)
		{
			event->ignore();
			return;
		}
	}
	QMainWindow::closeEvent(event);
}

void MainWindow::on_push_refresh_clicked()
{
	// Relead for all clients including local webview
	on_ReloadWtServer();
}

void MainWindow::on_actionExport_to_CMake_triggered()
{
#ifdef Q_OS_WIN
	// Try to find Wt installation again
	if (m_strIncludeDir.isEmpty())
	{
		m_strIncludeDir = FindWtIncludeDir();
		if (m_strIncludeDir.isEmpty())
		{
			m_strIncludeDir = "Could not be found";
			QMessageBox::critical(0, QObject::tr("Invalid Wt Directory"), QObject::tr("Could not find Wt Include path.\nSelect a valid path in Settings."));
			return; 
		}
		m_settings.setValue("m_strincludedir", m_strIncludeDir);
	}
	if (m_strLibraryDir.isEmpty())
	{
		m_strLibraryDir = FindWtLibraryDir();
		if (m_strLibraryDir.isEmpty())
		{
			m_strLibraryDir = "Could not be found";
			QMessageBox::critical(0, QObject::tr("Invalid Wt Directory"), QObject::tr("Could not find Wt Library path.\nSelect a valid path in Settings."));
			return; 
		}
		m_settings.setValue("m_strlibrarydir", m_strLibraryDir);
	}
	if (m_strBinaryDir.isEmpty())
	{
		m_strBinaryDir = FindWtBinaryDir();
		if (m_strBinaryDir.isEmpty())
		{
			m_strBinaryDir = "Could not be found";
			QMessageBox::critical(0, QObject::tr("Invalid Wt Directory"), QObject::tr("Could not find Wt Binary path.\nSelect a valid path in Settings."));
			return; 
		}
		m_settings.setValue("m_strbinarydir", m_strBinaryDir);
	}
	// Try once again to find boost lib files inside Wt installation
	if (m_listLibFileNames.isEmpty())
	{
		FindLibFileNames();
	}
#endif
	// create replace tokens list
    QList<QPair<QString, QString> > listReplaceTokens;
	listReplaceTokens.append(QPair<QString, QString>("@PROJNAME@"       , m_strProjName          ));
	listReplaceTokens.append(QPair<QString, QString>("@PROJNAMELOWER@"  , m_strProjName.toLower()));
	listReplaceTokens.append(QPair<QString, QString>("@PROJNAMEUPPER@"  , m_strProjName.toUpper()));
#ifdef Q_OS_WIN
	listReplaceTokens.append(QPair<QString, QString>("@WT_INCLUDE_PATH@", m_strIncludeDir        ));
	listReplaceTokens.append(QPair<QString, QString>("@WT_LIB_PATH@"    , m_strLibraryDir        ));
	listReplaceTokens.append(QPair<QString, QString>("@WT_BIN_PATH@"    , m_strBinaryDir         )); // NOTE : does not seem necessary to replace "/" for "\\" 
#endif
	// join params separated be space
	QString strJointParams;
	for (int i = WT_PARDOCROOT; i <= WT_VALHTTPPORT; i++)
	{
		if (i != WT_VALHTTPPORT)
		{
			strJointParams += m_vectWtParams.at(i);
			strJointParams += " ";
		}
		else
		{
			strJointParams += m_strServerPort;
		}
	}
	listReplaceTokens.append(QPair<QString, QString>("@WT_PARAMS@"      , strJointParams         ));
	// if lib files are defined
	QString strCorrectCMakeFile; 
#ifdef Q_OS_WIN
	if (!m_listLibFileNames.isEmpty())
	{
		// complete replace tokens list with lib file names
		listReplaceTokens.append( QPair<QString, QString>("@WT_DEBUG@"                     ,m_listLibFileNames.filter(QRegExp("(\w)*wtd.(lib|a)"                    )).first()) );
		listReplaceTokens.append( QPair<QString, QString>("@WT_RELEASE@"                   ,m_listLibFileNames.filter(QRegExp("(\w)*wt.(lib|a)"                     )).first()) );
		listReplaceTokens.append( QPair<QString, QString>("@WTHTTP_DEBUG@"                 ,m_listLibFileNames.filter(QRegExp("(\w)*wthttpd.(lib|a)"                )).first()) );
		listReplaceTokens.append( QPair<QString, QString>("@WTHTTP_RELEASE@"               ,m_listLibFileNames.filter(QRegExp("(\w)*wthttp.(lib|a)"                 )).first()) );
		listReplaceTokens.append( QPair<QString, QString>("@BOOST_DATE_TIME_DEBUG@"        ,m_listLibFileNames.filter(QRegExp("(\w)*date_time(.)*gd(.)*.(lib|a)"    )).first()) );
		listReplaceTokens.append( QPair<QString, QString>("@BOOST_DATE_TIME_RELEASE@"      ,m_listLibFileNames.filter(QRegExp("(\w)*date_time[^gd]+.(lib|a)"        )).first()) );
		listReplaceTokens.append( QPair<QString, QString>("@BOOST_FILESYSTEM_DEBUG@"       ,m_listLibFileNames.filter(QRegExp("(\w)*filesystem(.)*gd(.)*.(lib|a)"   )).first()) );
		listReplaceTokens.append( QPair<QString, QString>("@BOOST_FILESYSTEM_RELEASE@"     ,m_listLibFileNames.filter(QRegExp("(\w)*filesystem[^gd]+.(lib|a)"       )).first()) );
		listReplaceTokens.append( QPair<QString, QString>("@BOOST_PROGRAM_OPTIONS_DEBUG@"  ,m_listLibFileNames.filter(QRegExp("(\w)*program(.)*gd(.)*.(lib|a)"      )).first()) );
		listReplaceTokens.append( QPair<QString, QString>("@BOOST_PROGRAM_OPTIONS_RELEASE@",m_listLibFileNames.filter(QRegExp("(\w)*program[^gd]+.(lib|a)"          )).first()) );
		listReplaceTokens.append( QPair<QString, QString>("@BOOST_REGEX_DEBUG@"            ,m_listLibFileNames.filter(QRegExp("(\w)*regex(.)*gd(.)*.(lib|a)"        )).first()) );
		listReplaceTokens.append( QPair<QString, QString>("@BOOST_REGEX_RELEASE@"          ,m_listLibFileNames.filter(QRegExp("(\w)*regex[^gd]+.(lib|a)"            )).first()) );
		listReplaceTokens.append( QPair<QString, QString>("@BOOST_SIGNALS_DEBUG@"          ,m_listLibFileNames.filter(QRegExp("(\w)*signals(.)*gd(.)*.(lib|a)"      )).first()) );
		listReplaceTokens.append( QPair<QString, QString>("@BOOST_SIGNALS_RELEASE@"        ,m_listLibFileNames.filter(QRegExp("(\w)*signals[^gd]+.(lib|a)"          )).first()) );
		listReplaceTokens.append( QPair<QString, QString>("@BOOST_SYSTEM_DEBUG@"           ,m_listLibFileNames.filter(QRegExp("(\w)*[^file]system(.)*gd(.)*.(lib|a)")).first()) );
		listReplaceTokens.append( QPair<QString, QString>("@BOOST_SYSTEM_RELEASE@"         ,m_listLibFileNames.filter(QRegExp("(\w)*[^file]system[^gd]+.(lib|a)"    )).first()) );
		listReplaceTokens.append( QPair<QString, QString>("@BOOST_THREAD_DEBUG@"           ,m_listLibFileNames.filter(QRegExp("(\w)*thread(.)*gd(.)*.(lib|a)"       )).first()) );
		listReplaceTokens.append( QPair<QString, QString>("@BOOST_THREAD_RELEASE@"         ,m_listLibFileNames.filter(QRegExp("(\w)*thread[^gd]+.(lib|a)"           )).first()) );
		listReplaceTokens.append( QPair<QString, QString>("@BOOST_RANDOM_DEBUG@"           ,m_listLibFileNames.filter(QRegExp("(\w)*random(.)*gd(.)*.(lib|a)"       )).first()) );
		listReplaceTokens.append( QPair<QString, QString>("@BOOST_RANDOM_RELEASE@"         ,m_listLibFileNames.filter(QRegExp("(\w)*random[^gd]+.(lib|a)"           )).first()) );
		// the one without generator
		strCorrectCMakeFile = "./cmake/template_CMakeLists.txt";
	}
	else
	{
		// TODO : make template with boost finder
		//strCorrectCMakeFile = "./cmake/template_CMakeListsFinder.txt";
		QMessageBox::critical(0, QObject::tr("Invalid Wt Library Files"), QObject::tr("Could not find Wt Library Files.\nSelect a valid library path in Settings."));
		return;
	}
#endif
#ifndef Q_OS_WIN
	strCorrectCMakeFile = "./cmake/template_nix_CMakeLists.txt";
#endif
	QString strNewFileName;
	QMessageBox msgBox;
	QMessageBox::StandardButton respose;
	// process "./cmake/template_CMakeLists.txt"
	respose = QMessageBox::Ok;
	strNewFileName = m_strProjRootPath + "/CMakeLists.txt";
	if (QFile(strNewFileName).exists())
	{
		msgBox.setText(tr("File ") + strNewFileName + tr(" already exists.\nWould you like to overwrite it?"));
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
		respose = (QMessageBox::StandardButton)msgBox.exec();
	}
	if (respose == QMessageBox::Ok)
	{
		ProcessCMakeTemplate(strCorrectCMakeFile, strNewFileName, listReplaceTokens);
	}
#ifdef Q_OS_WIN
	// process "./cmake/template_vs.xml"
	respose = QMessageBox::Ok;
	strNewFileName = m_strProjRootPath + "/" + m_strProjName.toLower() + ".vcxproj.user";
	if (QFile(strNewFileName).exists())
	{
		msgBox.setText(tr("File ") + strNewFileName + tr(" already exists.\nWould you like to overwrite it?"));
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
		respose = (QMessageBox::StandardButton)msgBox.exec();
	}
	if (respose == QMessageBox::Ok)
	{
		ProcessCMakeTemplate("./cmake/template_vs.xml", strNewFileName, listReplaceTokens);
	}
	// process "./cmake/template_eclipse_release.xml"
	respose = QMessageBox::Ok;
	strNewFileName = m_strProjRootPath + "/" + m_strProjName.toLower() + "-release.exe.launch";
	if (QFile(strNewFileName).exists())
	{
		msgBox.setText(tr("File ") + strNewFileName + tr(" already exists.\nWould you like to overwrite it?"));
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
		respose = (QMessageBox::StandardButton)msgBox.exec();
	}
	if (respose == QMessageBox::Ok)
	{
		ProcessCMakeTemplate("./cmake/template_eclipse_release.xml", strNewFileName, listReplaceTokens);
	}
	// process "./cmake/template_eclipse_debug.xml"
	respose = QMessageBox::Ok;
	strNewFileName = m_strProjRootPath + "/" + m_strProjName.toLower() + "-debug.exe.launch";
	if (QFile(strNewFileName).exists())
	{
		msgBox.setText(tr("File ") + strNewFileName + tr(" already exists.\nWould you like to overwrite it?"));
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
		respose = (QMessageBox::StandardButton)msgBox.exec();
	}
	if (respose == QMessageBox::Ok)
	{
		ProcessCMakeTemplate("./cmake/template_eclipse_debug.xml", strNewFileName, listReplaceTokens);
	}
#endif
	// process "./cmake/template_project.h"
	respose = QMessageBox::Ok;
	strNewFileName = m_strProjRootPath + "/" + m_strProjName.toLower() + ".h";
	if (QFile(strNewFileName).exists())
	{
		msgBox.setText(tr("File ") + strNewFileName + tr(" already exists.\nWould you like to overwrite it?"));
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
		respose = (QMessageBox::StandardButton)msgBox.exec();
	}
	if (respose == QMessageBox::Ok)
	{
		ProcessCMakeTemplate("./cmake/template_project.h", strNewFileName, listReplaceTokens);
	}
	// process "./cmake/template_project.cpp"
	respose = QMessageBox::Ok;
	strNewFileName = m_strProjRootPath + "/" + m_strProjName.toLower() + ".cpp";
	if (QFile(strNewFileName).exists())
	{
		msgBox.setText(tr("File ") + strNewFileName + tr(" already exists.\nWould you like to overwrite it?"));
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
		respose = (QMessageBox::StandardButton)msgBox.exec();
	}
	if (respose == QMessageBox::Ok)
	{
		ProcessCMakeTemplate("./cmake/template_project.cpp", strNewFileName, listReplaceTokens);
	}
	// process "./cmake/template_main.cpp"
	respose = QMessageBox::Ok;
	strNewFileName = m_strProjRootPath + "/main.cpp";
	if (QFile(strNewFileName).exists())
	{
		msgBox.setText(tr("File ") + strNewFileName + tr(" already exists.\nWould you like to overwrite it?"));
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
		respose = (QMessageBox::StandardButton)msgBox.exec();
	}
	if (respose == QMessageBox::Ok)
	{
		ProcessCMakeTemplate("./cmake/template_main.cpp", strNewFileName, listReplaceTokens);
	}
#ifndef Q_OS_WIN
	// process "./cmake/template_script.sh"
	respose = QMessageBox::Ok;
	strNewFileName = m_strProjRootPath + "/" + m_strProjName+ ".sh";
	if (QFile(strNewFileName).exists())
	{
		msgBox.setText(tr("File ") + strNewFileName + tr(" already exists.\nWould you like to overwrite it?"));
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
		respose = (QMessageBox::StandardButton)msgBox.exec();
	}
	if (respose == QMessageBox::Ok)
	{
		ProcessCMakeTemplate("./cmake/template_script.sh", strNewFileName, listReplaceTokens);
	}
#endif
	// Aknowledge finished
	QMessageBox::information(0, QObject::tr("Finished Task"), QObject::tr("Finished exporting CMake project to %1.").arg(m_strProjRootPath));
}

void MainWindow::on_actionTutorials_triggered()
{
	QDesktopServices::openUrl ( QUrl("https://www.youtube.com/playlist?list=PLy_hF6QxSYC0nQtPUdxEeLUsM3PNnqApI") );
}

void MainWindow::on_actionC_Code_triggered()
{
	// Autogen Cpp and show in temp file (done in MainWindow::on_ReceivedAutoGenCpp given m_boolTemporaryCpp==true)
	m_boolTemporaryCpp = true;
	Q_EMIT RequestAutoGenCpp(m_strProjName);
}

void MainWindow::on_actionHtml_Code_triggered()
{
	// Autogen Html and show in temp file
	QTemporaryFile tmpFile;
	tmpFile.setAutoRemove(false);
	tmpFile.setFileTemplate(g_strLocalTempPath + "wtdesigner_html_preview_XXXXXX.txt"); // [NOTE] : .txt so does not open in browser
	if (!tmpFile.open())
	{
		qDebug() << "[ERROR] Could not open HTML preview in MainWindow::on_actionC_Code_triggered";
		return;
	}
	QString strHtml = m_WebView->page()->mainFrame()->toHtml();
	tmpFile.write(strHtml.toUtf8());
	tmpFile.flush();
	tmpFile.close();
	QDesktopServices::openUrl(QUrl("file:///" + tmpFile.fileName(), QUrl::TolerantMode));
}

void MainWindow::on_actionAbout_triggered()
{
	m_dialogAbout.show(); 
}

void MainWindow::on_trackedFileWasMoved(QString strRelSourceFilePath, QString strRelDestinationFilePath)
{
	if (m_treemodel.isTrackedCssFile(QFileInfo(strRelSourceFilePath)))
	{
		// remove old reference
		m_treemodel.removeTrackedCssFile(QFileInfo(strRelSourceFilePath));
		// add new reference
		m_treemodel.appendTrackedCssFile(QFileInfo(strRelDestinationFilePath));
		// Reload for all clients including local webview
		on_ReloadWtServer();
	}

	if (m_treemodel.isTrackedJsFile(QFileInfo(strRelSourceFilePath)))
	{
		// remove old reference
		m_treemodel.removeTrackedJsFile(QFileInfo(strRelSourceFilePath));
		// add new reference
		m_treemodel.appendTrackedJsFile(QFileInfo(strRelDestinationFilePath));
		// Reload for all clients including local webview
		on_ReloadWtServer();
	}
}

void MainWindow::on_removedTrackedCssFile(QFileInfo fileRef)
{
    QListWidgetItem * item = ui.listWtStylesheets->findItems(fileRef.filePath(), Qt::MatchExactly).first();
	if (item)
	{
		int remRow = ui.listWtStylesheets->row(item);
		delete ui.listWtStylesheets->takeItem(remRow);
	}
}

void MainWindow::on_appendedTrackedCssFile(QFileInfo fileRef)
{
	QListWidgetItem *newItem = new QListWidgetItem;
	newItem->setText(fileRef.filePath());
	ui.listWtStylesheets->insertItem(ui.listWtStylesheets->count(), newItem);
}

void MainWindow::on_removedTrackedJsFile(QFileInfo fileRef)
{
    QListWidgetItem * item = ui.listWtJavascripts->findItems(fileRef.filePath(), Qt::MatchExactly).first();
	if (item)
	{
		int remRow = ui.listWtJavascripts->row(item);
		delete ui.listWtJavascripts->takeItem(remRow);
	}
}

void MainWindow::on_appendedTrackedJsFile(QFileInfo fileRef)
{
	QListWidgetItem *newItem = new QListWidgetItem;
	newItem->setText(fileRef.filePath());
	ui.listWtJavascripts->insertItem(ui.listWtJavascripts->count(), newItem);
}


void MainWindow::on_listWtStylesheets_itemDoubleClicked(QListWidgetItem *item)
{
	QFileInfo info(item->text());
	if (info.isFile())
	{
		QDesktopServices::openUrl(QUrl("file:///" + info.absoluteFilePath(), QUrl::TolerantMode));
	}
}

void MainWindow::on_listWtJavascripts_itemDoubleClicked(QListWidgetItem *item)
{
	QFileInfo info(item->text());
	if (info.isFile())
	{
		QDesktopServices::openUrl(QUrl("file:///" + info.absoluteFilePath(), QUrl::TolerantMode));
	}
}
