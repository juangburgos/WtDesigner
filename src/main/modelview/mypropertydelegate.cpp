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

#include "mypropertydelegate.h"

#include <QItemDelegate>
#include <QInputDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QRect>
#include <QDebug>

MyPropertyDelegate::MyPropertyDelegate(QObject *parent)
: QStyledItemDelegate(parent)
{

}


QWidget *MyPropertyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	// declare all variables in advance of switch
	QWidget      * editor = NULL;

	// get field name
	QString * p_strFieldName = static_cast<QString*>(index.internalPointer());

	if (!p_strFieldName)
	{
		qDebug() << "[ERROR] Invalid QString in MyPropertyDelegate::createEditor";
		return new QLineEdit(parent);
	}

	if (p_strFieldName->compare("cod_site") == 0 ||
		p_strFieldName->compare("cod_area") == 0 ||
		p_strFieldName->compare("cod_wu")   == 0 )
	{
		editor = new QInputDialog(parent);
	}
	else
	{
		editor = new QLineEdit(parent);
	}

	return editor;
}

void MyPropertyDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QString strValue = index.model()->data(index, Qt::EditRole).toString();

	// get field name
	QString * p_strFieldName = static_cast<QString*>(index.internalPointer());

	if (!p_strFieldName)
	{
		qDebug() << "[ERROR] Invalid QString in MyPropertyDelegate::createEditor";
		return;
	}

	if (p_strFieldName->compare("cod_site") == 0)
	{
		QInputDialog * pdialog = qobject_cast<QInputDialog*>(editor);
		QStringList srtlist;
		srtlist.move(srtlist.indexOf(strValue), 0);
		pdialog->setComboBoxItems(srtlist);
	}
	else if (p_strFieldName->compare("cod_area") == 0)
	{
		QInputDialog * pdialog = qobject_cast<QInputDialog*>(editor);
		QStringList srtlist;
		srtlist.move(srtlist.indexOf(strValue), 0);
		pdialog->setComboBoxItems(srtlist);
	}
	else if (p_strFieldName->compare("cod_wu") == 0)
	{
		QInputDialog * pdialog = qobject_cast<QInputDialog*>(editor);
		QStringList srtlist;
		srtlist.move(srtlist.indexOf(strValue), 0);
		pdialog->setComboBoxItems(srtlist);
	}
	else 
	{
		QLineEdit * pedit = qobject_cast<QLineEdit*>(editor);
		pedit->setText(strValue);
	}

}

void MyPropertyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QString strValue;

	// get field name
	QString * p_strFieldName = static_cast<QString*>(index.internalPointer());

	if (!p_strFieldName)
	{
		qDebug() << "[ERROR] Invalid QString in MyPropertyDelegate::createEditor";
		return;
	}

	if (p_strFieldName->compare("cod_site") == 0 ||
		p_strFieldName->compare("cod_area") == 0 ||
		p_strFieldName->compare("cod_wu") == 0)
	{
		QInputDialog * pdialog = qobject_cast<QInputDialog*>(editor);
		strValue = pdialog->textValue();
	}
	else
	{
		QLineEdit * pedit = qobject_cast<QLineEdit*>(editor);
		strValue = pedit->text();
	}

	model->setData(index, strValue, Qt::EditRole);
}

void MyPropertyDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

	// get field name
	QString * p_strFieldName = static_cast<QString*>(index.internalPointer());

	if (!p_strFieldName)
	{
		qDebug() << "[ERROR] Invalid QString in MyPropertyDelegate::createEditor";
		return;
	}

	if (p_strFieldName->compare("cod_site") == 0 ||
		p_strFieldName->compare("cod_area") == 0 ||
		p_strFieldName->compare("cod_wu") == 0)
	{
		QRect scr = QApplication::desktop()->screenGeometry();
		editor->move(scr.center());
	}
	else
	{
		editor->setGeometry(option.rect);
	}
	
}

