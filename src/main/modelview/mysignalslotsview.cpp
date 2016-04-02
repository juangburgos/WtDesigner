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

#include "mysignalslotsview.h"
#include "mysignalslotsmodel.h"
#include <QDebug>

MySignalsSlotsView::MySignalsSlotsView(QWidget *parent) : QTreeView(parent)
{
	this->setIndentation(0);
	this->setItemDelegate(&m_mydelegate);
}

MySignalsSlotsView::~MySignalsSlotsView()
{

}

void MySignalsSlotsView::drawBranches(QPainter *painter, const QRect &rect, const QModelIndex &index) const
{
    QTreeView::drawBranches(painter, rect, index);
}

void MySignalsSlotsView::drawRow(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	QTreeView::drawRow(painter, option, index);
}

void MySignalsSlotsView::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {
        QModelIndex index = indexAt(event->pos());
        if (index.column() >= 0 && index.column() <= 4) 
		{ // column you want to use for one click
            edit(index);
			this->setCurrentIndex(index);
        }
    }
    QTreeView::mousePressEvent(event);
}

