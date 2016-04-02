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

#include "dialogabout.h"
#include "ui_dialogabout.h"

#include <QTransform>

DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout),
    m_item(QPixmap(":/logo_icon"))
{
    ui->setupUi(this);

    ui->graphicsView->setScene(&m_scene);
	
	// Set to 200 x 200
	QTransform transform;
	qreal logoWidth = m_item.boundingRect().width();
	qreal logoHeight = m_item.boundingRect().height();
	// logoWidth <-> 1.0
	//    200    <-> X
	// X = 200/logoWidht
	transform.scale(190/logoWidth, 190/logoHeight);
	m_item.setTransform(transform);
    m_scene.addItem(&m_item);
    ui->graphicsView->show();
}

DialogAbout::~DialogAbout()
{
    delete ui;
}

void DialogAbout::on_pushClose_clicked()
{
    close();
}
