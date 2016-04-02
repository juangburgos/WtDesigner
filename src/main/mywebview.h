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

#ifndef MYWEBVIEW_H
#define MYWEBVIEW_H

#include <QtWebKitWidgets>
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QApplication>
#include <QMimeData>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDropEvent>

class MainWindow;

class MyWebView : public QWebView
{
	Q_OBJECT
public:
	explicit MyWebView(QWidget * parent = 0);

	bool        eventFilter(QObject * watched, QEvent * event);
	void		ChangeHoveredElemColor(QWebElement &hitTestResult);
	void        ChangeClickedElemColor(QWebElement &hitTestResult);
	void        SetHighlightStyleClass(QString strClassName, QString &strTmpStyle);

	QWebElement FindCloserContainer(QWebElement &elem);
	QWebElement FindCloserWidget(QWebElement &elem);
	QWebElement FindElementByName(QWebElement &elem, QString &name);

	QString     GetCloserContainerId();

protected:
	void leaveEvent(QEvent * event);
	// RECEIVE DRAG------------------------
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dropEvent(QDropEvent *event);

Q_SIGNALS:
	void mouseMovedOverElem(QString elem, QString strClass);
	void mouseLeftClickedOverElem(QString elem);
	void receivedDragOnWebview(QByteArray baconfigChunk, QString strParentElemId);

public Q_SLOTS:
	void on_HighlightTreeSelectedElem(QString name);

	void on_loadStarted();
	void on_loadFinished(bool ok);

private:
	QString            m_strCurrentElemId;
	QWebElement        m_old_hover_element;

	// TODO : change to css class aproach
	QWebElement        m_old_click_element;
	QString            m_old_click_borderstyle = "";
	QString            m_old_click_borderwidth = "";
	QString            m_old_click_bordercolor = "";

	bool               m_boolEnableEvtProcess  = false;
	bool               m_boolIsDraggingWidget  = false;

	MainWindow       * m_pMainWindow = NULL;

};

#endif // MYWEBVIEW_H
