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

#include "dialogprojselect.h"
#include "ui_dialogprojselect.h"
#ifndef WTC_TESTING
#include "helperfunctions.h"
#endif

DialogProjSelect::DialogProjSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogProjSelect)
{
    ui->setupUi(this);
    // Set file system model
    m_modelCurrent.setRootPath(QUrl::fromLocalFile("").toString());
    m_modelCurrent.setNameFilters(QStringList() << "[^.]*"); // hide folders starting with dot (See QRegExp wildcard matching.)
    m_modelCurrent.setNameFilterDisables(false);
    // Set current tree
    ui->treeViewCurrent->setModel(&m_modelCurrent);
//    qDebug() << QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    ui->treeViewCurrent->setRootIndex(m_modelCurrent.index(QDir::homePath()));
        // Dirs only and first column only
    m_modelCurrent.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    for(int i = 0; i < m_modelCurrent.columnCount(); i++)
    {
        if(i > 0)
        {
            ui->treeViewCurrent->setColumnHidden(i, true);
        }
    }
    // Set drives list
    ui->listViewSystem->setModel(&m_modelCurrent);
    ui->listViewSystem->setRootIndex(m_modelCurrent.index(QUrl::fromLocalFile("").toString()));
    // Set combobox
    resetComboBox();
    // Disable Choose button until suitable selected
    ui->pushButtonChoose->setEnabled(false);
	// Disable back/forward buttons if necessary
	if (!m_stackdirchange.canUndo()) { ui->pushBack->setEnabled(false); }
	else { ui->pushBack->setEnabled(true); }
	if (!m_stackdirchange.canRedo()) { ui->pushForward->setEnabled(false); }
	else { ui->pushForward->setEnabled(true); }
	// Set window title
	this->setWindowTitle("Select Wt Creator Project Folder");
#ifndef WTC_TESTING         
    // Make icons white
	QIcon icon;
	SetIconColor(":/arrow-circle-left", icon, "white");
	ui->pushBack->setIcon(icon);
	SetIconColor(":/arrow-circle-right", icon, "white");
	ui->pushForward->setIcon(icon);
	SetIconColor(":/arrow-circle-up", icon, "white");
	ui->pushUp->setIcon(icon);
#endif


}

DialogProjSelect::~DialogProjSelect()
{
    delete ui;
}

void DialogProjSelect::on_pushButtonChoose_clicked()
{
    // Check if "./styles" exist
    bool boolResourcesDirExists = false;
    QString strSelDir  = ui->lineEditSelected->text();
    QFileInfoList list = QDir(strSelDir).entryInfoList();
    QString strRelDir;
    QFileInfo newinfo;
	for(int i = 0; i < list.count(); i++)
    {
		newinfo = list.at(i);
        // Get relative path of newinfo (file or dir) wrt to strSelDir (dir)
        if(newinfo.isDir())
        {
            strRelDir = QDir(strSelDir).relativeFilePath(newinfo.filePath());// Format file "filename.ext" Format folder "foldername"
            if(strRelDir.compare("resources", Qt::CaseInsensitive) == 0)
            {
                boolResourcesDirExists = true;
            }
        }
    }
    // Create warning message if any
    QString strErrorMsg = "";
    if(!boolResourcesDirExists)
    {
        strErrorMsg = tr("Could not find ./resources folder.");
    }
    // Show warning message if any
    if(!strErrorMsg.isEmpty())
    {

        int ret = QMessageBox::warning(this, tr("Wt Creator : Warning !"),
                                       strErrorMsg,
                                       QMessageBox::Ok | QMessageBox::Cancel,
                                       QMessageBox::Cancel);
        if(ret == QMessageBox::Cancel)
        {
            return;
        }
    }
    // Emit signal for loading project
    Q_EMIT openExistingProject(strSelDir + "/");
    accept();
}

void DialogProjSelect::on_pushCancel_clicked()
{
    reject();
}

void DialogProjSelect::on_listViewSystem_clicked(const QModelIndex &index)
{
    if(!index.isValid()) { return; }
    QFileInfo info = m_modelCurrent.fileInfo(index);
    if(!info.isDir())
    {
        ui->lineEditSelected->clear();
        ui->pushButtonChoose->setEnabled(false);
        return;
    }
    // Push to stack which automatically changes the dir
    MyUndoChangeDir *command = new MyUndoChangeDir(this, m_modelCurrent.fileInfo(ui->treeViewCurrent->rootIndex()).absoluteFilePath(), info.absoluteFilePath());
    m_stackdirchange.push(command);
	// Disable back/forward buttons if necessary
	if (!m_stackdirchange.canUndo()) { ui->pushBack->setEnabled(false); }
	else { ui->pushBack->setEnabled(true); }
	if (!m_stackdirchange.canRedo()) { ui->pushForward->setEnabled(false); }
	else { ui->pushForward->setEnabled(true); }
}

void DialogProjSelect::on_treeViewCurrent_clicked(const QModelIndex &index)
{
    if(!index.isValid()) { return; }
    QFileInfo info = m_modelCurrent.fileInfo(index);
    // Return if not Dir
    if(!info.isDir())
    {
        ui->lineEditSelected->clear();
        ui->pushButtonChoose->setEnabled(false);
        return;
    }
    QFileInfoList list = QDir(info.absoluteFilePath()).entryInfoList();
    //qDebug() << info.absoluteFilePath();
    QString strRelFileName;
	QFileInfo newinfo;
	for (int i = 0; i < list.count(); i++)
	{
		newinfo = list.at(i);
        // Get relative path of newinfo (file or dir) wrt to info (dir)
        strRelFileName = QDir(info.absoluteFilePath()).relativeFilePath(newinfo.filePath());// Format file "filename.ext" Format folder "foldername"
        // Check if selected Dir contains a project file
        if(strRelFileName.contains(".wui"))
        {
            ui->lineEditSelected->setText(info.absoluteFilePath());
            ui->pushButtonChoose->setEnabled(true);
            return;
        }
    }
    // If no Dir contained project file disbale choose button
    ui->lineEditSelected->clear();
    ui->pushButtonChoose->setEnabled(false);
}

void DialogProjSelect::on_treeViewCurrent_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid()) { return; }
    QFileInfo info = m_modelCurrent.fileInfo(index);
    if(!info.isDir())
    {
        ui->lineEditSelected->clear();
        ui->pushButtonChoose->setEnabled(false);
        return;
    }
    // Push to stack which automatically changes the dir
    MyUndoChangeDir *command = new MyUndoChangeDir(this, m_modelCurrent.fileInfo(ui->treeViewCurrent->rootIndex()).absoluteFilePath(),info.absoluteFilePath());
    m_stackdirchange.push(command);
	// Disable back/forward buttons if necessary
	if (!m_stackdirchange.canUndo()) { ui->pushBack->setEnabled(false); }
	else { ui->pushBack->setEnabled(true); }
	if (!m_stackdirchange.canRedo()) { ui->pushForward->setEnabled(false); }
	else { ui->pushForward->setEnabled(true); }
}

void DialogProjSelect::on_pushBack_clicked()
{
    m_stackdirchange.undo();
	// Disable back/forward buttons if necessary
	if (!m_stackdirchange.canUndo()) { ui->pushBack->setEnabled(false); }
	else { ui->pushBack->setEnabled(true); }
	if (!m_stackdirchange.canRedo()) { ui->pushForward->setEnabled(false); }
	else { ui->pushForward->setEnabled(true); }
}

void DialogProjSelect::on_pushForward_clicked()
{
    m_stackdirchange.redo();
	// Disable back/forward buttons if necessary
	if (!m_stackdirchange.canUndo()) { ui->pushBack->setEnabled(false); }
	else { ui->pushBack->setEnabled(true); }
	if (!m_stackdirchange.canRedo()) { ui->pushForward->setEnabled(false); }
	else { ui->pushForward->setEnabled(true); }
}

void DialogProjSelect::on_pushUp_clicked()
{
    QFileInfo info = m_modelCurrent.fileInfo(ui->treeViewCurrent->rootIndex());
    if(!info.isDir()){ return; }
    QDir dirUp = QDir(info.absoluteFilePath());
	if(!dirUp.cdUp()){ return; } // Move dir up
    // Push to stack which automatically changes the dir
    MyUndoChangeDir *command = new MyUndoChangeDir(this, m_modelCurrent.fileInfo(ui->treeViewCurrent->rootIndex()).absoluteFilePath(),dirUp.absolutePath());
    m_stackdirchange.push(command);
	// Disable back/forward buttons if necessary
	if (!m_stackdirchange.canUndo()) { ui->pushBack->setEnabled(false); }
	else { ui->pushBack->setEnabled(true); }
	if (!m_stackdirchange.canRedo()) { ui->pushForward->setEnabled(false); }
	else { ui->pushForward->setEnabled(true); }
}


MyUndoChangeDir::MyUndoChangeDir(DialogProjSelect *dialog, QString strOldDir, QString strNewDir, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_pdialog   = dialog   ;
    m_strOldDir = strOldDir;
    m_strNewDir = strNewDir;
}

void MyUndoChangeDir::undo()
{
    m_pdialog->ui->treeViewCurrent->setRootIndex(m_pdialog->m_modelCurrent.index(m_strOldDir));
}

void MyUndoChangeDir::redo()
{
    m_pdialog->ui->treeViewCurrent->setRootIndex(m_pdialog->m_modelCurrent.index(m_strNewDir));
}

void DialogProjSelect::resetComboBox()
{
    ui->comboBox->blockSignals(true);
    ui->comboBox->setModel(&m_modelCurrent);
    ui->comboBox->setRootModelIndex(m_modelCurrent.index(QDir::homePath()));
    ui->comboBox->blockSignals(false);
}

void DialogProjSelect::on_comboBox_activated(int index)
{
    QModelIndex indexParent = ui->comboBox->rootModelIndex();
    QFileInfo info = m_modelCurrent.fileInfo( indexParent.child(index,0) );

    if(!info.isDir())
    {
        ui->lineEditSelected->clear();
        ui->pushButtonChoose->setEnabled(false);
        return;
    }

    // Push to stack which automatically changes the dir
    MyUndoChangeDir *command = new MyUndoChangeDir(this, m_modelCurrent.fileInfo(ui->treeViewCurrent->rootIndex()).absoluteFilePath(), info.absoluteFilePath());
    m_stackdirchange.push(command);
    // Disable back/forward buttons if necessary
    if (!m_stackdirchange.canUndo()) { ui->pushBack->setEnabled(false); }
    else { ui->pushBack->setEnabled(true); }
    if (!m_stackdirchange.canRedo()) { ui->pushForward->setEnabled(false); }
    else { ui->pushForward->setEnabled(true); }
}
