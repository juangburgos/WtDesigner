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

#include "mainwindow.h"

const QString g_strHighlightClassName = "wtc_highlight";

MyWebView::MyWebView(QWidget *parent) : QWebView(parent)
{
	m_strCurrentElemId = "";
	installEventFilter(this);
	setAcceptDrops(true);
	QObject::connect( this, SIGNAL(loadStarted())     , this, SLOT(on_loadStarted())      );
	QObject::connect( this, SIGNAL(loadFinished(bool)), this, SLOT(on_loadFinished(bool)) );
	// set qt style to webview
	setStyleSheet("background-color: lightgray;");
	// set css style for highlighting element
	QString     strStyle = "border-style  : solid    !important; "
						   "border-width  : 2px      !important; "
						   "border-color  : #f0f090  !important; ";
	SetHighlightStyleClass(g_strHighlightClassName, strStyle);
}

bool MyWebView::eventFilter(QObject *watched, QEvent *event)
{
	if (!m_boolEnableEvtProcess) { return false; }

	if (event->type() == QEvent::MouseMove)
	{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
		QWebView *view          = dynamic_cast<QWebView*>(watched);
		QPoint pos              = view->mapFromGlobal(mouseEvent->globalPos());
		QWebFrame *frame        = view->page()->frameAt(mouseEvent->pos());
		if (frame!=NULL)
		{
			QWebHitTestResult hitTestResult = frame->hitTestContent(pos);
			// if the hovered elem is not the same as the previously hovered
			if (hitTestResult.element() != m_old_hover_element)
			{
				QWebElement elemCurrent = hitTestResult.element();
				// if dragging, overwrite with closer WContainer
				if (m_boolIsDraggingWidget)
				{
					elemCurrent = FindCloserContainer(elemCurrent);
				}
				if (!elemCurrent.isNull())
				{
					// Change color
					ChangeHoveredElemColor(elemCurrent);
					// Message to print
					m_strCurrentElemId = elemCurrent.attribute("id");
					// Emit message
					QStringList strlistClasses = elemCurrent.classes();
					strlistClasses.removeAll(g_strHighlightClassName);
					Q_EMIT mouseMovedOverElem(m_strCurrentElemId, strlistClasses.join(' '));
				}
			}
		}
	}
	else if (event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
		if (mouseEvent->button() == Qt::LeftButton)
		{
			QWebView *view = dynamic_cast<QWebView*>(watched);
			QPoint pos = view->mapFromGlobal(mouseEvent->globalPos());
			QWebFrame *frame = view->page()->frameAt(mouseEvent->pos());
			if (frame != NULL)
			{
				QWebHitTestResult hitTestResult = frame->hitTestContent(pos);
				QWebElement elemCurrent = FindCloserWidget(hitTestResult.element());
				// if the clicked elem is not the same as the previously clicked
				if (elemCurrent != m_old_click_element)
				{
					// Change color
					ChangeClickedElemColor(elemCurrent);
					// Message to print
					m_strCurrentElemId = elemCurrent.attribute("id");
					// Emit message
					Q_EMIT mouseLeftClickedOverElem(m_strCurrentElemId);
				}
			}
		}
	}
	return false;
}

QString MyWebView::GetCloserContainerId()
{
	QWebElement elemCurrent = FindCloserContainer(m_old_click_element);
	return elemCurrent.attribute("id");
}

void MyWebView::ChangeHoveredElemColor(QWebElement &hitTestResult)
{
	if (!m_boolEnableEvtProcess) { return; }
	// approach of creating class in user css for qwebview, and then toggle the class

	// Restore old element
	m_old_hover_element.removeClass(g_strHighlightClassName);
	// Store new as old
	m_old_hover_element = hitTestResult;
	// Highlight
	m_old_hover_element.addClass(g_strHighlightClassName);
}

void MyWebView::ChangeClickedElemColor(QWebElement &hitTestResult)
{
	if (!m_boolEnableEvtProcess) { return; }

	// Restore old element
	m_old_click_element.setStyleProperty("border-style", m_old_click_borderstyle);
	m_old_click_element.setStyleProperty("border-width", m_old_click_borderwidth);
	m_old_click_element.setStyleProperty("border-color", m_old_click_bordercolor);
	// Store new as old
	m_old_click_element = hitTestResult;
	// Store old properties to be able to restore later
	m_old_click_borderstyle = "";
	m_old_click_borderwidth = "";
	m_old_click_bordercolor = "";
	// Highlight
	m_old_click_element.setStyleProperty("border-style", "solid"  );
	m_old_click_element.setStyleProperty("border-width", "3px"    );
	m_old_click_element.setStyleProperty("border-color", "#FF0000"); // red

}

void MyWebView::on_HighlightTreeSelectedElem(QString name)
{
	if (!m_boolEnableEvtProcess) { return; }

	QWebElement webelem = FindElementByName(page()->mainFrame()->documentElement(), name);
	
	if (webelem.isNull())
	{
		qDebug() << "[ERROR] Invalid QWebElement in MyWebView::on_HighlightTreeSelectedElem.";
		return;
	}
	ChangeClickedElemColor(webelem);
}

QWebElement MyWebView::FindElementByName(QWebElement &elem, QString &name)
{
	if (elem.attribute("id").compare(name, Qt::CaseInsensitive) == 0)
	{
		return elem;
	}
	else
	{
		QWebElement childelem = elem.firstChild();
		QWebElement retelem;
		while (!childelem.isNull())
		{
			retelem = FindElementByName(childelem, name);
			if (!retelem.isNull())
			{
				return retelem;
			}
			childelem = childelem.nextSibling();
		}
	}
	return QWebElement();
}

void MyWebView::dragEnterEvent(QDragEnterEvent *event)
{
	if (!m_boolEnableEvtProcess) { return; }

	m_boolIsDraggingWidget = true;

	MyWidgetView *source = qobject_cast<MyWidgetView *>(event->source());

	if (source)
	{
		event->setDropAction(Qt::CopyAction);
		event->accept();
	}

}

void MyWebView::dragMoveEvent(QDragMoveEvent *event)
{
	if (!m_boolEnableEvtProcess) { return; }

	MyWidgetView *source = qobject_cast<MyWidgetView *>(event->source());

	if (source)
	{
		event->setDropAction(Qt::CopyAction);
		event->accept();
	}

	QMouseEvent mouseEvt(QEvent::MouseMove, QPointF(event->pos()), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
	eventFilter(this, &mouseEvt);
}

void MyWebView::dropEvent(QDropEvent *event)
{
	if (!m_boolEnableEvtProcess) { return; }

	m_boolIsDraggingWidget = false;

	MyWidgetView *source = qobject_cast<MyWidgetView *>(event->source());

	if (source)
	{
		// TODO : Code here or in other event to reject drag if hovered item does not inherit from WContainerWidget
		Q_EMIT receivedDragOnWebview(event->mimeData()->data(g_strWtwMime), m_strCurrentElemId);
		event->setDropAction(Qt::CopyAction);
		event->accept();
	}
}

void MyWebView::leaveEvent(QEvent * event)
{
	if (!m_boolEnableEvtProcess) { return; }

	// Restore old element
	m_old_hover_element.removeClass(g_strHighlightClassName);
}

void MyWebView::on_loadStarted()
{
	m_boolEnableEvtProcess = false;

	m_old_hover_element.removeClass(g_strHighlightClassName);

	m_strCurrentElemId = "";

	m_old_click_element.setStyleProperty("border-style", m_old_click_borderstyle);
	m_old_click_element.setStyleProperty("border-width", m_old_click_borderwidth);
	m_old_click_element.setStyleProperty("border-color", m_old_click_bordercolor);

	m_old_click_element = QWebElement();
	m_old_click_borderstyle = "";
	m_old_click_borderwidth = "";
	m_old_click_bordercolor = "";
}

void MyWebView::on_loadFinished(bool ok)
{
	m_boolEnableEvtProcess = true;
	QObject    *p_parent  = parent();
	MainWindow *p_mwindow = qobject_cast<MainWindow*>(p_parent);
	while (p_mwindow == NULL)
	{
		p_parent  = p_parent->parent();
		p_mwindow = qobject_cast<MainWindow*>(p_parent);
		if (p_mwindow)
		{
			m_pMainWindow = p_mwindow;
			Q_EMIT m_pMainWindow->RequestAllProperties("");
		}
	}
}

void MyWebView::SetHighlightStyleClass(QString strClassName, QString &strTmpStyle)
{
	QByteArray  strComposedStyle("." + strClassName.toUtf8() + " { " + strTmpStyle.toUtf8()  + " }");

	QWebSettings *settings = QWebSettings::globalSettings();
	settings->setUserStyleSheetUrl(QUrl("data:text/css;charset=utf-8;base64," + strComposedStyle.toBase64()));

}

QWebElement MyWebView::FindCloserContainer(QWebElement &elem)
{
	// find by elem id
	bool       bIsContainer = false;
	WDomElem * welem = m_pMainWindow->m_treemodel.getElemByName (elem.attribute("id"));
	// check if valid parent
	if (welem)
	{
		QDomElement delem = welem->getElem();
		if (delem.attribute(g_strClassAttr).compare("WContainerWidget") == 0 ||
			delem.attribute(g_strClassAttr).compare("WAnchor"         ) == 0 ||
			delem.attribute(g_strClassAttr).compare("WGroupBox"       ) == 0 ||
			delem.attribute(g_strClassAttr).compare("WPanel"          ) == 0 ||
			delem.attribute(g_strClassAttr).compare("WMenuItem"       ) == 0 ||
			delem.attribute(g_strClassAttr).compare("WStackedWidget"  ) == 0 ||
			delem.attribute(g_strClassAttr).compare("WTabItem"        ) == 0)
		{
			bIsContainer = true;
		}
	}
	// if container return, else keep looking
	if (bIsContainer)
	{
		return elem;
	}
	else
	{
		if (!elem.parent().isNull())
		{
			return FindCloserContainer(elem.parent());
		}
		else
		{
			return QWebElement();
		}
	}
}

QWebElement MyWebView::FindCloserWidget(QWebElement &elem)
{
	WDomElem * welem = m_pMainWindow->m_treemodel.getElemByName(elem.attribute("id"));
	// check if valid parent
	if (welem)
	{
		return elem;
	}
	else
	{
		return FindCloserWidget(elem.parent());
	}
}


