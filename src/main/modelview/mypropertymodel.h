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

#ifndef WQPROPMODEL_H
#define WQPROPMODEL_H

#include <QAbstractItemModel>
#include <QDomDocument>
#include <QDateTime>
#include <QDebug>
#include <QColor>

#include <algorithm>

class WPropNode
{
public:
	WPropNode(QString &node, int row, WPropNode *parent = 0);
	~WPropNode();
	WPropNode  * getChild(int i);
	WPropNode  * getParent();
	QString      getString();
	QString      getValidStringAt(int i);
	void         setString(QString &node);
	void         setRow(int i);
	int          getRow();
	int          countChild();
	bool         appendChild(QString &node);
	bool	     insertChild(int pos, int count, QString &node);
	bool         removeChild(int pos, int count);
	bool	     replaceChild(int pos, QString &node);
	void         reverseChildren();
	void         updateRowNumbers();

	void         debugWDomElemState(QString header = "");

	QString      strOldId;
	QString      strOldVal;
private:
	QString             strNode;
	QVector<WPropNode*> childWPropNodes;
	WPropNode         * parentWPropNode;
	int                 rowNum;

	
};

class MyPropertyModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	MyPropertyModel(QObject *parent = 0);
	~MyPropertyModel();

	Qt::ItemFlags flags      ( const QModelIndex &index )                                             const;
	QVariant      data       ( const QModelIndex &index, int role )                                   const;
	QVariant      headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	int           rowCount   ( const QModelIndex &parent = QModelIndex() )                            const;
	int           columnCount( const QModelIndex &parent = QModelIndex() )                            const;
	QModelIndex   index      ( int row, int column, const QModelIndex &parent = QModelIndex() )       const;
	QModelIndex   parent     ( const QModelIndex &index )                                             const;

	bool          setData    (const QModelIndex &index, const QVariant &value, int role);

	bool          removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
	bool          insertRows(int row, int count, const QModelIndex & parent = QModelIndex());

    bool          appendNode(QString &node, const QModelIndex & parent);
    bool          insertNode(int row, QString &node, const QModelIndex & parent);

	QDomElement   getRootElem() { return wRootElement; }
	WPropNode   * getRootNode() { return wRootNode; }

	WPropNode   * getNodeByName (QString &name);
	WPropNode   * getNodeByIndex(QModelIndex index);

	QModelIndex   getIndexByName(QString &name);
	QModelIndex   getIndexByNode(WPropNode * wnode);

	bool		  removeNodeByName(QString &name, const QModelIndex & parent = QModelIndex());
	bool          removeNodeByIndex(QModelIndex index);

	void          reverseChildren(const QModelIndex & parent = QModelIndex());
	void          clearCurrentConfig();

	void          setDomElement(QDomElement &elem);

private:
	WPropNode   * wRootNode;
	WPropNode   * findNodeByName(WPropNode * parent, QString &name);

	QDomElement   wRootElement;

	void          debugTreeState(WPropNode * welem, QString header = "", QString tabs = "");

	bool          isNewIdRepeated(QString &strNewId);
	bool          isNewIdCppKeyword(QString &strNewId);
	bool          recursiveIdCheck(QDomElement * elem, QString &strNewId);

Q_SIGNALS:
	//void          PropertyUpdated(MyPropertyModel *propmodel, )
};

#endif // WQPROPMODEL_H
