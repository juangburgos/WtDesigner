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

#include "mytreemodel.h"
#include "myglobals.h"

#include <QMessageBox>
#include <QTextStream>
#include <QBuffer>
#include <QXmlQuery>
#include <QXmlFormatter>
#include <QDir>

WDomElem::WDomElem(QDomElement &elem, int row, WDomElem *parent /*= 0*/)
{
	domElem        = elem;
	rowNum         = row;
	parentWDomElem = parent;
}

WDomElem::~WDomElem()
{
	// delete all children
	qDeleteAll(childWDomElems);
	childWDomElems.clear();
}

WDomElem * WDomElem::getChild(int i)
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
		WDomElem   * childDomElem = new WDomElem(childElem, i, this);
		childWDomElems.insert(i, 1, childDomElem);
		return childDomElem;
	}
	return NULL;
}

QDomElement WDomElem::getValidElemAt(int i)
{
	int numvalid = 0;
	for (int j = 0; j < domElem.childNodes().count(); j++)
	{
		if (domElem.childNodes().at(j).toElement().tagName().compare(g_strValidNodeTag, Qt::CaseInsensitive) == 0 ||
			domElem.childNodes().at(j).toElement().tagName().compare(g_strWRootTag    , Qt::CaseInsensitive) == 0  )
		{
			if (numvalid == i)
			{
				return domElem.childNodes().at(j).toElement();
			}
			numvalid++;
		}
	}
	return QDomElement();
}

WDomElem * WDomElem::getParent()
{
	return parentWDomElem;
}

QDomElement WDomElem::getElem()
{
	return domElem;
}

void WDomElem::setRow(int i)
{
	rowNum = i;
}

int WDomElem::getRow()
{
	return rowNum;
}

int WDomElem::countChild()
{
	int numchilds = 0;
	for (int i = 0; i < domElem.childNodes().count(); i++)
	{
		if (domElem.childNodes().at(i).toElement().tagName().compare(g_strValidNodeTag, Qt::CaseInsensitive) == 0 ||
			domElem.childNodes().at(i).toElement().tagName().compare(g_strWRootTag    , Qt::CaseInsensitive) == 0)
		{
			numchilds++;
		}
	}
	return numchilds;
}

bool WDomElem::appendChild(QDomElement &element)
{
	if (element.toElement().tagName().compare(g_strValidNodeTag, Qt::CaseInsensitive) != 0 )
	{
		return false;
	}
	// appenda at DOM tree level, WDomElem child is appended automagically in WDomElem::getChild (OR NOT ???)
	domElem.appendChild(element);
	// append at TREE level
	childWDomElems.resize(childWDomElems.size()+1);
	childWDomElems[childWDomElems.size()-1] = new WDomElem(element, childWDomElems.size()-1, this);
	// update row numbers
	updateRowNumbers();

	return true;
}

bool WDomElem::insertChild(int pos, int count, QDomElement &element)
{
	// check validity
	if ( (element.tagName().compare(g_strValidNodeTag, Qt::CaseInsensitive) != 0 )
		|| pos < 0 || count < 1 || pos > domElem.childNodes().size())
	{
		return false;
	}

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
			WDomElem * childNode = new WDomElem(element, pos, this);
			childWDomElems.insert(pos, 1, childNode);
		}
		// update row numbers
		updateRowNumbers();
	}

	return true;
}

bool WDomElem::removeChild(int pos, int count)
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

bool WDomElem::replaceChild(int pos, QDomElement &element)
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
	childWDomElems[pos] = new WDomElem(element, pos, this);
	updateRowNumbers(); // not sure if necessary

	return true;
}

void WDomElem::debugWDomElemState(QString header /*= ""*/)
{
	Q_UNUSED(header)
	//qDebug() << "-----------------------[DEBUGWDOMELEMSTATE]-------------------------";
	//qDebug() << "[DOM]--------------------" << header << "-----------------------";
	//for (int i = 0; i < getElem().childNodes().count(); i++)
	//{
	//	QDomElement elem = getElem().childNodes().at(i).toElement();
	//	qDebug() << i << ") " << elem.attribute(strIdAttr);
	//}
	//qDebug() << "[TREE]----------------------------------------------------------";
	//for (int i = 0; i < countChild(); i++)
	//{
	//	QDomElement elem = getChild(i)->getElem().toElement();
	//	qDebug() << getChild(i)->getRow() << ") " << elem.attribute(strIdAttr);
	//}
	//qDebug() << "[END]----------------------------------------------------------\n";
}

void WDomElem::updateRowNumbers()
{
	for (int i = 0; i < childWDomElems.size(); i++)
	{
		if (childWDomElems[i])
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




MyTreeModel::MyTreeModel(QDomDocument document /*= QDomDocument()*/, QObject *parent /*= 0*/)
	: QAbstractItemModel(parent), domDocument(document)
{
	// discard cpp keywords from onset
	mstrlistAllIds = QList<QString>()  << "alignas"  << "alignof"  << "and"  << "and_eq"  << "asm"  << "auto"  << "bitand"  << "bitor"  << "bool"  << "break"  << "case"  << "catch"  << "char"  << "char16_t"  << "char32_t"  << "class"  << "compl"  << "concept"  << "const"  << "constexpr"  << "const_cast"  << "continue"  << "decltype"  << "default"  << "delete"  << "do"  << "double"  << "dynamic_cast"  << "else"  << "enum"  << "explicit"  << "export"  << "extern"  << "false"  << "float"  << "for"  << "friend"  << "goto"  << "if"  << "inline"  << "int"  << "long"  << "mutable"  << "namespace"  << "new"  << "noexcept"  << "not"  << "not_eq"  << "nullptr"  << "operator"  << "or"  << "or_eq"  << "private"  << "protected"  << "public"  << "register"  << "reinterpret_cast"  << "requires"  << "return"  << "short"  << "signed"  << "sizeof"  << "static"  << "static_assert"  << "static_cast"  << "struct"  << "switch"  << "template"  << "this"  << "thread_local"  << "throw"  << "true"  << "try"  << "typedef"  << "typeid"  << "typename"  << "union"  << "unsigned"  << "using"  << "virtual"  << "void"  << "volatile"  << "wchar_t"  << "while"  << "xor"  << "xor_eq";
	//
	if (!domDocument.isNull() && domDocument.documentElement().tagName().compare("WtCreator", Qt::CaseInsensitive) == 0)
	{
		// get wroot
		QDomElement wroot;
		wroot           = domDocument.documentElement();
		wRootHiddenElem = new WDomElem(wroot, /* row: */ 0, nullptr);
	}
	else
	{
		loadDefaultConfig();
	}
	
}

MyTreeModel::~MyTreeModel()
{
	delete wRootHiddenElem;
}


void MyTreeModel::loadDefaultConfig()
{
	QDomElement wroot;
	// create xml doc info
	QDomProcessingInstruction metanode = domDocument.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
	domDocument.appendChild(metanode);
	// create time infof
	QDomComment datenode = domDocument.createComment(QString("Fecha: %1").arg(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss")));
	domDocument.appendChild(datenode);
	// create main element
	QDomElement mainroot = domDocument.createElement("WtCreator");
	mainroot.toElement().setAttribute("Rev", "1.0");
	domDocument.appendChild(mainroot);
	// create wroot
	wroot = domDocument.createElement("WRoot");
	mainroot.appendChild(wroot);
	wRootHiddenElem = new WDomElem(wroot, /* row: */ 0, nullptr);
}


Qt::ItemFlags MyTreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
	{
		return Qt::ItemIsEnabled;
	}

	return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant MyTreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
	{
		return QVariant();
	}
	// display id and classname
	WDomElem *welem      = static_cast<WDomElem*>(index.internalPointer());
	QString strId        = welem->getElem().attribute(g_strIdAttr   , "Unknown");
	QString strClassName = welem->getElem().attribute(g_strClassAttr, "Unknown");
	if( role == Qt::DisplayRole )
	{
		switch( index.column() )
		{
		case 0:
			return strId;
			break;
		case 1:
			return strClassName;
			break;

		default:
			break;
		}
	}

	// display icon
	if (role == Qt::DecorationRole)
	{
		if ( index.column() == 1 )
		{
			return Icons::GetCache().value(strClassName);
		}
	}

	return QVariant();
}

QVariant MyTreeModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole */) const
{
	if( role != Qt::DisplayRole || orientation != Qt::Horizontal )
	{
		return QVariant();
	}
	// headers for id and classname
	switch( section )
	{
	case 0:
		return QString( "Id" );
	case 1:
		return QString( "Class" );
	default:
		return QVariant();
	}
}

int MyTreeModel::rowCount(const QModelIndex &parent /*= QModelIndex() */) const
{
	if (parent.column() > 0)
	{
		return 0;
	}

	WDomElem *parentElem;

	if (!parent.isValid())
	{
		parentElem = wRootHiddenElem;	
	}
	else
	{
		parentElem = static_cast<WDomElem*>( parent.internalPointer() );	
	}

	// return DOM tree level number of children, because removals and insertions are done also at DOM tree level
	return parentElem->countChild();
}

int MyTreeModel::columnCount(const QModelIndex &parent /*= QModelIndex() */) const
{
	Q_UNUSED(parent)
	return 2;
}

QModelIndex MyTreeModel::index(int row, int column, const QModelIndex &parent /*= QModelIndex() */) const
{
	// check validity
	if (!hasIndex(row, column, parent))
	{
		return QModelIndex();	
	}

	WDomElem *parentElem;
	// get parent
	if (!parent.isValid())
	{
		parentElem = wRootHiddenElem;	
	}
	else
	{
		parentElem = static_cast<WDomElem*>( parent.internalPointer() );	
	}

	// get child at row from parent (this creates the WDomElem auto-magically if not exists already)
	WDomElem *childElem = parentElem->getChild(row);
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

QModelIndex MyTreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
	{
		return QModelIndex();	
	}


	WDomElem *childElem  = static_cast<WDomElem*>( index.internalPointer() );
	WDomElem *parentElem = childElem->getParent();

	if (!parentElem || parentElem == wRootHiddenElem)
	{
		return QModelIndex();	
	}

	return createIndex(parentElem->getRow(), 0, parentElem);
}

bool MyTreeModel::removeRows(int row, int count, const QModelIndex & parent /*= QModelIndex()*/)
{
	bool result = true;

	debugTreeState(wRootHiddenElem, "[BEFORE] removeRows");
	// necessary api part
	beginRemoveRows(parent, row, row+count-1);

	// get parent
	WDomElem *parentElem;

	if (!parent.isValid())
	{
		parentElem = wRootHiddenElem;
		qDebug() << "[ERROR] Cannot remove Wt Root item";
		return false; // DO NOT ALLOW TO REMOVE WT ROOT
	}
	else
	{
		parentElem = static_cast<WDomElem*>(parent.internalPointer());	
	}
	// remove from Id list recursivelly
    QDomElement tmpElem = parentElem->getChild(row)->getElem();
    removeFromUniqueIdList(&tmpElem);
	// remove children from underlying DOM and WDomElem tree
	result &= parentElem->removeChild(row, count);

	// necessary api part
	endRemoveRows();

	debugTreeState(wRootHiddenElem, "[AFTER] removeRows");

	return result;
}

bool MyTreeModel::insertRows(int row, int count, const QModelIndex & parent /*= QModelIndex()*/)
{
	bool result = true;

	debugTreeState(wRootHiddenElem, "[BEFORE] insertRows");
	// necessary api part
	beginInsertRows(parent, row, row+count-1);
	// get parent
	WDomElem *parentElem;

	if (!parent.isValid())
	{
		parentElem = wRootHiddenElem;	
		qDebug() << "[ERROR] Cannot insert more than one Wt Root item";
		return false; // DO NOT ALLOW TO INSERT ANOTHER WT ROOT
	}
	else
	{
		parentElem = static_cast<WDomElem*>(parent.internalPointer());	
	}

	QDomElement elem = domDocument.createElement(g_strValidNodeTag);
	elem.setAttribute(g_strIdAttr   , "Undefined");
	elem.setAttribute(g_strClassAttr, "Undefined");
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

	debugTreeState(wRootHiddenElem, "[AFTER] insertRows");

	return result;
}

bool MyTreeModel::appendElem(QDomElement &element /*= QDomElement()*/, const QModelIndex & parent /*= QModelIndex()*/)
{
	bool result = true;
	WDomElem *parentElem;

	debugTreeState(wRootHiddenElem, "[BEFORE] appendNode");
	// get parent
	if (!parent.isValid())
	{
		parentElem = wRootHiddenElem;	
	}
	else
	{
		parentElem = static_cast<WDomElem*>(parent.internalPointer());	
	}
	// get index of new appended element
	int idx = parentElem->countChild();
	// insert new default row
	result &= insertRow(idx, parent);

	// check if valid element, otherwise overwrite
	if (element.isNull())
	{
		element = domDocument.createElement(g_strValidNodeTag);
		element.setAttribute(g_strIdAttr   , "Undefined");
		element.setAttribute(g_strClassAttr, "Undefined");
	}
	// check if id unique, else append _copy and check again
	getUniqueIdList(&element);
	// replace default element for provided element
	result &= parentElem->replaceChild(idx, element);

	debugTreeState(wRootHiddenElem, "[AFTER] appendNode");

	return result;
}

bool MyTreeModel::insertElem(int row, QDomElement &element /*= QDomElement()*/, const QModelIndex & parent /*= QModelIndex()*/)
{
	bool result = true;
	WDomElem *parentElem;

	debugTreeState(wRootHiddenElem, "[BEFORE] insertElem");
	// get parent
	if (!parent.isValid())
	{
		parentElem = wRootHiddenElem;	
	}
	else
	{
		parentElem = static_cast<WDomElem*>(parent.internalPointer());	
	}

	// insert new default row
	result &= insertRow(row, parent);

	// check if valid element, otherwise overwrite
	if (element.isNull())
	{
		element = domDocument.createElement(g_strValidNodeTag);
		element.setAttribute(g_strIdAttr   , "Undefined");
		element.setAttribute(g_strClassAttr, "Undefined");
	}
	// check if id unique, else append _copy and check again
	getUniqueIdList(&element);
	// replace default element for provided element
	debugTreeState(wRootHiddenElem, "[BEFORE] replaceChild");
	result &= parentElem->replaceChild(row, element);
	

	// do recursively DOESNT SEEM NECESSARY, IT WAS CAUSING BUG

	debugTreeState(wRootHiddenElem, "[AFTER] insertElem");

	// fixes bug with ids incorrectly added to list when inserting fails due to undefined parent
	if (!result)
	{
		removeFromUniqueIdList(&element);
	}

	return result;
}

WDomElem * MyTreeModel::getElemByName(QString &name)
{
	return findItemByName(wRootHiddenElem, name);
}

QModelIndex MyTreeModel::getIndexByName(QString &name)
{
	WDomElem *welem = findItemByName(wRootHiddenElem, name);
	if (welem != NULL)
	{
		return getIndexByItem(welem);
	}

	return QModelIndex();
}

WDomElem * MyTreeModel::findItemByName(WDomElem * parent, QString &name)
{	// TODO : Quick fix, implement correctly 
	if (name.isEmpty())
	{
		return wRootHiddenElem;
	}
	// TODO : Quick fix, implement correctly 
	if (parent->getElem().attribute("Wt_id").compare(name, Qt::CaseInsensitive) == 0)
	{
		return parent;
	}
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
			WDomElem *welem = findItemByName(parent->getChild(i), name);
			if (welem != NULL)
			{
				return welem;
			}
		}
		
	}

	return NULL;
}

QModelIndex MyTreeModel::getIndexByItem(WDomElem * welem)
{
	// return null index if argument is root
	if (welem == wRootHiddenElem)
	{
		return QModelIndex();
	}
	// recursivelly get index of the parent given the parent element
	QModelIndex parentIndex = getIndexByItem(welem->getParent());
	// recreate the index with parent index and row
	return index(welem->getRow(), 0, parentIndex);
}

WDomElem * MyTreeModel::getElemByIndex(QModelIndex index)
{
	return static_cast<WDomElem*>( index.internalPointer() );
}

bool MyTreeModel::removeElemByName(QString &name)
{
	bool result = true;

	debugTreeState(wRootHiddenElem, "[BEFORE] removeItemByName");
	// get element index and parent element index
	QModelIndex elemIndex   = getIndexByName(name);
	// remove element
	result &= removeRow(elemIndex.row(), parent(elemIndex));

	debugTreeState(wRootHiddenElem, "[AFTER] removeItemByName");

	return result;
}

bool MyTreeModel::removeElemByIndex(QModelIndex index)
{
	bool result = true;
	// remove element
	result &= removeRow(index.row(), parent(index));

	return result;
}

void MyTreeModel::clearDocument()
{
	// clear configuration
	clearCurrentConfig();
	// create empty config
	loadDefaultConfig();
}

bool MyTreeModel::loadNewConfiguration(QByteArray config)
{
	// test validity
	QDomDocument dummydoc;
	QDomElement root;
	dummydoc.setContent(config);
	root = dummydoc.documentElement();
	if (root.tagName().compare(g_strDocumTag, Qt::CaseInsensitive) != 0)
	{
		return false;
	}
	// necessary api part
	beginResetModel();

	// clear old config
	clearCurrentConfig();
	// load configuration into dom document
	domDocument.setContent(config);
	root     = domDocument.documentElement();
	// clean invalid elements
	cleanInvalidElems(&root);
	// create unique id's qlist
	getUniqueIdList(&root);
	// create tree root
	wRootHiddenElem = new WDomElem(root, 0);
	// necessary api part
	endResetModel();

	debugTreeState(wRootHiddenElem, "[AFTER] loadNewConfiguration");

	return true;
}


void MyTreeModel::cleanInvalidElems(QDomElement *elem)
{
	for (int i = 0; i < elem->childNodes().count(); i ++)
	{
		if( !elem->isElement() ||
			(elem->childNodes().at(i).toElement().tagName().compare(g_strValidNodeTag , Qt::CaseInsensitive) != 0 && 
			 elem->childNodes().at(i).toElement().tagName().compare(g_strStylesheetTag, Qt::CaseInsensitive) != 0 &&
			 elem->childNodes().at(i).toElement().tagName().compare(g_strJavascriptTag, Qt::CaseInsensitive) != 0 &&
			 elem->childNodes().at(i).toElement().tagName().compare(g_strConnectionTag, Qt::CaseInsensitive) != 0 && 
			 elem->childNodes().at(i).toElement().tagName().compare(g_strWRootTag     , Qt::CaseInsensitive) != 0 ) ||
		    (elem->childNodes().at(i).toElement().tagName().compare(g_strValidNodeTag, Qt::CaseInsensitive) == 0 && elem->childNodes().at(i).toElement().attribute(g_strIdAttr).isEmpty()) ||
			(elem->childNodes().at(i).toElement().tagName().compare(g_strWRootTag    , Qt::CaseInsensitive) == 0 && elem->childNodes().at(i).toElement().attribute(g_strIdAttr).isEmpty())
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


void MyTreeModel::clearCurrentConfig()
{
	// clear dom tree
	QModelIndex index;
	for (int i = 0; i < wRootHiddenElem->countChild(); i++)
	{
		index = getIndexByItem(wRootHiddenElem->getChild(i));
		removeElemByIndex(index);
	}
	// delete treee structure
	delete wRootHiddenElem;
	// clear unique id's qlist
	mstrlistAllIds.clear();
	// discard cpp keywords from onset
	mstrlistAllIds = QList<QString>() << "alignas" << "alignof" << "and" << "and_eq" << "asm" << "auto" << "bitand" << "bitor" << "bool" << "break" << "case" << "catch" << "char" << "char16_t" << "char32_t" << "class" << "compl" << "concept" << "const" << "constexpr" << "const_cast" << "continue" << "decltype" << "default" << "delete" << "do" << "double" << "dynamic_cast" << "else" << "enum" << "explicit" << "export" << "extern" << "false" << "float" << "for" << "friend" << "goto" << "if" << "inline" << "int" << "long" << "mutable" << "namespace" << "new" << "noexcept" << "not" << "not_eq" << "nullptr" << "operator" << "or" << "or_eq" << "private" << "protected" << "public" << "register" << "reinterpret_cast" << "requires" << "return" << "short" << "signed" << "sizeof" << "static" << "static_assert" << "static_cast" << "struct" << "switch" << "template" << "this" << "thread_local" << "throw" << "true" << "try" << "typedef" << "typeid" << "typename" << "union" << "unsigned" << "using" << "virtual" << "void" << "volatile" << "wchar_t" << "while" << "xor" << "xor_eq";
	//
}

void MyTreeModel::debugTreeState(WDomElem * welem, QString header /*= ""*/, QString tabs /*= ""*/)
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
	//	//qDebug() << tabs << "[DOM]  ( " << i << ") " << welem->getElem().childNodes().at(i).toElement().attribute(g_strIdAttr);
	//	qDebug() << tabs << "[TREE] ( " << i << ") " << welem->getValidElemAt(i).attribute(g_strIdAttr);
	//	debugTreeState(welem->getChild(i), "", tabs + "\t");
	//}
	//if (tabs.isEmpty())
	//{
	//	qDebug() << "[END]------------------[DEBUGTREESTATE]-----------------------\n";
	//}
}

bool MyTreeModel::appendElemIntoName(QByteArray config, QString &name)
{
	return appendElemIntoIndex(config, getIndexByName(name));
}

bool MyTreeModel::appendElemIntoIndex(QByteArray config, QModelIndex index)
{
	bool result = true;
	// test validity
	QDomDocument dummydoc;
	QDomElement root;
	dummydoc.setContent(config);
	root = dummydoc.documentElement();
	if (root.tagName().compare(g_strDocumTag, Qt::CaseInsensitive) != 0)
	{
		return false;
	}
	// get actual root
	root = root.namedItem(g_strWRootTag).toElement();
	if (root.isNull())
	{
		return false;
	}

	// append all children of root node to the given parent (name)
	for (int j = 0; j < root.childNodes().count(); j++)
	{
		if (root.childNodes().at(j).toElement().tagName().compare(g_strValidNodeTag, Qt::CaseInsensitive) == 0)
		{
            QDomElement elemTemp = root.childNodes().at(j).toElement();
            result &= appendElem(elemTemp, index);
			j--;
		}
	}

	debugTreeState(wRootHiddenElem, "[AFTER] insertElemIntoIndex");

	return result;
	
}

void MyTreeModel::getUniqueIdList(QDomElement *elem)
{
	// overwrite Id if not unique by appending _copy
	QString strIdAttr = elem->attribute(g_strIdAttr, "");
	if (strIdAttr.isEmpty())
	{
		qDebug() << "[ERROR] Found empty Id in MyTreeModel::getUniqueIdList";
	}
	while (mstrlistAllIds.contains(strIdAttr))
	{
		qDebug() << "[ERROR] Repeated id in tree : " << strIdAttr << " in MyTreeModel::getUniqueIdList";
		strIdAttr += "_cp";
		elem->setAttribute(g_strIdAttr, strIdAttr);
	}
	// append unique id to list
	mstrlistAllIds.append(strIdAttr);
	// do for all children
	for (int i = 0; i < elem->childNodes().count(); i++)
	{
		// if not a valid node then continue (might be a signal or something)
		if (elem->childNodes().at(i).toElement().tagName().compare(g_strValidNodeTag, Qt::CaseInsensitive) != 0 &&
			elem->childNodes().at(i).toElement().tagName().compare(g_strWRootTag    , Qt::CaseInsensitive) != 0 )
		{
			continue;
		}
        QDomElement elemTemp = elem->childNodes().at(i).toElement();
        getUniqueIdList(&elemTemp);
	}
}

bool MyTreeModel::isHiddenRootElem(WDomElem * elem)
{
	if (elem == wRootHiddenElem)
	{
		return true;
	}
	return false;
}

QString MyTreeModel::findCloserContainer(QString &name)
{
	WDomElem * closerContainer = findCloserContainer(getElemByName(name));
	if (!closerContainer)
	{
		return QString();
	}
	return closerContainer->getElem().attribute(g_strIdAttr, "Unknown");
}

WDomElem * MyTreeModel::findCloserContainer(WDomElem * child)
{
	if (!child)
	{
		return NULL;
	}

	QDomElement delem = child->getElem();

	if (delem.attribute(g_strClassAttr).compare("WContainerWidget") == 0 ||
		delem.attribute(g_strClassAttr).compare("WAnchor"         ) == 0 ||
		delem.attribute(g_strClassAttr).compare("WGroupBox"       ) == 0 ||
		delem.attribute(g_strClassAttr).compare("WPanel"          ) == 0 ||
		delem.attribute(g_strClassAttr).compare("WMenuItem"       ) == 0 ||
		delem.attribute(g_strClassAttr).compare("WStackedWidget"  ) == 0 ||
		delem.attribute(g_strClassAttr).compare("WTabItem"        ) == 0)
	{
		return child;
	}

	if (child->getParent())
	{
		return findCloserContainer(child->getParent());
	}

	return NULL;
}

QList<QFileInfo> MyTreeModel::getAllTrackedCssFiles()
{
	QList<QFileInfo> retListTrackedCssFiles;
	// create query
	QString strQuery = "declare variable $inputDocument external;\n\
<p>\n\
{doc($inputDocument)//" + g_strStylesheetTag + "}\n\
</p>";
	// exec query
	QByteArray byteResult = execXQueryOnConfig(strQuery);
	// put result xml into a domdoc
	QDomDocument dummydoc;
	dummydoc.setContent(byteResult);
	// loop through results and get file infos
	QDomNode n = dummydoc.firstChild().firstChild();
	while (!n.isNull())
	{
		if (n.isElement())
		{
			QDomElement e = n.toElement();
			if (e.tagName().compare(g_strStylesheetTag) == 0)
			{
				retListTrackedCssFiles.append(QFileInfo(e.attribute("Path")));
			}
		}
		n = n.nextSibling();
	}
	// return list of file infos
	return retListTrackedCssFiles;
}

bool MyTreeModel::removeTrackedCssFile(QFileInfo fileRef, bool bShowDiag /*= false*/)
{
	// force relative
	QDir myDir("./");
	fileRef = QFileInfo(myDir.relativeFilePath(fileRef.absoluteFilePath()));
	//
	QDomElement parentElem = domDocument.documentElement();
	QDomElement styleElem = parentElem.firstChildElement(g_strStylesheetTag);
	while (!styleElem.isNull())
	{
		// check if exists
		QFileInfo oldFileRef(styleElem.attribute(g_strStylePathAttr));
		if (oldFileRef == fileRef)
		{
			// show info dialog
			if (bShowDiag)
			{
				QMessageBox msgBox;
				msgBox.setText("Removing CSS file from project.");
				msgBox.setInformativeText("The file " + fileRef.filePath() + " will be removed from the Wt application.");
				msgBox.setStandardButtons(QMessageBox::Ok);
				msgBox.setIcon(QMessageBox::Information);
				msgBox.setDefaultButton(QMessageBox::Ok);
				/*int ret =*/ msgBox.exec();
			}
			// remove
			parentElem.removeChild(styleElem);
			Q_EMIT removedTrackedCssFile(fileRef);
			return true;
		}
		// next sibling
		styleElem = styleElem.nextSiblingElement(g_strStylesheetTag);
	}
	// if it does not exists, cannot be removed, thus error
	qDebug() << "[ERROR] Stylesheet " << fileRef.filePath() << " does not esists in configuration in MyTreeModel::removeTrackedCssFile";
	return false;
}

bool MyTreeModel::appendTrackedCssFile(QFileInfo fileRef, bool bShowDiag /*= false*/)
{
	// force relative
	QDir myDir("./");
	fileRef = QFileInfo(myDir.relativeFilePath(fileRef.absoluteFilePath()));
	// append if not there
	QDomElement parentElem = domDocument.documentElement();
	QDomElement styleElem = parentElem.firstChildElement(g_strStylesheetTag);
	while (!styleElem.isNull())
	{
		// check if exists
		QFileInfo oldFileRef(styleElem.attribute(g_strStylePathAttr));
		if (oldFileRef == fileRef)
		{
			qDebug() << "[ERROR] Stylesheet " << fileRef.filePath() << " already in configuration in MyTreeModel::appendTrackedCssFile";
			return false;
		}
		// next sibling
		styleElem = styleElem.nextSiblingElement(g_strStylesheetTag);
	}
	// if it does not exist already then create it and append it
	QDomElement newStyleElem = domDocument.createElement(g_strStylesheetTag);
	newStyleElem.setAttribute(g_strStylePathAttr, fileRef.filePath());
	parentElem.insertBefore(newStyleElem, parentElem.firstChildElement(g_strStylesheetTag));
	// show info dialog
	if (bShowDiag)
	{
		QMessageBox msgBox;
		msgBox.setText("Appended CSS file to project.");
		msgBox.setInformativeText("The file " + fileRef.filePath() + " has been appended to the Wt application.");
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setIcon(QMessageBox::Information);
		msgBox.setDefaultButton(QMessageBox::Ok);
		/*int ret =*/ msgBox.exec();
	}
	Q_EMIT appendedTrackedCssFile(fileRef);
	return true;
}

bool MyTreeModel::isTrackedCssFile(QFileInfo fileRef)
{
	return getAllTrackedCssFiles().contains(fileRef);
}

QList<QFileInfo> MyTreeModel::getAllTrackedJsFiles()
{
	QList<QFileInfo> retListTrackedJsFiles;
	// create query
	QString strQuery = "declare variable $inputDocument external;\n\
<p>\n\
{doc($inputDocument)//" + g_strJavascriptTag + "}\n\
</p>";
	// exec query
	QByteArray byteResult = execXQueryOnConfig(strQuery);
	// put result xml into a domdoc
	QDomDocument dummydoc;
	dummydoc.setContent(byteResult);
	// loop through results and get file infos
	QDomNode n = dummydoc.firstChild().firstChild();
	while (!n.isNull())
	{
		if (n.isElement())
		{
			QDomElement e = n.toElement();
			if (e.tagName().compare(g_strJavascriptTag) == 0)
			{
				retListTrackedJsFiles.append(QFileInfo(e.attribute("Path")));
			}
		}
		n = n.nextSibling();
	}
	// return list of file infos
	return retListTrackedJsFiles;
}

bool MyTreeModel::removeTrackedJsFile(QFileInfo fileRef, bool bShowDiag /*= false*/)
{
	// force relative
	QDir myDir("./");
	fileRef = QFileInfo(myDir.relativeFilePath(fileRef.absoluteFilePath()));
	//
	QDomElement parentElem = domDocument.documentElement();
	QDomElement javasElem = parentElem.firstChildElement(g_strJavascriptTag);
	while (!javasElem.isNull())
	{
		// check if exists
		QFileInfo oldFileRef(javasElem.attribute(g_strJavasPathAttr));
		if (oldFileRef == fileRef)
		{
			// show info dialog
			if (bShowDiag)
			{
				QMessageBox msgBox;
				msgBox.setText("Removing JS file from project.");
				msgBox.setInformativeText("The file " + fileRef.filePath() + " will be removed from the Wt application.");
				msgBox.setStandardButtons(QMessageBox::Ok);
				msgBox.setIcon(QMessageBox::Information);
				msgBox.setDefaultButton(QMessageBox::Ok);
				/*int ret =*/ msgBox.exec();
			}
			// remove
			parentElem.removeChild(javasElem);
			Q_EMIT removedTrackedJsFile(fileRef);
			return true;
		}
		// next sibling
		javasElem = javasElem.nextSiblingElement(g_strJavascriptTag);
	}
	// if it does not exists, cannot be removed, thus error
	qDebug() << "[ERROR] JavaScript " << fileRef.filePath() << " does not esists in configuration in MyTreeModel::removeTrackedJsFile";
	return false;
}

bool MyTreeModel::appendTrackedJsFile(QFileInfo fileRef, bool bShowDiag /*= false*/)
{
	// force relative
	QDir myDir("./");
	fileRef = QFileInfo(myDir.relativeFilePath(fileRef.absoluteFilePath()));
	// append if not there
	QDomElement parentElem = domDocument.documentElement();
	QDomElement javasElem = parentElem.firstChildElement(g_strJavascriptTag);
	while (!javasElem.isNull())
	{
		// check if exists
		QFileInfo oldFileRef(javasElem.attribute(g_strJavasPathAttr));
		if (oldFileRef == fileRef)
		{
			qDebug() << "[ERROR] Stylesheet " << fileRef.filePath() << " already in configuration in MyTreeModel::appendTrackedJsFile";
			return false;
		}
		// next sibling
		javasElem = javasElem.nextSiblingElement(g_strJavascriptTag);
	}
	// if it does not exist already then create it and append it
	QDomElement newStyleElem = domDocument.createElement(g_strJavascriptTag);
	newStyleElem.setAttribute(g_strJavasPathAttr, fileRef.filePath());
	parentElem.insertBefore(newStyleElem, parentElem.firstChildElement(g_strJavascriptTag));
	// show info dialog
	if (bShowDiag)
	{
		QMessageBox msgBox;
		msgBox.setText("Appended JS file to project.");
		msgBox.setInformativeText("The file " + fileRef.filePath() + " has been appended to the Wt application.");
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setIcon(QMessageBox::Information);
		msgBox.setDefaultButton(QMessageBox::Ok);
		/*int ret =*/ msgBox.exec();
	}
	Q_EMIT appendedTrackedJsFile(fileRef);
	return true;
}

bool MyTreeModel::isTrackedJsFile(QFileInfo fileRef)
{
	return getAllTrackedJsFiles().contains(fileRef);
}

QByteArray MyTreeModel::execXQueryOnConfig(QString strQuery)
{
	// get input
	QByteArray byteInput = domDocument.toByteArray();
	QBuffer bufferInput(&byteInput);
	bufferInput.open(QIODevice::ReadWrite);

	// define output
	QByteArray byteOutput;
	QBuffer bufferOut(&byteOutput);
	bufferOut.open(QIODevice::ReadWrite);

	// create and check query
	QXmlQuery query;
	query.bindVariable("inputDocument", &bufferInput);
	query.setQuery(strQuery);
	if (!query.isValid())
	{
		qDebug() << "[ERROR] : Invalid Query in MyTreeModel::execXQueryOnConfig";
		return QByteArray();
	}

	// evaluate query
	QXmlFormatter formatter(query, &bufferOut);
	if (!query.evaluateTo(&formatter))
	{
		qDebug() << "[ERROR] : Could not eval Query in MyTreeModel::execXQueryOnConfig";
		return QByteArray();
	}

	// close buffers
	bufferInput.close();
	bufferOut.close();

	// get result
	return byteOutput;
}

bool MyTreeModel::isInvolvedInConnection(QString strElemId)
{
	QStringList listAllInvolvedInConnections;
	// get all involved item ids
	listAllInvolvedInConnections.append(getAllSignalSendersIds());
	listAllInvolvedInConnections.append(getAllSignalReceiverdIds());
	// test this
	if (listAllInvolvedInConnections.contains(strElemId, Qt::CaseInsensitive))
	{
		return true;
	}
	// test children recursivelly
	WDomElem * elem = getElemByName(strElemId);
	for (int i = 0; i < elem->countChild(); i++)
	{
		if (isInvolvedInConnection(elem->getChild(i)->getElem().attribute(g_strIdAttr)))
		{
			return true;
		}
	}
	// else return false
	return false;
}

QStringList MyTreeModel::getAllSignalSendersIds()
{
/*
declare variable $inputDocument external;
<senders>
{
doc($inputDocument)//WConnection/
<WObject>{../@Wt_id}</WObject>
}
</senders>
*/
	QStringList listAllSenders;
	// create query
	QString strQuery = "declare variable $inputDocument external;\n\
<senders>\n\
{\n\
doc($inputDocument)//" + g_strConnectionTag + "/\n\
<" + g_strValidNodeTag + ">{../@" + g_strIdAttr + "}</" + g_strValidNodeTag + ">\n\
}\n\
</senders>";
	// exec query
	QByteArray byteResult = execXQueryOnConfig(strQuery);

	// put result xml into a domdoc
	QDomDocument dummydoc;
	dummydoc.setContent(byteResult);
	// loop through results and get file infos
	QDomNode n = dummydoc.firstChild().firstChild();
	while (!n.isNull())
	{
		if (n.isElement())
		{
			QDomElement e = n.toElement();
			if (e.tagName().compare(g_strValidNodeTag) == 0)
			{
				listAllSenders.append(e.attribute(g_strIdAttr));
			}
		}
		n = n.nextSibling();
	}

	return listAllSenders;
}

QStringList MyTreeModel::getAllSignalReceiverdIds()
{
/*
declare variable $inputDocument external;
<receivers>
{
doc($inputDocument)//WConnection/
<WObject Wt_id="{@Wt_Receiver}"/>
}
</receivers>
*/
	QStringList listAllReceivers;
	// create query
	QString strQuery = "declare variable $inputDocument external;\n\
<receivers>\n\
{\n\
doc($inputDocument)//" + g_strConnectionTag + "/\n\
<" + g_strValidNodeTag + " " + g_strIdAttr + "=\"{@" + g_strReceiverAttr + "}\"/>\n\
}\n\
</receivers>";
	// exec query
	QByteArray byteResult = execXQueryOnConfig(strQuery);

	// put result xml into a domdoc
	QDomDocument dummydoc;
	dummydoc.setContent(byteResult);
	// loop through results and get file infos
	QDomNode n = dummydoc.firstChild().firstChild();
	while (!n.isNull())
	{
		if (n.isElement())
		{
			QDomElement e = n.toElement();
			if (e.tagName().compare(g_strValidNodeTag) == 0)
			{
				listAllReceivers.append(e.attribute(g_strIdAttr));
			}
		}
		n = n.nextSibling();
	}

	return listAllReceivers;
}

void MyTreeModel::replaceIdInUniqueList(QString strOldId, QString strNewId)
{
	// remove old, append new
	mstrlistAllIds.removeAll(strOldId);
	mstrlistAllIds.append(strNewId);
}

void MyTreeModel::removeFromUniqueIdList(QDomElement *elem)
{
	// remove id of this element
	QString strIdAttr = elem->attribute(g_strIdAttr, "");
	if (strIdAttr.isEmpty())
	{
		qDebug() << "[ERROR] Found empty Id in MyTreeModel::removeFromUniqueIdList";
	}
	// remove from list
	mstrlistAllIds.removeAll(strIdAttr);
	// do for all children
	for (int i = 0; i < elem->childNodes().count(); i++)
	{
		// if not a valid node then continue (might be a signal or something)
		if (elem->childNodes().at(i).toElement().tagName().compare(g_strValidNodeTag, Qt::CaseInsensitive) != 0 &&
			elem->childNodes().at(i).toElement().tagName().compare(g_strWRootTag, Qt::CaseInsensitive) != 0)
		{
			continue;
		}
        QDomElement tmpElem = elem->childNodes().at(i).toElement();
        removeFromUniqueIdList(&tmpElem);
	}
}
