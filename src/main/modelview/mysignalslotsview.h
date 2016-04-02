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

#ifndef MYSIGNALSLOTSVIEW_H
#define MYSIGNALSLOTSVIEW_H

#include <QTreeView>
#include <QPainter>
#include <QColor>
#include <QMouseEvent>

#include "mysignalslotsdelegate.h"

class MySignalsSlotsView : public QTreeView
{
	Q_OBJECT
public:
	explicit MySignalsSlotsView(QWidget * parent = 0);
	~MySignalsSlotsView();

    void drawBranches(QPainter * painter, const QRect & rect, const QModelIndex & index) const;

	void drawRow(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;

	MySignalSlotsDelegate m_mydelegate;

protected:
	void mousePressEvent(QMouseEvent * event);
};

#endif // MyPropertyView_H
