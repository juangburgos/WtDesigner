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

#include "mysignalslotsdelegate.h"
#include "mysignalslotsmodel.h"
#include "mixedclasses.h"

#include <QMetaObject>
#include <QMetaEnum>

MySignalSlotsDelegate::MySignalSlotsDelegate(QObject *parent)
: QStyledItemDelegate(parent)
{

}


void GetAllSignalNames(QStringList &strListAllSignals)
{
	QMetaObject metaObj = WtQtInteractWidget::staticMetaObject;
	for (int i = metaObj.enumeratorOffset(); i < metaObj.enumeratorCount(); ++i)
	{
		QMetaEnum metaenum = metaObj.enumerator(i);
		QString strEnumName = QString::fromLatin1(metaenum.name());
		if (strEnumName.compare("Wt_Signals_") == 0)
		{
			for (int j = 0; j < metaenum.keyCount(); ++j)
			{
				if ((WtQtInteractWidget::Wt_Signals_)j != WtQtInteractWidget::Wt_Signals_::Wt_Signals_invalid)
				{
					strListAllSignals.append(QString::fromLatin1(metaenum.valueToKey(j)).remove(strEnumName));
				}
			}
		}
	}
}


void GetAllSlotNames(QStringList &strListAllSlots)
{
	QMetaObject metaObj = WtQtWidget::staticMetaObject;
	for (int i = metaObj.enumeratorOffset(); i < metaObj.enumeratorCount(); ++i)
	{
		QMetaEnum metaenum = metaObj.enumerator(i);
		QString strEnumName = QString::fromLatin1(metaenum.name());
		if (strEnumName.compare("Wt_Slots_") == 0)
		{
			for (int j = 0; j < metaenum.keyCount(); ++j)
			{
				if ((WtQtWidget::Wt_Slots_)j != WtQtWidget::Wt_Slots_::Wt_Slots_invalid)
				{
					strListAllSlots.append(QString::fromLatin1(metaenum.valueToKey(j)).remove(strEnumName));
				}
			}
		}
	}
}

QWidget *MySignalSlotsDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	// declare all variables in advance of switch
	QWidget      * editor = NULL;
	QComboBox    * comboBox;
	QLineEdit    * ledit;
	QStringList    strListAllElemNames;
	QDomElement    rootElem;
	WConnectElem * welem;

	QStringList strListAllSignals;
	QStringList strListAllSlots;

	switch (index.column())
	{
	case 0:
		editor   = new  QComboBox(parent);
		comboBox = static_cast<QComboBox*>(editor);
		comboBox->setFrame(false);
		// fill with possible signals		
		GetAllSignalNames(strListAllSignals);
		comboBox->addItems(strListAllSignals);
		break;

	case 1:
		editor   = new  QComboBox(parent);
		comboBox = static_cast<QComboBox*>(editor);
		comboBox->setFrame(false);
		// fill with possible wwidgets
		welem    = static_cast<WConnectElem*>( index.internalPointer() );
		rootElem = welem->getElem().ownerDocument().documentElement();
		getAllElemNames(rootElem, strListAllElemNames);
		strListAllElemNames.insert(0, "wt_root");
		comboBox->addItems(strListAllElemNames);
		break;

	case 2:
		editor   = new  QComboBox(parent);
		comboBox = static_cast<QComboBox*>(editor);
		comboBox->setFrame(false);
		// fill with possible slots
		GetAllSlotNames(strListAllSlots);
		comboBox->addItems(strListAllSlots);
		break;

	case 3:
		editor = new  QLineEdit(parent);
		ledit  = static_cast<QLineEdit*>(editor);
		ledit->setFrame(false);
		break;

	default:
		break;
	}

	return editor;
}

void MySignalSlotsDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QString strValue = index.model()->data(index, Qt::EditRole).toString();

	QComboBox *comboBox;
	QLineEdit *ledit;

	switch (index.column())
	{
	case 0:
		comboBox = static_cast<QComboBox*>(editor);
		comboBox->setCurrentText(strValue);
		break;
	case 1:
		comboBox = static_cast<QComboBox*>(editor);
		comboBox->setCurrentText(strValue);
		break;
	case 2:
		comboBox = static_cast<QComboBox*>(editor);
		comboBox->setCurrentText(strValue);
		break;
	case 3:
		ledit    = static_cast<QLineEdit*>(editor);
		ledit->setText(strValue);
		break;
	default:
		break;
	}
}

void MySignalSlotsDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

	QComboBox *comboBox;
	QLineEdit *ledit;

	QString strValue;
	switch (index.column())
	{
	case 0:
		comboBox = static_cast<QComboBox*>(editor);
		strValue = comboBox->currentText();
		break;
	case 1:
		comboBox = static_cast<QComboBox*>(editor);
		strValue = comboBox->currentText();
		break;
	case 2:
		comboBox = static_cast<QComboBox*>(editor);
		strValue = comboBox->currentText();
		break;
	case 3:
		ledit = static_cast<QLineEdit*>(editor);
		strValue = ledit->text();
		break;
	default:
		break;
	}

	model->setData(index, strValue, Qt::EditRole);
}

void MySignalSlotsDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)
	editor->setGeometry(option.rect);
}

void MySignalSlotsDelegate::getAllElemNames(QDomElement &parent, QStringList &strListAllElemNames) const
{
	// recursivelly make WObjects name list (by id)
	if (parent.tagName().compare("WObject") == 0)
	{
		strListAllElemNames.append(parent.attribute("Wt_id"));
	}
	for (int i = 0; i < parent.childNodes().count(); i++)
	{
		getAllElemNames(parent.childNodes().at(i).toElement(), strListAllElemNames);
	}
}

