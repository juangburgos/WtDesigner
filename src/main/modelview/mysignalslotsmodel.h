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

#ifndef WSIGSLOTMODEL_H
#define WSIGSLOTMODEL_H

#include <QAbstractItemModel>
#include <QDomDocument>
#include <QDateTime>
#include <QDebug>
#include <QColor>

class WConnectElem
{
public:
	WConnectElem(QDomElement &elem, int row, WConnectElem *parent = 0);
	~WConnectElem();
	WConnectElem  * getChild(int i);
	WConnectElem  * getParent();
	QDomElement     getElem();
	QDomElement     getValidElemAt(int i);
	void            setRow(int i);
	int             getRow();
	int             countChild();
	bool            appendChild(QDomElement &element);
	bool            insertChild(int pos, int count, QDomElement &element);
	bool            removeChild(int pos, int count);
	bool            replaceChild(int pos, QDomElement &element);
				    
	void            debugWDomElemState(QString header = "");
private:
	QDomElement            domElem;
	QVector<WConnectElem*> childWDomElems;
	WConnectElem         * parentWDomElem;
	int                    rowNum;

	void            updateRowNumbers();
};

class MySignalSlotModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	MySignalSlotModel(QDomElement elem = QDomElement(), QObject *parent = 0);
	~MySignalSlotModel();

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

	bool          appendElem(QDomElement &element = QDomElement(), const QModelIndex & parent = QModelIndex());
	bool          insertElem(int row, QDomElement &element = QDomElement(), const QModelIndex & parent = QModelIndex());

	WConnectElem    * getRootItem() { return wRootElem; }

	WConnectElem    * getElemByIndex(QModelIndex index);

	QModelIndex   getIndexByItem(WConnectElem * welem);

	bool          removeElemByIndex(QModelIndex index);

    void          clearCurrentConfig();
    bool          loadNewConfiguration(QDomElement elem);

private:
	WConnectElem   * wRootElem;

	WConnectElem   * findItemByName(WConnectElem * parent, QString &name);

	void         debugTreeState(WConnectElem * welem, QString header = "", QString tabs = "");

	void		 cleanInvalidElems(QDomElement *elem);

Q_SIGNALS:
	void DisconnectOldSignal(QString strSender, QString strSignal, QString strReceiver, QString strSlot, QString strParameter );
};

#endif // WQTREEMODEL_H
