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

#include "mypropertymodel.h"
#include "myglobals.h"
#include "../helperfunctions.h"

#include <QMessageBox>

WPropNode::WPropNode(QString &node, int row, WPropNode *parent /*= 0*/)
{
	strNode         = node;
	rowNum          = row;
	parentWPropNode = parent;
}

WPropNode::~WPropNode()
{
	// delete all children
	qDeleteAll(childWPropNodes);
	childWPropNodes.clear();
}

WPropNode * WPropNode::getChild(int i)
{
	// check if exists
	if (i >= 0 && i < childWPropNodes.count() && childWPropNodes[i] != NULL)
	{
		return childWPropNodes[i];
	}
	// else return NULL
	return NULL;
}

QString WPropNode::getValidStringAt(int i)
{
	// check if exists
	if (i >= 0 && i < childWPropNodes.count() && childWPropNodes[i] != NULL)
	{
		return childWPropNodes[i]->getString();
	}
	// else return NULL
	return QString();
}

WPropNode * WPropNode::getParent()
{
	return parentWPropNode;
}

QString WPropNode::getString()
{
	return strNode;
}

void WPropNode::setRow(int i)
{
	rowNum = i;
}

int WPropNode::getRow()
{
	return rowNum;
}

int WPropNode::countChild()
{
	return childWPropNodes.count();
}

bool WPropNode::appendChild(QString &node)
{
	WPropNode * childNode = new WPropNode(node, countChild(), this);
	childWPropNodes.append(childNode);
	return true;
}

bool WPropNode::insertChild(int pos, int count, QString &node)
{
	// decide if append or insert
	if (pos >= countChild())
	{
		// append new nodes
		for (int i = 0; i < count; i++)
		{
			appendChild(node);
		}
		
	}
	else
	{
		// insert new nodes
		for (int i = 0; i < count; i++)
		{
			WPropNode * childNode = new WPropNode(node, pos, this);
			childWPropNodes.insert(pos, 1, childNode);
		}
		// update row numbers
		updateRowNumbers();
	}

	return true;
}

bool WPropNode::removeChild(int pos, int count)
{
	// check validity
	if (pos+count > countChild())
	{
		return false;
	}
	// remove children
	for (int i = 0; i < count; i++)
	{
		delete childWPropNodes[pos];
		childWPropNodes.remove(pos);
	}
	// update row numbers
	updateRowNumbers();

	return true;
}

bool WPropNode::replaceChild(int pos, QString &node)
{
	// check validity
	if (pos >= countChild())
	{
		return false;
	}

	// just overwrite
	childWPropNodes[pos]->setString(node);

	return true;
}


void WPropNode::setString(QString &node)
{
	strNode = node;
}

void WPropNode::reverseChildren()
{
	std::reverse(childWPropNodes.begin(), childWPropNodes.end());
	// update row numbers
	updateRowNumbers();
}

void WPropNode::updateRowNumbers()
{
	for (int i = 0; i < childWPropNodes.size(); i++)
	{
		if (childWPropNodes[i])
		{
			childWPropNodes[i]->setRow(i);
		}
		else
		{ // remove if NULL
			childWPropNodes.remove(i);
			i--;
		}
	}
}

void WPropNode::debugWDomElemState(QString header /*= ""*/)
{
	//qDebug() << "-----------------------[DEBUGWDOMELEMSTATE]-------------------------";
	//for (int i = 0; i < countChild(); i++)
	//{
	//	qDebug() << getChild(i)->getRow() << ") " << getChild(i)->getString();
	//}
	//qDebug() << "[END]----------------------------------------------------------\n";
}



// *************************************************************************************************************************




MyPropertyModel::MyPropertyModel(QObject *parent /*= 0*/) : QAbstractItemModel(parent)
{
	// create internal empty root
    QString strTemp("root");
    wRootNode    = new WPropNode(strTemp, 0, NULL);
	wRootElement = QDomElement();
}

MyPropertyModel::~MyPropertyModel()
{
	if (wRootNode)
	{
		delete wRootNode;
	}
	wRootElement = QDomElement();
}


Qt::ItemFlags MyPropertyModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
	{
		return Qt::ItemIsEnabled;
	}
    // get node to test parent for WtQtObject
    WPropNode *wnode = static_cast<WPropNode*>(index.internalPointer());

	// make column 1 (second column) editable
    if (index.column() == 1 && index.parent().isValid() && wnode->getParent()->getString().compare("WObject",Qt::CaseInsensitive) != 0)
	{
		return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
	}

	return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant MyPropertyModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
	{
		return QVariant();
	}

	WPropNode *welem = static_cast<WPropNode*>(index.internalPointer());

	// display id and classname
	if( role == Qt::DisplayRole || role == Qt::EditRole ) // also EditRole to retain old test when entering edit mode
	{
		

		switch( index.column() )
		{
		case 0:

			if (welem->countChild() == 0)
			{
				return welem->getString().remove(0, 3); // Remove the "Wt_" from property name
			}
			return welem->getString();
			
			break;
		case 1:

			if (!wRootElement.isNull())
			{
				return DecodeTextXML(wRootElement.attribute(welem->getString(), "")); // Display attribute if available
			}
			return "";

			break;

		default:
			break;
		}
	}

	return QVariant();
}

QVariant MyPropertyModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole */) const
{
	if( role != Qt::DisplayRole || orientation != Qt::Horizontal )
	{
		return QVariant();
	}
	// headers for id and classname
	switch( section )
	{
	case 0:
		return "Property";
	case 1:
		return "Value";
	default:
		return QVariant();
	}
}

int MyPropertyModel::rowCount(const QModelIndex &parent /*= QModelIndex() */) const
{
	if (parent.column() > 0)
	{
		return 0;
	}

	WPropNode *parentElem;

	if (!parent.isValid())
	{
		parentElem = wRootNode;	
	}
	else
	{
		parentElem = static_cast<WPropNode*>( parent.internalPointer() );	
	}

	// return DOM tree level number of children, because removals and insertions are done also at DOM tree level
	return parentElem->countChild();
}

int MyPropertyModel::columnCount(const QModelIndex &parent /*= QModelIndex() */) const
{
	Q_UNUSED(parent)
	return 2;
}

QModelIndex MyPropertyModel::index(int row, int column, const QModelIndex &parent /*= QModelIndex() */) const
{
	// check validity
	if (!hasIndex(row, column, parent))
	{
		return QModelIndex();	
	}

	WPropNode *parentElem;
	// get parent
	if (!parent.isValid())
	{
		parentElem = wRootNode;	
	}
	else
	{
		parentElem = static_cast<WPropNode*>( parent.internalPointer() );	
	}

	// get child at row from parent (this creates the WDomElem auto-magically if not exists already)
	WPropNode *childElem = parentElem->getChild(row);
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

QModelIndex MyPropertyModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
	{
		return QModelIndex();	
	}


	WPropNode *childElem  = static_cast<WPropNode*>( index.internalPointer() );
	WPropNode *parentElem = childElem->getParent();

	if (!parentElem || parentElem == wRootNode)
	{
		return QModelIndex();	
	}

	return createIndex(parentElem->getRow(), 0, parentElem);
}


bool MyPropertyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (role != Qt::EditRole)
		return false;

	if (!index.isValid() || wRootElement.isNull())
	{
		return true;
	}
	
	// change data (in QDomElement)
	WPropNode *wnode = getNodeByIndex(index);

	// early exit (if value did not change at all)
	QString strNewValue = EncodeTextXML(value.toString());
	if (strNewValue.compare(wRootElement.attribute(wnode->getString())) == 0)
	{
		return false;
	}

	// check if repeated Id
	if (wnode->getString().compare(g_strIdAttr) == 0)
	{
		if (isNewIdCppKeyword(strNewValue))
		{
			QMessageBox msgBox;
			msgBox.setIcon(QMessageBox::Critical);
			msgBox.setText(tr("The id = \"") + strNewValue + tr("\" is a C++ keyword. Please use a different id."));
			msgBox.exec();
			return false;
		}
		if (isNewIdRepeated(strNewValue))
		{
			QMessageBox msgBox;
			msgBox.setIcon(QMessageBox::Critical);
			msgBox.setText(tr("The id = \"") + strNewValue + tr("\" is already in use. Please use a different one."));
			msgBox.exec();
			return false;
		}
	}

	// change data in xml config tree
	wnode->strOldId  = wRootElement.attribute(g_strIdAttr);
	wnode->strOldVal = wRootElement.attribute(wnode->getString());

	wRootElement.setAttribute(wnode->getString(), strNewValue);

	Q_EMIT dataChanged(index, index);

	return true;
}


bool MyPropertyModel::removeRows(int row, int count, const QModelIndex & parent /*= QModelIndex()*/)
{
	bool result = true;

	debugTreeState(wRootNode, "[BEFORE] removeRows");
	// necessary api part
	beginRemoveRows(parent, row, row+count-1);

	// get parent
	WPropNode *parentElem;

	if (!parent.isValid())
	{
		parentElem = wRootNode;	
	}
	else
	{
		parentElem = static_cast<WPropNode*>(parent.internalPointer());	
	}
	// remove children from underlying DOM and WDomElem tree
	result &= parentElem->removeChild(row, count);
	// necessary api part
	endRemoveRows();

	debugTreeState(wRootNode, "[AFTER] removeRows");

	return result;
}

bool MyPropertyModel::insertRows(int row, int count, const QModelIndex & parent /*= QModelIndex()*/)
{
	bool result = true;

	debugTreeState(wRootNode, "[BEFORE] insertRows");
	// necessary api part
	beginInsertRows(parent, row, row+count-1);
	// get parent
	WPropNode *parentElem;

	if (!parent.isValid())
	{
		parentElem = wRootNode;	
	}
	else
	{
		parentElem = static_cast<WPropNode*>(parent.internalPointer());	
	}

	// decide whether append or insert
	if (row >= parentElem->countChild())
	{
		for (int i = 0; i < count; i++)
		{
            QString strTemp = QString("default");
            result &= parentElem->appendChild(strTemp);
		}

	}
	else
	{
        QString strTemp = QString("default");
        result &= parentElem->insertChild(row, count, strTemp);
	}
	// necessary api part
	endInsertRows();

	debugTreeState(wRootNode, "[AFTER] insertRows");

	return result;
}

bool MyPropertyModel::appendNode(QString &node /*= QString("")*/, const QModelIndex & parent /*= QModelIndex()*/)
{
	bool result = true;
	WPropNode *parentElem;

	debugTreeState(wRootNode, "[BEFORE] appendNode");
	// get parent
	if (!parent.isValid())
	{
		parentElem = wRootNode;	
	}
	else
	{
		parentElem = static_cast<WPropNode*>(parent.internalPointer());	
	}
	// get index of new appended element
	int idx = parentElem->countChild();
	// insert new default row
	result &= insertRow(idx, parent);

	// check if valid element, otherwise overwrite
	if (node.isEmpty())
	{
		node = QString("default");
	}
	// replace default element for provided element
	result &= parentElem->replaceChild(idx, node);

	debugTreeState(wRootNode, "[AFTER] appendNode");

	return result;
}

bool MyPropertyModel::insertNode(int row, QString &node /*= QString("")*/, const QModelIndex & parent /*= QModelIndex()*/)
{
	bool result = true;
	WPropNode *parentElem;

	debugTreeState(wRootNode, "[BEFORE] insertElem");
	// get parent
	if (!parent.isValid())
	{
		parentElem = wRootNode;	
	}
	else
	{
		parentElem = static_cast<WPropNode*>(parent.internalPointer());	
	}

	// insert new default row
	result &= insertRow(row, parent);

	// check if valid element, otherwise overwrite
	if (node.isEmpty())
	{
		node = QString("default");
	}
	// replace default element for provided element
	result &= parentElem->replaceChild(row, node);

	debugTreeState(wRootNode, "[AFTER] insertElem");

	return result;
}

WPropNode * MyPropertyModel::getNodeByName(QString &name)
{
	return findNodeByName(wRootNode, name);
}

QModelIndex MyPropertyModel::getIndexByName(QString &name)
{
	WPropNode *welem = findNodeByName(wRootNode, name);
	if (welem != NULL)
	{
		return getIndexByNode(welem);
	}

	return QModelIndex();
}

WPropNode * MyPropertyModel::findNodeByName(WPropNode * parent, QString &name)
{
	QString node;
	// recursivelly search tree to look for specified Wt_id
	for (int i = 0; i < parent->countChild(); i++)
	{
		node = parent->getChild(i)->getString();
		if (node.compare(name, Qt::CaseInsensitive) == 0)
		{
			return parent->getChild(i);
		}
		else
		{
			WPropNode *welem = findNodeByName(parent->getChild(i), name);
			if (welem != NULL)
			{
				return welem;
			}
		}
		
	}

	return NULL;
}

QModelIndex MyPropertyModel::getIndexByNode(WPropNode * wnode)
{
	// return null index if argument is root
	if (wnode == wRootNode)
	{
		return QModelIndex();
	}
	// recursivelly get index of the parent given the parent element
	QModelIndex parentIndex = getIndexByNode(wnode->getParent());
	// recreate the index with parent index and row
	return index(wnode->getRow(), 0, parentIndex);
}

WPropNode * MyPropertyModel::getNodeByIndex(QModelIndex index)
{
	if (index.isValid())
	{
		return static_cast<WPropNode*>(index.internalPointer());
	}
	else
	{
		return wRootNode;
	}
}

bool MyPropertyModel::removeNodeByName(QString &name, const QModelIndex & parent /*= QModelIndex()*/)
{
	bool result = true;

	debugTreeState(wRootNode, "[BEFORE] removeItemByName");

	if (parent.isValid())
	{
		WPropNode *node = getNodeByIndex(parent);
		for (int i = 0; i < node->countChild(); i++)
		{
			if (node->getChild(i)->getString().compare(name, Qt::CaseInsensitive) == 0)
			{
				result &= removeRow(node->getChild(i)->getRow(), parent);
				return result;
			}
		}
		result = false;
	}
	else
	{
		// get element index and parent element index
		QModelIndex elemIndex = getIndexByName(name);
		// remove element
		result &= removeRow(elemIndex.row(), MyPropertyModel::parent(elemIndex));
	}


	debugTreeState(wRootNode, "[AFTER] removeItemByName");

	return result;
}

bool MyPropertyModel::removeNodeByIndex(QModelIndex index)
{
	bool result = true;
	// remove element
	result &= removeRow(index.row(), parent(index));

	return result;
}


void MyPropertyModel::clearCurrentConfig()
{
	beginResetModel();
	// clear dom tree
	QModelIndex index;
	while (wRootNode->countChild() > 0)
	{
		index = getIndexByNode(wRootNode->getChild(0));
		removeNodeByIndex(index);
	}
	wRootElement = QDomElement();
	endResetModel();
}

void MyPropertyModel::reverseChildren(const QModelIndex & parent /*= QModelIndex()*/)
{
	WPropNode *node = getNodeByIndex(parent);
	beginResetModel();
	node->reverseChildren();
	endResetModel();
}

void MyPropertyModel::setDomElement(QDomElement &elem)
{
	wRootElement = elem;
}

void MyPropertyModel::debugTreeState(WPropNode * welem, QString header /*= ""*/, QString tabs /*= ""*/)
{
	Q_UNUSED(welem )
	Q_UNUSED(header)
	Q_UNUSED(tabs  )
	//// recusivelly print tree state
	//if (tabs.isEmpty())
	//{
	//	qDebug() << "-----------------------[DEBUGTREESTATE]-------------------------";
	//	qDebug() << "-----------------------" << header << "-------------------------";
	//}
	//for (int i = 0; i < welem->countChild(); i++)
	//{
	//	qDebug() << tabs << "[TREE] ( " << welem->getChild(i)->getRow() << ") " << welem->getChild(i)->getString();
	//	debugTreeState( welem->getChild(i), "", tabs + "\t");
	//}
	//if (tabs.isEmpty())
	//{
	//	qDebug() << "[END]------------------[DEBUGTREESTATE]-----------------------\n";
	//}
}

bool MyPropertyModel::isNewIdCppKeyword(QString &strNewId)
{
	QStringList listCppKeywords = QStringList() << "alignas"  << "alignof"  << "and"  << "and_eq"  << "asm"  << "auto"  << "bitand"  << "bitor"  << "bool"  << "break"  << "case"  << "catch"  << "char"  << "char16_t"  << "char32_t"  << "class"  << "compl"  << "concept"  << "const"  << "constexpr"  << "const_cast"  << "continue"  << "decltype"  << "default"  << "delete"  << "do"  << "double"  << "dynamic_cast"  << "else"  << "enum"  << "explicit"  << "export"  << "extern"  << "false"  << "float"  << "for"  << "friend"  << "goto"  << "if"  << "inline"  << "int"  << "long"  << "mutable"  << "namespace"  << "new"  << "noexcept"  << "not"  << "not_eq"  << "nullptr"  << "operator"  << "or"  << "or_eq"  << "private"  << "protected"  << "public"  << "register"  << "reinterpret_cast"  << "requires"  << "return"  << "short"  << "signed"  << "sizeof"  << "static"  << "static_assert"  << "static_cast"  << "struct"  << "switch"  << "template"  << "this"  << "thread_local"  << "throw"  << "true"  << "try"  << "typedef"  << "typeid"  << "typename"  << "union"  << "unsigned"  << "using"  << "virtual"  << "void"  << "volatile"  << "wchar_t"  << "while"  << "xor"  << "xor_eq";
	for (int i = 0; i < listCppKeywords.count(); i++)
	{
		if (strNewId.compare(listCppKeywords.at(i)) == 0)
		{
			return true;
		}
	}
	return false;
}

bool MyPropertyModel::isNewIdRepeated(QString &strNewId)
{
    QDomElement elemTemp = wRootElement.ownerDocument().documentElement();
    return recursiveIdCheck(&elemTemp, strNewId);
}

bool MyPropertyModel::recursiveIdCheck(QDomElement * elem, QString &strNewId)
{
	if (elem->attribute(g_strIdAttr,"").compare(strNewId) == 0)
	{
		return true;
	}
	else
	{
		for (int i = 0; i < elem->childNodes().count(); i++)
		{
            QDomElement elemTemp = elem->childNodes().at(i).toElement();
			// if not a valid node then continue (might be a signal or something)
            if (elemTemp.tagName().compare(g_strValidNodeTag, Qt::CaseInsensitive) != 0 &&
                elemTemp.tagName().compare(g_strWRootTag, Qt::CaseInsensitive) != 0)
			{
				continue;
			}

            if(recursiveIdCheck(&elemTemp, strNewId))
			{
				return true;
			}
		}
	}
	return false;
}



