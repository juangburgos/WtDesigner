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

#include "mytreeview.h"
#include <QDebug>
#include <QKeyEvent>

MyTreeView::MyTreeView(QWidget *parent) : QTreeView(parent)
{
	this->setContextMenuPolicy(Qt::CustomContextMenu);
}

MyTreeView::~MyTreeView()
{

}

void MyTreeView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Backspace)
    {
        Q_EMIT backspacePressed();
    }
}

void MyTreeView::drawRow(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	QTreeView::drawRow(painter, option, index);
}

