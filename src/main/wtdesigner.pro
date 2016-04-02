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

# For compiling with Qt Creator
# TEMPLATE     = app
# For compiling with Visual Studio
TEMPLATE      = vcapp

win32 {
RC_ICONS      = ./../../rec/WtDesigner_logo.ico
}


TARGET        = WtDesigner

CONFIG       -= flat
CONFIG       += no_batch

QT           += core xml widgets gui webkitwidgets svg xmlpatterns

DEFINES      += QT_DLL QT_WEBKITWIDGETS_LIB QT_WIDGETS_LIB QT_XML_LIB 

DESTDIR       = ../../bin/WtDesigner/

INCLUDEPATH  += ./wtwithqtlib          

include(../wtconfig.pri)

HEADERS      += ./myglobals.h                                   \			
                ./mainpage.h                                    \
                ./mainwindow.h                                  \
                ./mixedclasses.h                                \
                ./myundocommands.h                              \                
                ./mywebview.h                                   \
                ./wtserverworker.h                              \
                ./helperfunctions.h                             \
                ./wtwithqtlib/dispatchthread.h                  \
                ./wtwithqtlib/wqapplication.h                   \
                ./modelview/mytreeview.h                        \
		./modelview/mytreemodel.h                       \
		./modelview/mypropertymodel.h                   \
		./modelview/mypropertyview.h                    \
		./modelview/mypropertydelegate.h                \
		./modelview/mywidgetmodel.h                     \
		./modelview/mywidgetview.h                      \
		./modelview/mysignalslotsmodel.h                \
		./modelview/mysignalslotsview.h                 \                      
                ./modelview/mysignalslotsdelegate.h             \
                ./modelview/myfilesystemview.h                  \
		./dialogs/dialogconfig/dialogconfig.h           \
		./dialogs/dialogprojselect/dialogprojselect.h   \ 	
		./dialogs/dialogprojsave/dialogprojsave.h       \ 
                ./dialogs/dialogabout/dialogabout.h             \ 			
               
SOURCES      += ./main.cpp                                      \
                ./mainpage.cpp                                  \
                ./mainwindow.cpp                                \
		./mixedclasses.cpp                              \
		./myundocommands.cpp                            \				
                ./mywebview.cpp                                 \               
                ./wtserverworker.cpp                            \
                ./helperfunctions.cpp                           \
                ./wtwithqtlib/dispatchthread.cpp                \
                ./wtwithqtlib/wqapplication.cpp                 \
                ./modelview/mytreeview.cpp                      \
                ./modelview/mytreemodel.cpp                     \
		./modelview/mypropertymodel.cpp                 \
		./modelview/mypropertyview.cpp                  \
		./modelview/mypropertydelegate.cpp              \
		./modelview/mywidgetmodel.cpp                   \
		./modelview/mywidgetview.cpp                    \
		./modelview/mysignalslotsmodel.cpp              \
		./modelview/mysignalslotsview.cpp               \       
                ./modelview/mysignalslotsdelegate.cpp           \
                ./modelview/myfilesystemview.cpp                \   
		./dialogs/dialogconfig/dialogconfig.cpp         \
		./dialogs/dialogprojselect/dialogprojselect.cpp \
		./dialogs/dialogprojsave/dialogprojsave.cpp     \
                ./dialogs/dialogabout/dialogabout.cpp           \       
             
FORMS        += ./mainwindow.ui                                 \
                ./dialogs/dialogconfig/dialogconfig.ui          \
                ./dialogs/dialogprojselect/dialogprojselect.ui  \
                ./dialogs/dialogprojsave/dialogprojsave.ui      \
                ./dialogs/dialogabout/dialogabout.ui            \


RESOURCES     = ./../../rec/qtresources.qrc
RESOURCES    += ./../../rec/styles/qdarkstyle/style.qrc