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

#include "mywidgetview.h"
#include "mywidgetmodel.h"
#include <QDebug>

MyWidgetView::MyWidgetView(QWidget *parent) : QTreeView(parent)
{
	this->setHeaderHidden(true);
}

MyWidgetView::~MyWidgetView()
{

}

void MyWidgetView::drawBranches(QPainter *painter, const QRect &rect, const QModelIndex &index) const
{
    QTreeView::drawBranches(painter, rect, index);
}

void MyWidgetView::drawRow(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	QTreeView::drawRow(painter, option, index);
}

void MyWidgetView::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
    {
        QModelIndex index = indexAt(event->pos());
        if (index.column() == 1) { // column you want to use for one click
            edit(index);
        }
    }

    // SEND ------------------------
    if (event->button() == Qt::LeftButton)
    {
        m_startPos = event->pos();
    }
    QTreeView::mousePressEvent(event);
}

void MyWidgetView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        int distance = (event->pos() - m_startPos).manhattanLength();
        if (distance >= QApplication::startDragDistance())
        {
            startDrag();
        }
    }
    QTreeView::mouseMoveEvent(event);
}

void MyWidgetView::startDrag()
{
    WWidgetNode *item = static_cast<WWidgetNode*>(currentIndex().internalPointer());
    if (item)
    {
        // set config to mime data
        QMimeData *mimeData = new QMimeData;
		mimeData->setData(g_strWtwMime,item->getConfig());
        // set icon to drag
        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(item->getIcon().pixmap(60,60));
        // execute drag (blocking)       
		drag->exec(Qt::CopyAction); 
    }
}

