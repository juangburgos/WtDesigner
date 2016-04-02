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

#include "dialogconfig.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString str_docroot       = ".";
    QString str_http_address  = "0.0.0.0";
    QString str_http_port     = "8080";
    int     int_phone_width   = 480;
    int     int_tablet_width  = 768;
    int     int_laptop_width  = 992;
    int     int_computer_width= 1200;


    DialogConfig w;

    w.setDocRootRef(&str_docroot);
    w.setHttpAddressRef((&str_http_address));
    w.setHttpPortRef(&str_http_port);

    w.setPhoneWidthRef(&int_phone_width);
    w.setTabletWidthRef(&int_tablet_width);
    w.setLaptopWidthRef(&int_laptop_width);
    w.setComputerWidthRef(&int_computer_width);

    w.setupDialogWidgets();

    w.show();

    return a.exec();
}
