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

#ifndef MYUNDOREMOVE_H
#define MYUNDOREMOVE_H

#include<QUndoCommand>
#include "./modelview/mytreemodel.h"
#include "./modelview/mysignalslotsmodel.h"

class MainWindow;

// ---------------------------------------------------------------------------------------------------------------------------------------------------

// remove tree element command
class MyUndoRemoveElem : public QUndoCommand
{
public:
    MyUndoRemoveElem(MyTreeModel *ptreemodel, QString &elemname, MainWindow *pmainwindow, QUndoCommand * parent = 0);

	virtual void undo();
	virtual void redo();

private:
	MainWindow  *m_pmainwindow;
	MyTreeModel *m_ptreemodel ;
	QString      m_elemname   ;
	QString      m_parentname ;
	QByteArray   m_configchunk;
	int          m_row;

};

// append tree element command (used by drag and drop, copy - paste and so on)
class MyUndoAppendElem : public QUndoCommand
{
public:
	MyUndoAppendElem(MyTreeModel *ptreemodel, QByteArray configchunk, QString &parentname, int &irow, MainWindow *pmainwindow, QUndoCommand * parent = 0);

	virtual void undo();
	virtual void redo();

private:
	MainWindow  *m_pmainwindow;
	MyTreeModel *m_ptreemodel;
	QString      m_parentname;
	QByteArray   m_configchunk;
	int          m_row;

};

// TODO edit property command (NOT FEASIBLE why?) -----------------------------------------------------------------------------------------------------

class MyUndoEditProperty : public QUndoCommand
{
public:
	MyUndoEditProperty(MainWindow *pmainwindow, QString strElemChanged, QString strPropChanged, QString strOldPropVal, QString strNewPropVal, QUndoCommand * parent = 0);

	virtual void undo();
	virtual void redo();

private:
	MainWindow  *m_pmainwindow   ;
	QString      m_strElemChanged;
	QString      m_strPropChanged;
	QString      m_strOldPropVal ;
	QString      m_strNewPropVal ;
};

// ---------------------------------------------------------------------------------------------------------------------------------------------------

// remove connection command
class MyUndoRemoveConnect : public QUndoCommand
{
public:
	MyUndoRemoveConnect(MySignalSlotModel *psigslotmodel, int irow, MainWindow *pmainwindow, QUndoCommand * parent = 0);

	virtual void undo();
	virtual void redo();

private:
	MainWindow        *m_pmainwindow;
	MySignalSlotModel *m_psigslotmodel;
	int                m_row;
	QString            m_strSender;
	QString            m_strSignal;
	QString            m_strReceiver;
	QString            m_strSlot;
	QString            m_strParameter;
};

// append connection command
class MyUndoAppendConnect: public QUndoCommand
{
public:
	MyUndoAppendConnect(MySignalSlotModel *psigslotmodel, MainWindow *pmainwindow, QUndoCommand * parent = 0);

	virtual void undo();
	virtual void redo();

private:
	MainWindow        *m_pmainwindow;
	MySignalSlotModel *m_psigslotmodel;
	int                m_row;
	QString            m_strSender;
	QString            m_strSignal;
	QString            m_strReceiver;
	QString            m_strSlot;
	QString            m_strParameter;
};

// ---------------------------------------------------------------------------------------------------------------------------------------------------

//// remove style command
//class MyUndoRemoveStyle : public QUndoCommand
//{
//public:
//	MyUndoRemoveStyle(MyTreeModel *ptreemodel, QString strFilePath, MainWindow *pmainwindow, QUndoCommand * parent = 0);
//
//	virtual void undo();
//	virtual void redo();
//
//private:
//	MainWindow         *m_pmainwindow;
//	MyTreeModel        *m_ptreemodel;
//	QString             m_strFilePath;
//
//};
//
//
//// append style command
//
//class MyUndoAppendStyle : public QUndoCommand
//{
//public:
//	MyUndoAppendStyle(MyTreeModel *ptreemodel, QString strFilePath, MainWindow *pmainwindow, QUndoCommand * parent = 0);
//
//	virtual void undo();
//	virtual void redo();
//
//private:
//	MainWindow         *m_pmainwindow;
//	MyTreeModel        *m_ptreemodel;
//	QString             m_strFilePath;
//
//};

#endif // MYUNDOREMOVE_H
