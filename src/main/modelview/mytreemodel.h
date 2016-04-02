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

#ifndef WQTREEMODEL_H
#define WQTREEMODEL_H

#include <QAbstractItemModel>
#include <QDomDocument>
#include <QDateTime>
#include <QDebug>
#include <QList>
#include <QIcon>
#include <QFileInfo>

class WDomElem : public QObject
{
public:
	WDomElem(QDomElement &elem, int row, WDomElem *parent = 0);
	~WDomElem();
	WDomElem  * getChild(int i);
	WDomElem  * getParent();
	QDomElement getElem();
	QDomElement getValidElemAt(int i);
	void        setRow(int i);
	int         getRow();
	int         countChild();
	bool        appendChild(QDomElement &element);
	bool        insertChild(int pos, int count, QDomElement &element);
	bool        removeChild(int pos, int count);
	bool        replaceChild(int pos, QDomElement &element);

	void        debugWDomElemState(QString header = "");
private:
	QDomElement        domElem;
	QVector<WDomElem*> childWDomElems;
	WDomElem         * parentWDomElem;
	int                rowNum;

	void        updateRowNumbers();
};

class MyTreeModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	MyTreeModel(QDomDocument document = QDomDocument(), QObject *parent = 0);
	~MyTreeModel();

	Qt::ItemFlags flags      ( const QModelIndex &index )                                             const;
	QVariant      data       ( const QModelIndex &index, int role )                                   const;
	QVariant      headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	int           rowCount   ( const QModelIndex &parent = QModelIndex() )                            const;
	int           columnCount( const QModelIndex &parent = QModelIndex() )                            const;
	QModelIndex   index      ( int row, int column, const QModelIndex &parent = QModelIndex() )       const;
	QModelIndex   parent     ( const QModelIndex &index )                                             const;

	bool          removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
	bool          insertRows(int row, int count, const QModelIndex & parent = QModelIndex());

	bool          appendElem(QDomElement &element = QDomElement(), const QModelIndex & parent = QModelIndex());
	bool          insertElem(int row, QDomElement &element = QDomElement(), const QModelIndex & parent = QModelIndex());

	QDomDocument  getDocument() { return domDocument; }

	WDomElem    * getElemByName (QString &name);
	WDomElem    * getElemByIndex(QModelIndex index);

	QModelIndex   getIndexByName(QString &name);
	QModelIndex   getIndexByItem(WDomElem * welem);

	bool          removeElemByName(QString &name);
	bool          removeElemByIndex(QModelIndex index);

	bool		  appendElemIntoName(QByteArray config, QString &name);
	bool          appendElemIntoIndex(QByteArray config, QModelIndex index);

	void          clearDocument();
	bool          loadNewConfiguration(QByteArray config);

	void          replaceUniqueId(QString oldId, QString newId);

	void          setMapIconsByClassName(QMap<QString, QIcon> mapIcons);

	bool          isHiddenRootElem(WDomElem * elem);

	QString       findCloserContainer(QString &name);

	QList<QFileInfo> getAllTrackedCssFiles();
	bool			 removeTrackedCssFile(QFileInfo fileRef, bool bShowDiag = false);
	bool			 appendTrackedCssFile(QFileInfo fileRef, bool bShowDiag = false);
	bool             isTrackedCssFile(QFileInfo fileRef);

	QList<QFileInfo> getAllTrackedJsFiles();
	bool			 removeTrackedJsFile(QFileInfo fileRef, bool bShowDiag = false);
	bool			 appendTrackedJsFile(QFileInfo fileRef, bool bShowDiag = false);
	bool             isTrackedJsFile(QFileInfo fileRef);

	bool             isInvolvedInConnection(QString strElemId);
	QStringList      getAllSignalSendersIds();
	QStringList      getAllSignalReceiverdIds();

Q_SIGNALS:
	void             removedTrackedCssFile(QFileInfo fileRef);
	void             appendedTrackedCssFile(QFileInfo fileRef);
	void             removedTrackedJsFile(QFileInfo fileRef);
	void             appendedTrackedJsFile(QFileInfo fileRef);

private:
	QDomDocument domDocument;
	WDomElem   * wRootHiddenElem;

	void         getUniqueIdList(QDomElement *elem); 
	void         clearCurrentConfig();
	void         loadDefaultConfig();
	WDomElem   * findItemByName(WDomElem * parent, QString &name);
	WDomElem   * findCloserContainer(WDomElem * child);

	void         debugTreeState(WDomElem * welem, QString header = "", QString tabs = "");

	void		 cleanInvalidElems(QDomElement *elem);
       
	QByteArray   execXQueryOnConfig(QString strQuery);

	QList<QString> mstrlistAllIds;

	QMap<QString, QIcon> m_mapIconByClassName;
};

#endif // WQTREEMODEL_H
