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

#ifndef DIALOGPROJSAVE_H
#define DIALOGPROJSAVE_H

#include <QDialog>
#include <QFileSystemModel>
#include <QUrl>
#include <QDebug>
#include <QMessageBox>
#include <QUndoCommand>
#include <QStandardPaths>

namespace Ui {
class DialogProjSave;
}

class DialogProjSave : public QDialog
{
    Q_OBJECT

public:
    explicit DialogProjSave(QWidget *parent = 0);
    ~DialogProjSave();

    Ui::DialogProjSave * ui               ;
    QFileSystemModel       m_modelCurrent ;

Q_SIGNALS:
    void saveNewProject(QString path, QString projname);

private Q_SLOTS:
    void on_pushButtonSave_clicked();

    void on_pushCancel_clicked();

    void on_listViewSystem_clicked(const QModelIndex &index);

    void on_treeViewCurrent_clicked(const QModelIndex &index);

    void on_treeViewCurrent_doubleClicked(const QModelIndex &index);

    void on_pushBack_clicked();

    void on_pushForward_clicked();

    void on_pushUp_clicked();

    void on_comboBox_activated(int index);

private:
    QUndoStack             m_stackdirchange ;

    void resetComboBox();

};

class MyUndoChangeDirSave : public QUndoCommand
{
public:
    MyUndoChangeDirSave(DialogProjSave * dialog, QString strOldDir, QString strNewDir, QUndoCommand * parent = 0);

    virtual void undo();
    virtual void redo();

private:
    DialogProjSave   * m_pdialog   ;
    QString            m_strOldDir ;
    QString            m_strNewDir ;

};

#endif // DIALOGPROJSAVE_H
