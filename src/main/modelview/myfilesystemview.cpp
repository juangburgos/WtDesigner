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

#include "myfilesystemview.h"
#include <QDebug>
#include <QKeyEvent>
#include <QFileSystemModel>
#include <QApplication>

MyFileSystemView::MyFileSystemView(QWidget *parent) : QTreeView(parent)
{

}

void MyFileSystemView::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_startPos = event->pos();
	}
	QTreeView::mousePressEvent(event);
}

void MyFileSystemView::mouseMoveEvent(QMouseEvent *event)
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

void MyFileSystemView::startDrag()
{
	QModelIndex index = currentIndex();
	if (index.isValid())
	{
		QFileInfo myFileInfo = qobject_cast<QFileSystemModel*>(this->model())->fileInfo(index);
		if (!myFileInfo.suffix().contains("css", Qt::CaseInsensitive) && !myFileInfo.suffix().contains("js", Qt::CaseInsensitive))
		{
			m_boolIsWatched = false;
			return;
		}
		// construct relative source path
		QDir myDir("./");
		m_strRelativeSourceFilePath = myDir.relativeFilePath(myFileInfo.absoluteFilePath());
		m_boolIsWatched = true;
	}
}

void MyFileSystemView::dropEvent(QDropEvent *event)
{
	QTreeView::dropEvent(event);
	if (!m_boolIsWatched)
	{
		return;
	}
	auto mypos = event->pos();
	auto mysrc = event->source();
	if (qobject_cast<MyFileSystemView*>(mysrc))
	{
		QModelIndex index = this->indexAt(mypos); // show only the target folder
		if (index.isValid())
		{
			QFileInfo destFileInfo = qobject_cast<QFileSystemModel*>(this->model())->fileInfo(index);
			QString strAbsoluteDestinationDir;
			if (destFileInfo.isFile())
			{
				strAbsoluteDestinationDir = destFileInfo.absolutePath();
			}
			else
			{
				strAbsoluteDestinationDir = destFileInfo.absoluteFilePath();
			}
			QFileInfo sourceFileInfo(m_strRelativeSourceFilePath);
			QString strAbsoluteDestinationFilePath = strAbsoluteDestinationDir + "/" + sourceFileInfo.fileName();
			// construct relative destination path
			QDir myDir("./");
			m_strRelativeDestinationFilePath = myDir.relativeFilePath(strAbsoluteDestinationFilePath);
			// send signal
			if (m_strRelativeSourceFilePath.compare(m_strRelativeDestinationFilePath) != 0 && QFileInfo(m_strRelativeDestinationFilePath).exists())
			{
				Q_EMIT trackedFileWasMoved(m_strRelativeSourceFilePath, m_strRelativeDestinationFilePath);
			}
			// clean
			m_strRelativeSourceFilePath.clear();
			m_strRelativeDestinationFilePath.clear();
			m_boolIsWatched = false;
		}
	}
}
