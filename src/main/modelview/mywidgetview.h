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

#ifndef MYWIDGETVIEW_H
#define MYWIDGETVIEW_H

#include <QTreeView>
#include <QPainter>
#include <QColor>
#include <QMouseEvent>
#include <QApplication>
#include <QMimeData>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDropEvent>

const QString g_strWtwMime = "wtw";

class MyWidgetView : public QTreeView
{
	Q_OBJECT
public:
	explicit MyWidgetView(QWidget * parent = 0);
	~MyWidgetView();

    void drawBranches(QPainter * painter, const QRect & rect, const QModelIndex & index) const;

	void drawRow(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;

protected:
    // SEND ------------------------
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void startDrag();
    QPoint m_startPos;
};

#endif // MyWidgetView_H
