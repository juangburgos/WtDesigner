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

#ifndef MYSIGNALSLOTSDELEGATE_H
#define MYSIGNALSLOTSDELEGATE_H

#include <QStyledItemDelegate>
#include <QComboBox>
#include <QLineEdit>
#include <QtXml/QDomElement>

void GetAllSignalNames(QStringList &strListAllSignals);
void GetAllSlotNames(QStringList &strListAllSlots);

class MySignalSlotsDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    MySignalSlotsDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

	void getAllElemNames(QDomElement &parent, QStringList &strListAllElemNames) const;
};

#endif // MYSIGNALSLOTSDELEGATE_H
