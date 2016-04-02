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

#include "dialogprojsave.h"
#include "ui_dialogprojsave.h"
#ifndef WTC_TESTING
#include "helperfunctions.h"
#endif

#include <QRegExpValidator>


DialogProjSave::DialogProjSave(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogProjSave)
{
    ui->setupUi(this);
    // Set file system model
    m_modelCurrent.setRootPath(QUrl::fromLocalFile("").toString());
    m_modelCurrent.setNameFilters(QStringList() << "[^.]*"); // hide folders starting with dot (See QRegExp wildcard matching.)
    m_modelCurrent.setNameFilterDisables(false);
    // Set current tree
    ui->treeViewCurrent->setModel(&m_modelCurrent);
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
    ui->pushButtonSave->setEnabled(false);
	// Disable back/forward buttons if necessary
	if (!m_stackdirchange.canUndo()) { ui->pushBack->setEnabled(false); }
	else { ui->pushBack->setEnabled(true); }
	if (!m_stackdirchange.canRedo()) { ui->pushForward->setEnabled(false); }
	else { ui->pushForward->setEnabled(true); }
	// Set window title
    this->setWindowTitle("Select Wt Creator New Project Name and Folder");
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

    // set validator for project name
    ui->lineEditProjName->setValidator(new QRegExpValidator( QRegExp("[a-z,A-Z,0-9]{1,20}"), this ));
}

DialogProjSave::~DialogProjSave()
{
    delete ui;
}

void DialogProjSave::on_pushButtonSave_clicked()
{

    QString strSelDir  = ui->lineEditSelected->text();
    // Show warning message if empty projname
    if(strSelDir.isEmpty())
    {

        QMessageBox::critical(this, tr("Wt Creator : Error !"),
                              "Destination path cannot be empty.",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }

    QString strSelProjName  = ui->lineEditProjName->text();
    // Show warning message if empty projname
    if(strSelProjName.isEmpty())
    {

        QMessageBox::critical(this, tr("Wt Creator : Error !"),
                              "Project name cannot be empty.",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }

    // Emit signal for loading project
    Q_EMIT saveNewProject(strSelDir + "/", strSelProjName);
    accept();
}

void DialogProjSave::on_pushCancel_clicked()
{
    reject();
}

void DialogProjSave::on_listViewSystem_clicked(const QModelIndex &index)
{
    if(!index.isValid()) { return; }
    QFileInfo info = m_modelCurrent.fileInfo(index);
    if(!info.isDir())
    {
        ui->lineEditSelected->clear();
        ui->pushButtonSave->setEnabled(false);
        return;
    }
    // Push to stack which automatically changes the dir
    MyUndoChangeDirSave *command = new MyUndoChangeDirSave(this, m_modelCurrent.fileInfo(ui->treeViewCurrent->rootIndex()).absoluteFilePath(), info.absoluteFilePath());
    m_stackdirchange.push(command);
	// Disable back/forward buttons if necessary
	if (!m_stackdirchange.canUndo()) { ui->pushBack->setEnabled(false); }
	else { ui->pushBack->setEnabled(true); }
	if (!m_stackdirchange.canRedo()) { ui->pushForward->setEnabled(false); }
	else { ui->pushForward->setEnabled(true); }
}

void DialogProjSave::on_treeViewCurrent_clicked(const QModelIndex &index)
{
    if(!index.isValid()) { return; }
    QFileInfo info = m_modelCurrent.fileInfo(index);
    // Return if not Dir
    if(!info.isDir())
    {
        ui->lineEditSelected->clear();
        ui->pushButtonSave->setEnabled(false);
        return;
    }
    ui->lineEditSelected->setText(info.absoluteFilePath());
    ui->pushButtonSave->setEnabled(true);
    return;
}

void DialogProjSave::on_treeViewCurrent_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid()) { return; }
    QFileInfo info = m_modelCurrent.fileInfo(index);
    if(!info.isDir())
    {
        ui->lineEditSelected->clear();
        ui->pushButtonSave->setEnabled(false);
        return;
    }
    // Push to stack which automatically changes the dir
    MyUndoChangeDirSave *command = new MyUndoChangeDirSave(this, m_modelCurrent.fileInfo(ui->treeViewCurrent->rootIndex()).absoluteFilePath(),info.absoluteFilePath());
    m_stackdirchange.push(command);
	// Disable back/forward buttons if necessary
	if (!m_stackdirchange.canUndo()) { ui->pushBack->setEnabled(false); }
	else { ui->pushBack->setEnabled(true); }
	if (!m_stackdirchange.canRedo()) { ui->pushForward->setEnabled(false); }
	else { ui->pushForward->setEnabled(true); }
}

void DialogProjSave::on_pushBack_clicked()
{
    m_stackdirchange.undo();
	// Disable back/forward buttons if necessary
	if (!m_stackdirchange.canUndo()) { ui->pushBack->setEnabled(false); }
	else { ui->pushBack->setEnabled(true); }
	if (!m_stackdirchange.canRedo()) { ui->pushForward->setEnabled(false); }
	else { ui->pushForward->setEnabled(true); }
}

void DialogProjSave::on_pushForward_clicked()
{
    m_stackdirchange.redo();
	// Disable back/forward buttons if necessary
	if (!m_stackdirchange.canUndo()) { ui->pushBack->setEnabled(false); }
	else { ui->pushBack->setEnabled(true); }
	if (!m_stackdirchange.canRedo()) { ui->pushForward->setEnabled(false); }
	else { ui->pushForward->setEnabled(true); }
}

void DialogProjSave::on_pushUp_clicked()
{
    QFileInfo info = m_modelCurrent.fileInfo(ui->treeViewCurrent->rootIndex());
    if(!info.isDir()){ return; }
    QDir dirUp = QDir(info.absoluteFilePath());
	if(!dirUp.cdUp()){ return; } // Move dir up
    // Push to stack which automatically changes the dir
    MyUndoChangeDirSave *command = new MyUndoChangeDirSave(this, m_modelCurrent.fileInfo(ui->treeViewCurrent->rootIndex()).absoluteFilePath(),dirUp.absolutePath());
    m_stackdirchange.push(command);
	// Disable back/forward buttons if necessary
	if (!m_stackdirchange.canUndo()) { ui->pushBack->setEnabled(false); }
	else { ui->pushBack->setEnabled(true); }
	if (!m_stackdirchange.canRedo()) { ui->pushForward->setEnabled(false); }
	else { ui->pushForward->setEnabled(true); }
}


MyUndoChangeDirSave::MyUndoChangeDirSave(DialogProjSave *dialog, QString strOldDir, QString strNewDir, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_pdialog   = dialog   ;
    m_strOldDir = strOldDir;
    m_strNewDir = strNewDir;
}

void MyUndoChangeDirSave::undo()
{
    m_pdialog->ui->treeViewCurrent->setRootIndex(m_pdialog->m_modelCurrent.index(m_strOldDir));
}

void MyUndoChangeDirSave::redo()
{
    m_pdialog->ui->treeViewCurrent->setRootIndex(m_pdialog->m_modelCurrent.index(m_strNewDir));
}

void DialogProjSave::resetComboBox()
{
    ui->comboBox->blockSignals(true);
    ui->comboBox->setModel(&m_modelCurrent);
    ui->comboBox->setRootModelIndex(m_modelCurrent.index(QDir::homePath()));
    ui->comboBox->blockSignals(false);
}

void DialogProjSave::on_comboBox_activated(int index)
{
    QModelIndex indexParent = ui->comboBox->rootModelIndex();
    QFileInfo info = m_modelCurrent.fileInfo( indexParent.child(index,0) );

    if(!info.isDir())
    {
        ui->lineEditSelected->clear();
        ui->pushButtonSave->setEnabled(false);
        return;
    }

    // Push to stack which automatically changes the dir
    MyUndoChangeDirSave *command = new MyUndoChangeDirSave(this, m_modelCurrent.fileInfo(ui->treeViewCurrent->rootIndex()).absoluteFilePath(), info.absoluteFilePath());
    m_stackdirchange.push(command);
    // Disable back/forward buttons if necessary
    if (!m_stackdirchange.canUndo()) { ui->pushBack->setEnabled(false); }
    else { ui->pushBack->setEnabled(true); }
    if (!m_stackdirchange.canRedo()) { ui->pushForward->setEnabled(false); }
    else { ui->pushForward->setEnabled(true); }
}
