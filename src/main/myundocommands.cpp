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

#include "mainwindow.h"

// MyUndoRemove -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

MyUndoRemoveElem::MyUndoRemoveElem(MyTreeModel *ptreemodel, QString &elemname, MainWindow *pmainwindow, QUndoCommand * parent /*= 0*/) : QUndoCommand(parent)
{
	// set command description
	setText("remove element");
	// copy main window
	m_pmainwindow = pmainwindow;
	// store for internal use
	m_ptreemodel  = ptreemodel;
	m_elemname    = elemname;
	// get config chunk to be deleted
	WDomElem * welem = m_ptreemodel->getElemByName(m_elemname);
	QDomElement elem = welem->getElem();
	// copy parent name
	m_parentname = welem->getParent()->getElem().attribute(g_strIdAttr);
	// copy row in parent
	m_row = welem->getRow();
	// deep copy the dom part to another document
	QDomDocument doc;
	QDomElement rootElem = doc.createElement(g_strValidNodeTag);
	rootElem = elem.cloneNode().toElement();
	doc.appendChild(rootElem);
	m_configchunk = doc.toByteArray();
}

void MyUndoRemoveElem::undo()
{
	// insert config chunk into tree
	QDomDocument doc;
	doc.setContent(m_configchunk);
	QDomElement elem = doc.documentElement().cloneNode().toElement();
	if (m_ptreemodel->insertElem(m_row, elem, m_ptreemodel->getIndexByName(m_parentname)))
	{
		// insert into webpage
		Q_EMIT m_pmainwindow->InsertConfig(m_configchunk, m_parentname, m_row);
		// connect any associated signals
		for (int i = 0; i < elem.childNodes().count(); i++)
		{
			if (elem.childNodes().at(i).toElement().tagName().compare(g_strConnectionTag, Qt::CaseInsensitive) == 0)
			{
				Q_EMIT m_pmainwindow->ConnectionChanged(elem.attribute(g_strIdAttr), 
														elem.childNodes().at(i).toElement().attribute("Wt_Signal"),
														elem.childNodes().at(i).toElement().attribute("Wt_Receiver"),
														elem.childNodes().at(i).toElement().attribute("Wt_Slot"),
														elem.childNodes().at(i).toElement().attribute("Wt_Parameter"));
			}
		}
	}
	else
	{
		qDebug() << "[ERROR] : MyUndoRemove, error in m_ptreemodel->insertElem";
	}
	

}

void MyUndoRemoveElem::redo()
{
	// disconnect any associated signals (needs to be before remove, while m_elemname exists)
	QDomElement elem = m_ptreemodel->getElemByName(m_elemname)->getElem();
	for (int i = 0; i < elem.childNodes().count(); i++)
	{
		if (elem.childNodes().at(i).toElement().tagName().compare(g_strConnectionTag, Qt::CaseInsensitive) == 0)
		{
			Q_EMIT m_pmainwindow->m_sigslotmodel.DisconnectOldSignal(elem.attribute(g_strIdAttr),
				elem.childNodes().at(i).toElement().attribute("Wt_Signal"),
				elem.childNodes().at(i).toElement().attribute("Wt_Receiver"),
				elem.childNodes().at(i).toElement().attribute("Wt_Slot"),
				elem.childNodes().at(i).toElement().attribute("Wt_Parameter"));
		}
	}
	// store classname in case is popupmenu and need to reload (cannot be after removed element, therefore here)
	WDomElem *wdomelem   = m_ptreemodel->getElemByName(m_elemname);
	QString strClassName = wdomelem->getElem().attribute("Wt_className");
	bool  boolIsInvolved = m_ptreemodel->isInvolvedInConnection(m_elemname); // tested down recursivelly
	// remove from tree
	if (m_ptreemodel->removeElemByName(m_elemname))
	{
		if ( strClassName.compare("WPopupMenu") == 0 || boolIsInvolved)
		{
			// shutdown and start again
			m_pmainwindow->on_ReloadWtServer();
			return;
		}
		// remove from webpage
		Q_EMIT m_pmainwindow->ElementEliminated(m_elemname);
		// select any other item
		m_pmainwindow->on_treeviewWtTree_clicked(m_pmainwindow->ui.treeviewWtTree->currentIndex());
	}
	else
	{
		qDebug() << "[ERROR] : MyUndoRemove, error in m_ptreemodel->removeElemByName";
	}
}

// MyUndoAppend -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

MyUndoAppendElem::MyUndoAppendElem(MyTreeModel *ptreemodel, QByteArray configchunk, QString &parentname, int &irow, MainWindow *pmainwindow, QUndoCommand * parent /*= 0*/) : QUndoCommand(parent)
{
	// set command description
	setText("append element");
	// copy main window
	m_pmainwindow = pmainwindow;
	// store for internal use
	m_configchunk = configchunk;
	m_ptreemodel  = ptreemodel;
	m_parentname  = parentname;
	m_row         = irow;
}

void MyUndoAppendElem::undo()
{
	QDomElement elem = m_ptreemodel->getElemByName(m_parentname)->getChild(m_row)->getElem();
	QString elemname = elem.attribute(g_strIdAttr);
	// disconnect any associated signals
	for (int i = 0; i < elem.childNodes().count(); i++)
	{
		if (elem.childNodes().at(i).toElement().tagName().compare(g_strConnectionTag, Qt::CaseInsensitive) == 0)
		{
			Q_EMIT m_pmainwindow->m_sigslotmodel.DisconnectOldSignal(elem.attribute(g_strIdAttr),
				elem.childNodes().at(i).toElement().attribute("Wt_Signal"),
				elem.childNodes().at(i).toElement().attribute("Wt_Receiver"),
				elem.childNodes().at(i).toElement().attribute("Wt_Slot"),
				elem.childNodes().at(i).toElement().attribute("Wt_Parameter"));
		}
	}
	// remove from tree
	if (m_ptreemodel->removeElemByName(elemname))
	{
		WDomElem *wdomparentelem = m_ptreemodel->getElemByName(m_parentname);
		if (wdomparentelem->getElem().attribute("Wt_className").compare("WNavigationBar") == 0 ||
			(m_ptreemodel->isHiddenRootElem(wdomparentelem->getParent()) && elem.childNodes().count() > 0))
		{
			// shutdown and start again
			m_pmainwindow->on_ReloadWtServer();
			return;
		}
		// remove from webpage
		Q_EMIT m_pmainwindow->ElementEliminated(elemname);
		// select any other item
		m_pmainwindow->on_treeviewWtTree_clicked(m_pmainwindow->ui.treeviewWtTree->currentIndex());
	}
	else
	{
		qDebug() << "[ERROR] : MyUndoRemove, error in m_ptreemodel->removeElemByName";
	}
}

void MyUndoAppendElem::redo()
{
	// insert config chunk into tree
	QDomDocument doc;
	doc.setContent(m_configchunk);
	QDomElement elem = doc.documentElement().cloneNode().toElement();
	if (m_ptreemodel->insertElem(m_row, elem, m_ptreemodel->getIndexByName(m_parentname)))
	{
		WDomElem *wdomparentelem = m_ptreemodel->getElemByName(m_parentname);
		// adding siblings next to WNavigationBar and WMenu requires reload
		bool isSiblingOfNeedsReload = false;
		for (int i = 0; i < wdomparentelem->countChild(); i++)
		{
			if (wdomparentelem->getChild(i)->getElem().attribute("Wt_className").compare("WNavigationBar") == 0 ||
				wdomparentelem->getChild(i)->getElem().attribute("Wt_className").compare("WMenu") == 0)
			{
				isSiblingOfNeedsReload = true;
			}
		}
		// some cases require a hard reload to render in proper fashion
		if (  wdomparentelem->getElem().attribute("Wt_className").compare("WNavigationBar") == 0 ||
			 (m_ptreemodel->isHiddenRootElem(wdomparentelem->getParent()) && elem.childNodes().count() > 0) ||
			 isSiblingOfNeedsReload ||
			 m_ptreemodel->isInvolvedInConnection(elem.attribute(g_strIdAttr)) // tested down recursivelly
			 )
		{
			// shutdown and start again
			m_pmainwindow->on_ReloadWtServer();
			return;
		}
		// overwrite m_configchunk after possible repeated Id correction
		doc.clear();
		QDomElement rootElem = doc.createElement(g_strValidNodeTag);
		rootElem = elem.cloneNode().toElement();
		doc.appendChild(rootElem);
		m_configchunk = doc.toByteArray();
		// insert into webpage
		Q_EMIT m_pmainwindow->InsertConfig(m_configchunk, m_parentname, m_row);
		// connect any associated signals
		for (int i = 0; i < elem.childNodes().count(); i++)
		{
			if (elem.childNodes().at(i).toElement().tagName().compare(g_strConnectionTag, Qt::CaseInsensitive) == 0)
			{
				Q_EMIT m_pmainwindow->ConnectionChanged(elem.attribute(g_strIdAttr),
					elem.childNodes().at(i).toElement().attribute("Wt_Signal"),
					elem.childNodes().at(i).toElement().attribute("Wt_Receiver"),
					elem.childNodes().at(i).toElement().attribute("Wt_Slot"),
					elem.childNodes().at(i).toElement().attribute("Wt_Parameter"));
			}
		}
		// Request properties for recently inserted item
		Q_EMIT m_pmainwindow->RequestAllProperties(elem.attribute(g_strIdAttr));
	}
	else
	{
		qDebug() << "[ERROR] : MyUndoAppendElem, error in m_ptreemodel->insertElem";
	}
}

// MyUndoEditProperty -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

MyUndoEditProperty::MyUndoEditProperty(MainWindow *pmainwindow, QString strElemChanged, QString strPropChanged, QString strOldPropVal, QString strNewPropVal, QUndoCommand * parent /*= 0*/) : QUndoCommand(parent)
{
	// store for internal use
	m_pmainwindow    = pmainwindow;
	m_strElemChanged = strElemChanged;
	m_strPropChanged = strPropChanged;
	m_strOldPropVal  = strOldPropVal;
	m_strNewPropVal  = strNewPropVal;
}

void MyUndoEditProperty::undo()
{
	if (m_strPropChanged.compare(g_strIdAttr) == 0)
	{
		// find item and change it if it has not been changed already
		WDomElem * domElem = m_pmainwindow->m_treemodel.getElemByName(m_strNewPropVal);
		if (!domElem) { qDebug() << "[ERROR] Could not find element " << m_strNewPropVal << " in MyUndoEditProperty::undo"; return; }
		domElem->getElem().setAttribute(m_strPropChanged, m_strOldPropVal);
		m_pmainwindow->UpdatePropertyTree(domElem->getElem()); // refresh prop model/view?
		// fixes bug when changing id of element and then copying and pasting that element
		m_pmainwindow->m_treemodel.replaceIdInUniqueList(m_strNewPropVal, m_strOldPropVal);
	}
	else
	{
		// find item and change it if it has not been changed already
		WDomElem * domElem = m_pmainwindow->m_treemodel.getElemByName(m_strElemChanged);
		if (!domElem) { qDebug() << "[ERROR] Could not find element " << m_strElemChanged << " in MyUndoEditProperty::undo"; return; }
		domElem->getElem().setAttribute(m_strPropChanged, m_strOldPropVal);
		m_pmainwindow->UpdatePropertyTree(domElem->getElem()); // refresh prop model/view?
	}
	// some cases require reload
	if (m_strPropChanged.compare(g_strIdAttr) == 0 ||
		m_strPropChanged.compare(g_strInlineAttr) == 0)
	{
		// shutdown and start again (NOTE this does not reloads configuration or resets the internal loaded tree)
		m_pmainwindow->on_ReloadWtServer();
	}
	else
	{
		// emit change
		Q_EMIT m_pmainwindow->PropertyChanged(m_strElemChanged, m_strPropChanged, m_strOldPropVal);
	}
}

void MyUndoEditProperty::redo()
{
	if (m_strPropChanged.compare(g_strIdAttr) == 0)
	{
		// find item and change it if it has not been changed already
		WDomElem * domElem = m_pmainwindow->m_treemodel.getElemByName(m_strNewPropVal);
		if (!domElem)
		{
			domElem = m_pmainwindow->m_treemodel.getElemByName(m_strOldPropVal);
			if (!domElem) { qDebug() << "[ERROR] Could not find element " << m_strOldPropVal << " in MyUndoEditProperty::redo"; return; }
			domElem->getElem().setAttribute(m_strPropChanged, m_strNewPropVal);
			m_pmainwindow->UpdatePropertyTree(domElem->getElem()); // refresh prop model/view?
		}
		// fixes bug when changing id of element and then copying and pasting that element
		m_pmainwindow->m_treemodel.replaceIdInUniqueList(m_strOldPropVal, m_strNewPropVal);
	}
	else
	{
		// find item and change it if it has not been changed already
		WDomElem * domElem = m_pmainwindow->m_treemodel.getElemByName(m_strElemChanged);
		if (!domElem) { qDebug() << "[ERROR] Could not find element " << m_strElemChanged << " in MyUndoEditProperty::redo"; return; }
		domElem->getElem().setAttribute(m_strPropChanged, m_strNewPropVal);
		m_pmainwindow->UpdatePropertyTree(domElem->getElem()); // refresh prop model/view?
	}
	// some cases require reload
	if (m_strPropChanged.compare(g_strIdAttr)      == 0 ||
		m_strPropChanged.compare(g_strInlineAttr)  == 0 ||
		m_strPropChanged.compare("Wt_htmlTagName") == 0)
	{
		// shutdown and start again (NOTE this does not reloads configuration or resets the internal loaded tree)
		m_pmainwindow->on_ReloadWtServer();
	}
	else
	{
		// emit change
		Q_EMIT m_pmainwindow->PropertyChanged(m_strElemChanged, m_strPropChanged, m_strNewPropVal);
	}
}


// MyUndoRemoveConnect -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

MyUndoRemoveConnect::MyUndoRemoveConnect(MySignalSlotModel *psigslotmodel, int irow, MainWindow *pmainwindow, QUndoCommand * parent /*= 0*/) : QUndoCommand(parent)
{
	// store for internal use
	m_pmainwindow   = pmainwindow;
	m_psigslotmodel = psigslotmodel;
	m_row           = irow;
	m_strSender     = "";
}

void MyUndoRemoveConnect::undo()
{
	// load parent into signlaslot model
	m_pmainwindow->on_mouseLeftClickedOverElem(m_strSender);
	QModelIndex index = m_pmainwindow->m_treemodel.getIndexByName(m_strSender);
	m_pmainwindow->on_treeviewWtTree_clicked(index);
	// safety measure
	if (m_psigslotmodel->getRootItem() == NULL) { return; }
	// create dom element
	QDomDocument doc = m_psigslotmodel->getRootItem()->getElem().ownerDocument();
	QDomElement elem = doc.createElement(g_strConnectionTag);
	elem.setAttribute("Wt_Signal"   , m_strSignal);
	elem.setAttribute("Wt_Receiver" , m_strReceiver);
	elem.setAttribute("Wt_Slot"     , m_strSlot);
	elem.setAttribute("Wt_Parameter", m_strParameter);
	if (m_psigslotmodel->insertElem(m_row, elem))
	{
		// change tree
		Q_EMIT m_pmainwindow->ConnectionChanged(m_strSender, m_strSignal, m_strReceiver, m_strSlot, m_strParameter);
	}
	else
	{
		qDebug() << "[ERROR] : MyUndoAppendConnect, error in m_psigslotmodel->insertElem";
	}
}

void MyUndoRemoveConnect::redo()
{
	// if is not the first time the action is applied
	if (!m_strSender.isEmpty())
	{
		// load parent into signlaslot model
		m_pmainwindow->on_mouseLeftClickedOverElem(m_strSender);
		QModelIndex index = m_pmainwindow->m_treemodel.getIndexByName(m_strSender);
		m_pmainwindow->on_treeviewWtTree_clicked(index);
	}
	else
	{
		// get sender name
		WConnectElem *wroot = m_psigslotmodel->getRootItem();
		m_strSender         = wroot->getElem().attribute(g_strIdAttr);
	}
	// get signal from parent element
	QModelIndex ssindex = m_psigslotmodel->index(m_row, 0);
	QDomElement signalelem = m_psigslotmodel->getElemByIndex(ssindex)->getElem();
	// get names from objects (we have the sender from redo)
	m_strSignal    = signalelem.attribute("Wt_Signal"   );
	m_strReceiver  = signalelem.attribute("Wt_Receiver" );
	m_strSlot      = signalelem.attribute("Wt_Slot"     );
	m_strParameter = signalelem.attribute("Wt_Parameter");
	// delete the element
	if (m_psigslotmodel->removeElemByIndex(ssindex))
	{// emit change
		Q_EMIT m_psigslotmodel->DisconnectOldSignal(m_strSender, m_strSignal, m_strReceiver, m_strSlot, m_strParameter);
	}
	else
	{
		qDebug() << "[ERROR] : MyUndoRemoveConnect, error in m_psigslotmodel->removeElemByIndex";
	}
}

// MyUndoAppendConnect -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

MyUndoAppendConnect::MyUndoAppendConnect(MySignalSlotModel *psigslotmodel, MainWindow *pmainwindow, QUndoCommand * parent /*= 0*/) : QUndoCommand(parent)
{
	// store for internal use
	m_pmainwindow   = pmainwindow;
	m_psigslotmodel = psigslotmodel;
	m_strSender     = "";
}

void MyUndoAppendConnect::undo()
{
	// load parent into signlaslot model
	m_pmainwindow->on_mouseLeftClickedOverElem(m_strSender);
	QModelIndex index = m_pmainwindow->m_treemodel.getIndexByName(m_strSender);
	m_pmainwindow->on_treeviewWtTree_clicked(index);
	// get signal from parent element
	QModelIndex ssindex      = m_psigslotmodel->index(m_row, 0);
	QDomElement signalelem   = m_psigslotmodel->getElemByIndex(ssindex)->getElem();
	// get names from objects (we have the sender from redo)
	m_strSignal    = signalelem.attribute("Wt_Signal");
	m_strReceiver  = signalelem.attribute("Wt_Receiver");
	m_strSlot      = signalelem.attribute("Wt_Slot");
	m_strParameter = signalelem.attribute("Wt_Parameter");
	// delete the element
	if (m_psigslotmodel->removeElemByIndex(ssindex))
	{// emit change
		Q_EMIT m_psigslotmodel->DisconnectOldSignal(m_strSender, m_strSignal, m_strReceiver, m_strSlot, m_strParameter);
	}
	else
	{
		qDebug() << "[ERROR] : MyUndoAppendConnect, error in m_psigslotmodel->removeElemByIndex";
	}
}

void MyUndoAppendConnect::redo()
{
	// if is not the first time the action is applied
	if (!m_strSender.isEmpty())
	{
		m_pmainwindow->on_mouseLeftClickedOverElem(m_strSender);
		QModelIndex index = m_pmainwindow->m_treemodel.getIndexByName(m_strSender);
		m_pmainwindow->on_treeviewWtTree_clicked(index);
		// safety measure
		if (m_psigslotmodel->getRootItem() == NULL) { return; }
		// create dom element
		QDomDocument doc = m_psigslotmodel->getRootItem()->getElem().ownerDocument();
		QDomElement elem = doc.createElement(g_strConnectionTag);
		elem.setAttribute("Wt_Signal"   , m_strSignal);
		elem.setAttribute("Wt_Receiver" , m_strReceiver);
		elem.setAttribute("Wt_Slot"     , m_strSlot);
		elem.setAttribute("Wt_Parameter", m_strParameter);
		if (m_psigslotmodel->insertElem(m_row,elem))
		{
			// change tree
			Q_EMIT m_pmainwindow->ConnectionChanged(m_strSender, m_strSignal, m_strReceiver, m_strSlot, m_strParameter);
		}
		else
		{
			qDebug() << "[ERROR] : MyUndoAppendConnect, error in m_psigslotmodel->insertElem";
		}
	}
	else
	{
		// safety measure
		if (m_psigslotmodel->getRootItem() == NULL) { return; }
		// append element
		if (m_psigslotmodel->appendElem())
		{
			// store index of new appended elem
			WConnectElem *wroot = m_psigslotmodel->getRootItem();
			int newrow = wroot->countChild() - 1; // get last one
			m_row = newrow;
			// store sender to be able to undo
			QDomElement elem = m_psigslotmodel->getRootItem()->getChild(m_row)->getElem();
			m_strSender    = wroot->getElem().attribute(g_strIdAttr);
			m_strSignal    = elem.attribute("Wt_Signal");
			m_strReceiver  = elem.attribute("Wt_Receiver");
			m_strSlot      = elem.attribute("Wt_Slot");
			m_strParameter = elem.attribute("Wt_Parameter");
			// change tree
			Q_EMIT m_pmainwindow->ConnectionChanged(m_strSender, m_strSignal, m_strReceiver, m_strSlot, m_strParameter);
		}
		else
		{
			qDebug() << "[ERROR] : MyUndoAppendConnect, error in m_psigslotmodel->appendElem";
		}
	}


}

// MyUndoRemoveStyle -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


//MyUndoRemoveStyle::MyUndoRemoveStyle(MyTreeModel *ptreemodel, QString strFilePath, MainWindow *pmainwindow, QUndoCommand * parent /*= 0*/) : QUndoCommand(parent)
//{
//	// store for internal use
//	m_pmainwindow = pmainwindow;
//	m_ptreemodel  = ptreemodel;
//	m_strFilePath = strFilePath;
//}
//
//void MyUndoRemoveStyle::undo()
//{
//	// append to xml config
//	if (m_ptreemodel->appendStyleSheet(m_strFilePath))
//	{
//		// Relead for all clients including local webview
//		m_pmainwindow->on_ReloadWtServer();
//	}
//	else
//	{
//		qDebug() << "[ERROR] : MyUndoRemoveStyle, error in m_pstylemodel->insertElem";
//	}
//}
//
//void MyUndoRemoveStyle::redo()
//{
//	// remove from xml config
//	if (m_ptreemodel->removeStyleSheet(m_strFilePath))
//	{
//		// Relead for all clients including local webview
//		m_pmainwindow->on_ReloadWtServer();
//	}
//	else
//	{
//		qDebug() << "[ERROR] : MyUndoRemoveStyle, error in m_pstylemodel->removeElemByIndex";
//	}
//}
//
//
//// MyUndoAppendStyle -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//MyUndoAppendStyle::MyUndoAppendStyle(MyTreeModel *ptreemodel, QString strFilePath, MainWindow *pmainwindow, QUndoCommand * parent /*= 0*/) : QUndoCommand(parent)
//{
//	// store for internal use
//	m_pmainwindow = pmainwindow;
//	m_ptreemodel  = ptreemodel;
//	m_strFilePath = strFilePath;
//}
//
//void MyUndoAppendStyle::undo()
//{
//	// remove from xml config
//	if (m_ptreemodel->removeStyleSheet(m_strFilePath))
//	{
//		// Relead for all clients including local webview
//		m_pmainwindow->on_ReloadWtServer();
//	}
//	else
//	{
//		qDebug() << "[ERROR] : MyUndoAppendStyle, error in m_pstylemodel->removeElemByIndex";
//	}
//}
//
//void MyUndoAppendStyle::redo()
//{
//	// append to xml config
//	if (m_ptreemodel->appendStyleSheet(m_strFilePath))
//	{
//		// Relead for all clients including local webview
//		m_pmainwindow->on_ReloadWtServer();
//	}
//	else
//	{
//		qDebug() << "[ERROR] : MyUndoAppendStyle, error in m_pstylemodel->appendElem";
//	}
//}

