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

#ifndef WQWIDGETMODEL_H
#define WQWIDGETMODEL_H

#include <QAbstractItemModel>
#include <QDomDocument>
#include <QDateTime>
#include <QDebug>
#include <QColor>

#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QIcon>

//#include <algorithm>

enum WWIDGETNODETYPE
{
	ROOT,
	CATEGORY,
	WIDGET
};

class WWidgetNode : public QObject
{
	Q_OBJECT
public:
	WWidgetNode(WWIDGETNODETYPE intNodeType, QString &strPath, int row, WWidgetNode *parent = 0);
	~WWidgetNode();
	WWidgetNode  * getChild(int i);
	WWidgetNode  * getParent();

	void           setRow(int i);
	int            getRow();
	int            countChild();


	WWIDGETNODETYPE getNodeType();
	void            setNodeType(WWIDGETNODETYPE &intNodeType);

	QString        getName();
	QIcon          getIcon();

    QByteArray     getConfig();
				   
	void           debugPrintState(QString header = "");

private:		   
	WWIDGETNODETYPE         m_intNodeType; // could be "root", "category", "widget"
	QString                 m_strPath;
	QString                 m_strName;
	QByteArray              m_byteConfig;
	QIcon                   m_icon;
	QVector<WWidgetNode*>   childWWidgetNodes;
	WWidgetNode           * parentWWidgetNode;
	int                     rowNum;

	void updateRowNumbers();
	void setupNodeChildren();

};

class MyWidgetModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	MyWidgetModel(QString strPath, QObject *parent = 0);
	~MyWidgetModel();

	Qt::ItemFlags flags      ( const QModelIndex &index )                                             const;
	QVariant      data       ( const QModelIndex &index, int role )                                   const;
	QVariant      headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	int           rowCount   ( const QModelIndex &parent = QModelIndex() )                            const;
	int           columnCount( const QModelIndex &parent = QModelIndex() )                            const;
	QModelIndex   index      ( int row, int column, const QModelIndex &parent = QModelIndex() )       const;
	QModelIndex   parent     ( const QModelIndex &index )                                             const;

	QByteArray    getWidgetConfigByName(QString strWidgetName);

private:
	WWidgetNode   * wRootNode;

	void          debugTreeState(WWidgetNode * welem, QString header = "", QString tabs = "");
	QByteArray    findWidgetConfigRecursive(WWidgetNode * wparent, QString &strWidgetName);

};

#endif // WQPROPMODEL_H
