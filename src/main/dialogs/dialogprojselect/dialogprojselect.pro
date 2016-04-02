# Copyright (c) 2016 Juan Gonzalez Burgos
# 
# This file is part of WtDesigner.
# 
# WtDesigner is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# WtDesigner is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with WtDesigner.  If not, see <http://www.gnu.org/licenses/>.

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET    = dialogprojselect
TEMPLATE  = app
# TEMPLATE  = vcapp

DEFINES  += WTC_TESTING


SOURCES  += diagprojselmain.cpp  \
            dialogprojselect.cpp

HEADERS  += dialogprojselect.h

FORMS    += dialogprojselect.ui

RESOURCES  = ./../../../../rec/qtresources.qrc