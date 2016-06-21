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

#include "mywidgetmodel.h"
#include "myglobals.h"

WWidgetNode::WWidgetNode(WWIDGETNODETYPE intNodeType, QString &strPath, int row, WWidgetNode *parent /*= 0*/) : QObject(parent)
{
	m_intNodeType     = intNodeType;
	m_strPath         = strPath;
	rowNum            = row;
	parentWWidgetNode = parent;

	m_strName         = "";
	m_byteConfig      = "";
	m_icon            = QIcon();

	setupNodeChildren();
}


void WWidgetNode::setupNodeChildren()
{
	QFileInfo info(m_strPath);
	if (!info.isDir()) { qDebug() << "[ERROR] Not a directory in WWidgetNode::setupNodeChildren"; }

	QDir          dir(info.absoluteFilePath());
	QFileInfo     newinfo;
	QFileInfoList list;
	WWidgetNode * child;
	int           count = 0;
	// loop all entries in dir
	for (int i = 0; i < dir.entryInfoList().count(); i++)
	{
		// get entry of this dir
		newinfo = dir.entryInfoList().at(i);
		// continue if entry is not a dir
		if (!newinfo.fileName().compare(".") || !newinfo.fileName().compare("..")) { continue; }
		// depending on this dir type choose what to do with the cuirrent entry
		switch (m_intNodeType)
		{
		case ROOT:
			if (newinfo.isDir())
			{
				// create category child
                QString strTemp = newinfo.absoluteFilePath();
                child = new WWidgetNode(CATEGORY, strTemp, count, this);
				childWWidgetNodes.append(child);
				count++;
			}
			break;
		case CATEGORY:
			if (newinfo.isDir())
			{
				list = QDir(newinfo.absoluteFilePath()).entryInfoList();
				for (int j = 0; j < list.count(); j++)
				{
					// check if selected child dir contains a project file
					if (list.at(j).filePath().contains(".wtw"))
					{
						// create widget child
                        QString strTemp = newinfo.absoluteFilePath();
                        child = new WWidgetNode(WIDGET, strTemp, count, this);
						// discirminate some widgets (do not add to WWidgetNode tree but yes to QObject tree)
						QString strName = child->getName();
						if (strName.compare("WTabItem"  ) != 0 &&
							strName.compare("WMenuItem" ) != 0 &&
							strName.compare("WPopupItem") != 0)
						{
							// append to list of widgets
							childWWidgetNodes.append(child);
							count++;
						}
					}
				}
			}
			break;
		case WIDGET:
			if (newinfo.isFile() && newinfo.fileName().contains(".wtw"))
			{

				m_strName = newinfo.baseName(); // must be class name
				QFile file(newinfo.absoluteFilePath());
				if (!file.open(QFile::ReadOnly)) { qDebug() << "[ERROR] Opening file in WWidgetNode::setupNodeChildren : " << newinfo.absoluteFilePath(); }
				m_byteConfig = file.readAll();
			}
			else if (newinfo.isDir() && QDir(newinfo.absoluteFilePath()).dirName().compare("icon", Qt::CaseInsensitive) == 0)
			{
				list = QDir(newinfo.absoluteFilePath()).entryInfoList();
				for (int j = 0; j < list.count(); j++)
				{
					// check if selected child is icon file
					if (list.at(j).baseName().compare(dir.dirName()) == 0)
					{
						// get icon
						m_icon = QIcon(list.at(j).absoluteFilePath());
					}
				}
			}
			break;
		default:
			break;
		}

	}

	if (m_intNodeType == WIDGET)
	{
	    Icons::GetCache().insert(m_strName, m_icon);
	}

	// set name for category
	if (m_intNodeType == CATEGORY && count > 0)
	{
		m_strName = dir.dirName();
	}

	// just in case
	updateRowNumbers();
}


WWidgetNode::~WWidgetNode()
{
	// delete all children
	qDeleteAll(childWWidgetNodes);
	childWWidgetNodes.clear();
}

WWidgetNode * WWidgetNode::getChild(int i)
{
	// check if exists
	if (i >= 0 && i < childWWidgetNodes.count() && childWWidgetNodes[i] != NULL)
	{
		return childWWidgetNodes[i];
	}
	// else return NULL
	return NULL;
}

WWidgetNode * WWidgetNode::getParent()
{
	return parentWWidgetNode;
}

void WWidgetNode::setRow(int i)
{
	rowNum = i;
}

int WWidgetNode::getRow()
{
	return rowNum;
}

int WWidgetNode::countChild()
{
	return childWWidgetNodes.count();
}

void WWidgetNode::updateRowNumbers()
{
	for (int i = 0; i < childWWidgetNodes.size(); i++)
	{
		if (childWWidgetNodes[i])
		{
			childWWidgetNodes[i]->setRow(i);
		}
		else
		{ // remove if NULL
			childWWidgetNodes.remove(i);
			i--;
		}
	}
}

void WWidgetNode::debugPrintState(QString header /*= ""*/)
{
	//qDebug() << "-----------------------[DEBUGWDOMELEMSTATE]-------------------------";
	//for (int i = 0; i < countChild(); i++)
	//{
	//	qDebug() << getChild(i)->getRow() << ") " << getChild(i)->getString();
	//}
	//qDebug() << "[END]----------------------------------------------------------\n";
}

WWIDGETNODETYPE WWidgetNode::getNodeType()
{
	return m_intNodeType;
}

void WWidgetNode::setNodeType(WWIDGETNODETYPE &intNodeType)
{
	m_intNodeType = intNodeType;
}

QString WWidgetNode::getName()
{
	return m_strName;
}

QIcon WWidgetNode::getIcon()
{
    return m_icon;
}

QByteArray WWidgetNode::getConfig()
{
    return m_byteConfig;
}



// *************************************************************************************************************************




MyWidgetModel::MyWidgetModel(QString strPath, QObject *parent /*= 0*/) : QAbstractItemModel(parent)
{
	// create internal root, which auto load categories and widgets
	wRootNode    = new WWidgetNode(WWIDGETNODETYPE::ROOT, strPath, 0, NULL);
}

MyWidgetModel::~MyWidgetModel()
{
	if (wRootNode)
	{
		delete wRootNode;
	}
}


Qt::ItemFlags MyWidgetModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
	{
		return Qt::ItemIsEnabled;
	}

	return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant MyWidgetModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
	{
		return QVariant();
	}

	WWidgetNode *welem = static_cast<WWidgetNode*>(index.internalPointer());
	QString strName = welem->getName();

	// display name
	if( role == Qt::DisplayRole )
	{	
		switch( index.column() )
		{
		case 0:
			return strName;
			break;

		default:
			break;
		}
	}

	// display icon
	if (role == Qt::DecorationRole)
	{
		if (!welem->getIcon().isNull())
		{
			return welem->getIcon();
		}
	}

	return QVariant();
}

QVariant MyWidgetModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole */) const
{
	if( role != Qt::DisplayRole || orientation != Qt::Horizontal )
	{
		return QVariant();
	}
	// headers for id and classname
	switch( section )
	{
	case 0:
		return "WWidget";
	default:
		return QVariant();
	}
}

int MyWidgetModel::rowCount(const QModelIndex &parent /*= QModelIndex() */) const
{
	if (parent.column() > 0)
	{
		return 0;
	}

	WWidgetNode *parentElem;

	if (!parent.isValid())
	{
		parentElem = wRootNode;	
	}
	else
	{
		parentElem = static_cast<WWidgetNode*>( parent.internalPointer() );	
	}

	// return number of children
	return parentElem->countChild();
}

int MyWidgetModel::columnCount(const QModelIndex &parent /*= QModelIndex() */) const
{
	Q_UNUSED(parent)
	return 1;
}

QModelIndex MyWidgetModel::index(int row, int column, const QModelIndex &parent /*= QModelIndex() */) const
{
	// check validity
	if (!hasIndex(row, column, parent))
	{
		return QModelIndex();	
	}

	WWidgetNode *parentElem;
	// get parent
	if (!parent.isValid())
	{
		parentElem = wRootNode;	
	}
	else
	{
		parentElem = static_cast<WWidgetNode*>( parent.internalPointer() );	
	}

	// get child at row from parent (this creates the WWidgetNode auto-magically if not exists already)
	WWidgetNode *childElem = parentElem->getChild(row);
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

QModelIndex MyWidgetModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
	{
		return QModelIndex();	
	}


	WWidgetNode *childElem  = static_cast<WWidgetNode*>( index.internalPointer() );
	WWidgetNode *parentElem = childElem->getParent();

	if (!parentElem || parentElem == wRootNode)
	{
		return QModelIndex();	
	}

	return createIndex(parentElem->getRow(), 0, parentElem);
}


void MyWidgetModel::debugTreeState(WWidgetNode * welem, QString header /*= ""*/, QString tabs /*= ""*/)
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

QByteArray MyWidgetModel::getWidgetConfigByName(QString strWidgetName)
{
	return findWidgetConfigRecursive(wRootNode, strWidgetName);
}

QByteArray MyWidgetModel::findWidgetConfigRecursive(WWidgetNode * wparent, QString &strWidgetName)
{
	// recurse using QObject tree (to include hidden widgets as well)
	if (wparent->getName().compare(strWidgetName) == 0)
	{
		return wparent->getConfig();
	}
	else
	{
		for (int i = 0; i < wparent->children().count(); i++)
		{
			QByteArray config = findWidgetConfigRecursive(qobject_cast<WWidgetNode*>( wparent->children().at(i) ), strWidgetName);
			if (!config.isEmpty())
			{
				return config;
			}
		}
	}
	return QByteArray();
}
