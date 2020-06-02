######################################################################
# Automatically generated by qmake (3.1) Tue Jun 2 10:10:27 2020
######################################################################

TEMPLATE = app
TARGET = src
INCLUDEPATH += . /usr/local/include

# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += main/helperfunctions.h \
           main/mainpage.h \
           main/mainwindow.h \
           main/mixedclasses.h \
           main/myglobals.h \
           main/myundocommands.h \
           main/mywebview.h \
           main/cmake/template_project.h \
           main/modelview/myfilesystemview.h \
           main/modelview/mypropertydelegate.h \
           main/modelview/mypropertymodel.h \
           main/modelview/mypropertyview.h \
           main/modelview/mysignalslotsdelegate.h \
           main/modelview/mysignalslotsmodel.h \
           main/modelview/mysignalslotsview.h \
           main/modelview/mytreemodel.h \
           main/modelview/mytreeview.h \
           main/modelview/mywidgetmodel.h \
           main/modelview/mywidgetview.h \
           main/wtwithqtlib/DispatchThread.h \
           main/wtwithqtlib/WQApplication.h \
           main/dialogs/dialogabout/dialogabout.h \
           main/dialogs/dialogconfig/dialogconfig.h \
           main/dialogs/dialogprojsave/dialogprojsave.h \
           main/dialogs/dialogprojselect/dialogprojselect.h
FORMS += main/mainwindow.ui \
         main/dialogs/dialogabout/dialogabout.ui \
         main/dialogs/dialogconfig/dialogconfig.ui \
         main/dialogs/dialogprojsave/dialogprojsave.ui \
         main/dialogs/dialogprojselect/dialogprojselect.ui
SOURCES += main/helperfunctions.cpp \
           main/main.cpp \
           main/mainpage.cpp \
           main/mainwindow.cpp \
           main/mixedclasses.cpp \
           main/myglobals.cpp \
           main/myundocommands.cpp \
           main/mywebview.cpp \
           main/cmake/template_main.cpp \
           main/cmake/template_project.cpp \
           main/modelview/myfilesystemview.cpp \
           main/modelview/mypropertydelegate.cpp \
           main/modelview/mypropertymodel.cpp \
           main/modelview/mypropertyview.cpp \
           main/modelview/mysignalslotsdelegate.cpp \
           main/modelview/mysignalslotsmodel.cpp \
           main/modelview/mysignalslotsview.cpp \
           main/modelview/mytreemodel.cpp \
           main/modelview/mytreeview.cpp \
           main/modelview/mywidgetmodel.cpp \
           main/modelview/mywidgetview.cpp \
           main/wtwithqtlib/DispatchThread.cpp \
           main/wtwithqtlib/WQApplication.cpp \
           main/dialogs/dialogabout/dialogabout.cpp \
           main/dialogs/dialogabout/main.cpp \
           main/dialogs/dialogconfig/diagconfigmain.cpp \
           main/dialogs/dialogconfig/dialogconfig.cpp \
           main/dialogs/dialogprojsave/diagprojsavemain.cpp \
           main/dialogs/dialogprojsave/dialogprojsave.cpp \
           main/dialogs/dialogprojselect/diagprojselmain.cpp \
           main/dialogs/dialogprojselect/dialogprojselect.cpp