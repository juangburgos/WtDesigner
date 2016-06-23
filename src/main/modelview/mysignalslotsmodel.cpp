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

#include "mysignalslotsmodel.h"
#include "mysignalslotsdelegate.h"
#include "myglobals.h"


WConnectElem::WConnectElem(QDomElement &elem, int row, WConnectElem *parent /*= 0*/)
{
	domElem        = elem;
	rowNum         = row;
	parentWDomElem = parent;
	childWDomElems.clear();
}

WConnectElem::~WConnectElem()
{
	// delete all children
	qDeleteAll(childWDomElems);
	childWDomElems.clear();
}

WConnectElem * WConnectElem::getChild(int i)
{
	// check if exists
	if (i >= 0 && i < childWDomElems.count() && childWDomElems[i] != NULL)
	{
		return childWDomElems[i];
	}
	// resize if necessary
	if (childWDomElems.size() < i + 1)
	{
		childWDomElems.resize(i + 1);
	}	
	// check if valid index
	if (i >= 0 && i < countChild()) 
	{
		// auto-create child and insert into children
		QDomElement  childElem    = getValidElemAt(i);
		WConnectElem   * childDomElem = new WConnectElem(childElem, i, this);
		childWDomElems.insert(i, 1, childDomElem);
		return childDomElem;
	}
	return NULL;
}

QDomElement WConnectElem::getValidElemAt(int i)
{
	int numvalid = 0;
	for (int j = 0; j < domElem.childNodes().count(); j++)
	{
		if (domElem.childNodes().at(j).toElement().tagName().compare(g_strConnectionTag, Qt::CaseInsensitive) == 0)
		{
			if (numvalid == i)
			{
				return domElem.childNodes().at(j).toElement();
			}
			numvalid++;
		}
		else if (domElem.childNodes().at(j).toElement().tagName().compare(g_strValidNodeTag) != 0)
		{
			domElem.removeChild(domElem.childNodes().at(j));
			j--;
		}
	}
	return QDomElement();
}

WConnectElem * WConnectElem::getParent()
{
	return parentWDomElem;
}

QDomElement WConnectElem::getElem()
{
	return domElem;
}

void WConnectElem::setRow(int i)
{
	rowNum = i;
}

int WConnectElem::getRow()
{
	return rowNum;
}

int WConnectElem::countChild()
{
	int numchilds = 0;
	
	for (int i = 0; i < domElem.childNodes().count(); i++)
	{
		if (domElem.childNodes().at(i).toElement().tagName().compare(g_strConnectionTag) == 0)
		{
			numchilds++;
		}
		else if (domElem.childNodes().at(i).toElement().tagName().compare(g_strValidNodeTag) != 0)
		{
			domElem.removeChild(domElem.childNodes().at(i));
			i--;
		}
	}
	return numchilds;
}

bool WConnectElem::appendChild(QDomElement &element)
{
	if (element.toElement().tagName().compare(g_strConnectionTag, Qt::CaseInsensitive) != 0)
	{
		return false;
	}
	// appenda at DOM tree level, WDomElem child is appended automagically in WDomElem::getChild (OR NOT ???)
	domElem.appendChild(element);
	// append at TREE level
	childWDomElems.resize(childWDomElems.size()+1);
	childWDomElems[childWDomElems.size()-1] = new WConnectElem(element, childWDomElems.size()-1, this);
	// update row numbers
	updateRowNumbers();

	return true;
}

bool WConnectElem::insertChild(int pos, int count, QDomElement &element)
{
	// check validity
	if (element.tagName().compare(g_strConnectionTag, Qt::CaseInsensitive) != 0 || pos < 0 || count < 1 || pos > domElem.childNodes().size())
	{
		return false;
	}
	//// decide if append or insert
	//if (pos == domElem.childNodes().size())
	//{
	//	appendChild(element);
	//}
	//else
	//{
	//	//domElem.insertBefore(element, domElem.childNodes().at(pos));
	//	domElem.insertBefore(element, getValidElemAt(pos));
	//}
	//// update row numbers
	//updateRowNumbers();

	// decide if append or insert
	if (pos >= countChild())
	{
		// append new nodes
		for (int i = 0; i < count; i++)
		{
			appendChild(element);
		}
	}
	else
	{
		// insert new nodes
		for (int i = 0; i < count; i++)
		{
			domElem.insertBefore(element, getValidElemAt(pos));
			WConnectElem * childNode = new WConnectElem(element, pos, this);
			childWDomElems.insert(pos, 1, childNode);
		}
		// update row numbers
		updateRowNumbers();
	}

	return true;
}

bool WConnectElem::removeChild(int pos, int count)
{
	// check validity
	if (pos+count > domElem.childNodes().size())
	{
		return false;
	}
	// remove children
	for (int i = 0; i < count; i++)
	{
		// remove from DOM
		//domElem.removeChild(domElem.childNodes().at(pos));
		QDomElement delem = getValidElemAt(pos);
		getElem().removeChild(delem);
		// remove from tree
		delete childWDomElems[pos];
		childWDomElems.remove(pos);

	}
	// update row numbers
	updateRowNumbers();

	return true;
}

bool WConnectElem::replaceChild(int pos, QDomElement &element)
{
	// check validity
	if (pos >= domElem.childNodes().size())
	{
		return false;
	}

	// Qt Doc: If newChild is the child of another node, it is reparented to this node. 
	// Qt Doc: If newChild is a child of this node, then its position in the list of children is changed.
	//domElem.replaceChild(element, domElem.childNodes().at(pos));
	domElem.replaceChild(element, getValidElemAt(pos));

	delete childWDomElems[pos];
	childWDomElems[pos] = new WConnectElem(element, pos, this);

	return true;
}

void WConnectElem::debugWDomElemState(QString header /*= ""*/)
{
	Q_UNUSED(header);

	//qDebug() << "-----------------------[DEBUGWDOMELEMSTATE]-------------------------";
	//qDebug() << "[DOM]--------------------" << header << "-----------------------";
	//for (int i = 0; i < getElem().childNodes().count(); i++)
	//{
	//	QDomElement elem = getElem().childNodes().at(i).toElement();
	//	qDebug() << i << ") " << elem.attribute(g_strIdAttr);
	//}
	//qDebug() << "[TREE]----------------------------------------------------------";
	//for (int i = 0; i < countChild(); i++)
	//{
	//	QDomElement elem = getChild(i)->getElem().toElement();
	//	qDebug() << getChild(i)->getRow() << ") " << elem.attribute(g_strIdAttr);
	//}
	//qDebug() << "[END]----------------------------------------------------------\n";
}

void WConnectElem::updateRowNumbers()
{
	for (int i = 0; i < childWDomElems.size(); i++)
	{
		if (childWDomElems[i] != NULL)
		{
			childWDomElems[i]->setRow(i);
		}
		else
		{ // remove if NULL
			childWDomElems.remove(i);
			i--;
		}
	}
}





// *************************************************************************************************************************

MySignalSlotModel::MySignalSlotModel(QDomElement elem /*= QDomElement()*/, QObject *parent /*= 0*/)
: QAbstractItemModel(parent)
{
	wRootElem = new WConnectElem(elem, 0, NULL);
}


MySignalSlotModel::~MySignalSlotModel()
{
	delete wRootElem;
}



Qt::ItemFlags MySignalSlotModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
	{
		return Qt::ItemIsEnabled;
	}

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

QVariant MySignalSlotModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
	{
		return QVariant();
	}

	WConnectElem *welem = static_cast<WConnectElem*>(index.internalPointer());

	// display id and classname
    if( role == Qt::DisplayRole || role == Qt::EditRole ) // also EditRole to retain old test when entering edit mode
	{
		

		switch( index.column() )
		{
		case 0:
			return welem->getElem().attribute("Wt_Signal"   , "Unknown");
			break;
		case 1:
			return welem->getElem().attribute("Wt_Receiver" , "Unknown");
			break;
		case 2:
			return welem->getElem().attribute("Wt_Slot"     , "Unknown");
			break;
		case 3:
			return welem->getElem().attribute("Wt_Parameter", "Unknown");
			break;

		default:
			break;
		}
	}

	return QVariant();
}

QVariant MySignalSlotModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole */) const
{
	if( role != Qt::DisplayRole || orientation != Qt::Horizontal )
	{
		return QVariant();
	}
	// headers for id and classname
	switch( section )
	{
	case 0:
		return "Signal";
	case 1:
		return "Receiver";
	case 2:
		return "Slot";
	case 3:
		return "Parameter";
	default:
		return QVariant();
	}
}

int MySignalSlotModel::rowCount(const QModelIndex &parent /*= QModelIndex() */) const
{
	if (parent.column() > 0)
	{
		return 0;
	}

	WConnectElem *parentElem;

	if (!parent.isValid())
	{
		parentElem = wRootElem;	
	}
	else
	{
		parentElem = static_cast<WConnectElem*>( parent.internalPointer() );	
	}

	if (parentElem == NULL)
	{
		return 0;
	}

	// return DOM tree level number of children, because removals and insertions are done also at DOM tree level
	return parentElem->countChild();
}

int MySignalSlotModel::columnCount(const QModelIndex &parent /*= QModelIndex() */) const
{
	Q_UNUSED(parent)
	return 4;
}

QModelIndex MySignalSlotModel::index(int row, int column, const QModelIndex &parent /*= QModelIndex() */) const
{
	// check validity
	if (!hasIndex(row, column, parent))
	{
		return QModelIndex();	
	}

	WConnectElem *parentElem;
	// get parent
	if (!parent.isValid())
	{
		parentElem = wRootElem;	
	}
	else
	{
		parentElem = static_cast<WConnectElem*>( parent.internalPointer() );	
	}

	// get child at row from parent (this creates the WDomElem auto-magically if not exists already)
	WConnectElem *childElem = parentElem->getChild(row);
	// if valid, return it
	if (childElem)
	{
		return createIndex(row, column, childElem);
	}
	else
	{
		return QModelIndex();	
	}
}

QModelIndex MySignalSlotModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
	{
		return QModelIndex();	
	}


	WConnectElem *childElem  = static_cast<WConnectElem*>( index.internalPointer() );
	WConnectElem *parentElem = childElem->getParent();

	if (!parentElem || parentElem == wRootElem)
	{
		return QModelIndex();	
	}

    return createIndex(parentElem->getRow(), 0, parentElem);
}

bool MySignalSlotModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    if (!index.isValid() || wRootElem == NULL)
    {
        return true;
    }

    // change data (in QDomElement)
    WConnectElem *wnode = getElemByIndex(index);

	// first disconnect old connection
	QString strOldSender    = wnode->getParent()->getElem().attribute("Wt_id");
	QString strOldSignal    = wnode->getElem().attribute("Wt_Signal");
	QString strOldReceiver  = wnode->getElem().attribute("Wt_Receiver");
	QString strOldSlot      = wnode->getElem().attribute("Wt_Slot");
	QString strOldParameter = wnode->getElem().attribute("Wt_Parameter");
	Q_EMIT DisconnectOldSignal(strOldSender, strOldSignal, strOldReceiver, strOldSlot, strOldParameter);

    switch (index.column()) 
	{
    case 0:
        wnode->getElem().setAttribute("Wt_Signal", value.toString());
        break;
    case 1:
        wnode->getElem().setAttribute("Wt_Receiver", value.toString());
        break;
    case 2:
        wnode->getElem().setAttribute("Wt_Slot", value.toString());
        break;
    case 3:
        wnode->getElem().setAttribute("Wt_Parameter", value.toString());
        break;
    default:
        break;
    }
    Q_EMIT dataChanged(index, index);

    return true;
}

bool MySignalSlotModel::removeRows(int row, int count, const QModelIndex & parent /*= QModelIndex()*/)
{
	bool result = true;

	debugTreeState(wRootElem, "[BEFORE] removeRows");
	// necessary api part
	beginRemoveRows(parent, row, row+count-1);

	// get parent
	WConnectElem *parentElem;

	if (!parent.isValid())
	{
		parentElem = wRootElem;	
	}
	else
	{
		parentElem = static_cast<WConnectElem*>(parent.internalPointer());	
	}

	// remove children from underlying DOM and WDomElem tree
	result &= parentElem->removeChild(row, count);

	// necessary api part
	endRemoveRows();

	debugTreeState(wRootElem, "[AFTER] removeRows");

	return result;
}

bool MySignalSlotModel::insertRows(int row, int count, const QModelIndex & parent /*= QModelIndex()*/)
{
	bool result = true;

	debugTreeState(wRootElem, "[BEFORE] insertRows");
	// necessary api part
	beginInsertRows(parent, row, row+count-1);
	// get parent
	WConnectElem *parentElem;

	if (!parent.isValid())
	{
		parentElem = wRootElem;	
	}
	else
	{
		parentElem = static_cast<WConnectElem*>(parent.internalPointer());	
	}

    QDomDocument domDocument = parentElem->getElem().ownerDocument();
	QDomElement  elem        = domDocument.createElement(g_strConnectionTag);
	QStringList  strListAllSignals; GetAllSignalNames(strListAllSignals);
	QStringList  strListAllSlots  ; GetAllSlotNames(strListAllSlots);
	elem.setAttribute("Wt_Signal"   , strListAllSignals.at(0));
	elem.setAttribute("Wt_Receiver" , wRootElem->getElem().attribute(g_strIdAttr));
	elem.setAttribute("Wt_Slot"     , strListAllSlots.at(0));
	elem.setAttribute("Wt_Parameter", "undefined");
	// decide whether append or insert
	if (row >= parentElem->countChild())
	{
		for (int i = 0; i < count; i++)
		{
			result &= parentElem->appendChild(elem);
		}
	}
	else
	{
		result &= parentElem->insertChild(row, count, elem);
	}
	// necessary api part
	endInsertRows();

	debugTreeState(wRootElem, "[AFTER] insertRows");

	return result;
}

bool MySignalSlotModel::appendElem(QDomElement &element /*= QDomElement()*/, const QModelIndex & parent /*= QModelIndex()*/)
{
	bool result = true;
	WConnectElem *parentElem;

	debugTreeState(wRootElem, "[BEFORE] appendNode");
	// get parent
	if (!parent.isValid())
	{
		parentElem = wRootElem;	
	}
	else
	{
		parentElem = static_cast<WConnectElem*>(parent.internalPointer());	
	}
	// get index of new appended element
	int idx = parentElem->countChild();
	// insert new default row
	result &= insertRow(idx, parent);

	// check if valid element, otherwise overwrite
    QDomDocument domDocument = parentElem->getElem().ownerDocument();
	if (element.isNull())
	{
        element = domDocument.createElement(g_strConnectionTag);
		QStringList  strListAllSignals; GetAllSignalNames(strListAllSignals);
		QStringList  strListAllSlots  ; GetAllSlotNames(strListAllSlots);
		element.setAttribute("Wt_Signal"   , strListAllSignals.at(0));
		element.setAttribute("Wt_Receiver" , wRootElem->getElem().attribute(g_strIdAttr));
		element.setAttribute("Wt_Slot"     , strListAllSlots.at(0));
		element.setAttribute("Wt_Parameter", "undefined");
	}
	// replace default element for provided element
	result &= parentElem->replaceChild(idx, element);

	debugTreeState(wRootElem, "[AFTER] appendNode");

	return result;
}

bool MySignalSlotModel::insertElem(int row, QDomElement &element /*= QDomElement()*/, const QModelIndex & parent /*= QModelIndex()*/)
{
	bool result = true;
	WConnectElem *parentElem;

	debugTreeState(wRootElem, "[BEFORE] insertElem");
	// get parent
	if (!parent.isValid())
	{
		parentElem = wRootElem;	
	}
	else
	{
		parentElem = static_cast<WConnectElem*>(parent.internalPointer());	
	}

	// insert new default row
	result &= insertRow(row, parent);

	// check if valid element, otherwise overwrite
    QDomDocument domDocument = parentElem->getElem().ownerDocument();
	if (element.isNull())
	{
        element = domDocument.createElement(g_strConnectionTag);
		QStringList  strListAllSignals; GetAllSignalNames(strListAllSignals);
		QStringList  strListAllSlots  ; GetAllSlotNames(strListAllSlots);
		element.setAttribute("Wt_Signal"   , strListAllSignals.at(0));
		element.setAttribute("Wt_Receiver" , wRootElem->getElem().attribute(g_strIdAttr));
		element.setAttribute("Wt_Slot"     , strListAllSlots.at(0));
		element.setAttribute("Wt_Parameter", "undefined");
	}
	// replace default element for provided element
	result &= parentElem->replaceChild(row, element);

	debugTreeState(wRootElem, "[AFTER] insertElem");

	return result;
}


WConnectElem * MySignalSlotModel::findItemByName(WConnectElem * parent, QString &name)
{
	QDomElement elem;
	// recursivelly search tree to look for specified Wt_id
	for (int i = 0; i < parent->countChild(); i++)
	{
		elem = parent->getChild(i)->getElem();
		if (elem.attribute("Wt_id").compare(name, Qt::CaseInsensitive) == 0)
		{
			return parent->getChild(i);
		}
		else
		{
			WConnectElem *welem = findItemByName(parent->getChild(i), name);
			if (welem != NULL)
			{
				return welem;
			}
		}
		
	}

	return NULL;
}

QModelIndex MySignalSlotModel::getIndexByItem(WConnectElem * welem)
{
	// return null index if argument is root
	if (welem == wRootElem)
	{
		return QModelIndex();
	}
	// recursivelly get index of the parent given the parent element
	QModelIndex parentIndex = getIndexByItem(welem->getParent());
	// recreate the index with parent index and row
	return index(welem->getRow(), 0, parentIndex);
}

WConnectElem * MySignalSlotModel::getElemByIndex(QModelIndex index)
{
	return static_cast<WConnectElem*>( index.internalPointer() );
}

bool MySignalSlotModel::removeElemByIndex(QModelIndex index)
{
	bool result = true;
	if (!index.isValid())
	{
		return false;
	}
	// remove element
	result &= removeRow(index.row(), parent(index));

	return result;
}

bool MySignalSlotModel::loadNewConfiguration(QDomElement elem)
{
	bool result = true;
	// test validity
    if (elem.tagName().compare(g_strValidNodeTag, Qt::CaseInsensitive) != 0)
	{
		return false;
	}
	// clear old config
	clearCurrentConfig();

	// necessary api part
	beginResetModel();

	// clean invalid elements
    cleanInvalidElems(&elem);
	// create tree root
    wRootElem = new WConnectElem(elem, 0);

	// add children
	for (int i = 0; i < elem.childNodes().count(); i++)
	{
		if (elem.childNodes().at(i).isElement() && !elem.childNodes().at(i).toElement().attribute(g_strConnectionTag).isEmpty())
		{
            QDomElement elemTemp = elem.childNodes().at(i).toElement();
            result &= insertElem(i, elemTemp, QModelIndex());
		}
	}

	// necessary api part
	endResetModel();

	debugTreeState(wRootElem, "[AFTER] loadNewConfiguration");

	return result;
}


void MySignalSlotModel::cleanInvalidElems(QDomElement *elem)
{
	for (int i = 0; i < elem->childNodes().count(); i ++)
	{
		if(!elem->isElement() ||
			(elem->childNodes().at(i).toElement().tagName().compare(g_strValidNodeTag, Qt::CaseInsensitive) != 0 && elem->childNodes().at(i).toElement().tagName().compare(g_strConnectionTag, Qt::CaseInsensitive) != 0) ||
		    (elem->childNodes().at(i).toElement().tagName().compare(g_strValidNodeTag, Qt::CaseInsensitive) == 0 && elem->childNodes().at(i).toElement().attribute(g_strIdAttr).isEmpty())
			)
		{
			elem->removeChild(elem->childNodes().at(i));
			i--;
		}
		else
		{
            QDomElement elemTemp = elem->childNodes().at(i).toElement();
            cleanInvalidElems(&elemTemp);
		}
	}
}


void MySignalSlotModel::clearCurrentConfig()
{
	if (wRootElem == NULL)
	{ // early return
		return;
	}
	beginResetModel();
	// delete treee structure
	delete wRootElem;
	wRootElem = NULL;
	endResetModel();
}

void MySignalSlotModel::debugTreeState(WConnectElem * welem, QString header /*= ""*/, QString tabs /*= ""*/)
{
	Q_UNUSED(welem)
	Q_UNUSED(header)
	Q_UNUSED(tabs)
	// recusivelly print tree state
	//if (tabs.isEmpty())
	//{
	//	qDebug() << "-----------------------[DEBUGTREESTATE]-------------------------";
	//	qDebug() << "-----------------------" << header << "-------------------------";
	//}
	//for (int i = 0; i < welem->countChild(); i++)
	//{
	//	qDebug() << tabs << "[TREE] ( " << welem->getChild(i)->getRow() << ") " << welem->getChild(i)->getElem().attribute("Wt_Receiver") << welem->getChild(i)->getElem().attribute("Wt_Signal") ;
	//}
	//if (tabs.isEmpty())
	//{
	//	qDebug() << "[END]------------------[DEBUGTREESTATE]-----------------------\n";
	//}
}
