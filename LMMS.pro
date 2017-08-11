#-------------------------------------------------
#
# Project created by QtCreator 2017-08-08T07:29:06
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LMMS
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        panel.cpp \
    base/cbaseeditdlg.cpp \
    base/cdatabasepackage.cpp \
    base/cdirecteditdlg.cpp \
    base/cspindelegate.cpp \
    cdetaildlg.cpp \
    cstoragedlg.cpp \
    base/ccombodelegate.cpp \
    base/cdoublespindelegate.cpp \
    base/clineeditdelegate.cpp \
    cprescribedlg.cpp \
    cmaintaindlg.cpp

HEADERS  += panel.h \
    base/cbaseeditdlg.h \
    base/cdatabasepackage.h \
    base/cdirecteditdlg.h \
    base/cspindelegate.h \
    cdetaildlg.h \
    cmaintaindlg.h \
    base/ccombodelegate.h \
    base/cdoublespindelegate.h \
    base/clineeditdelegate.h \
    cstoragedlg.h \
    cprescribedlg.h

FORMS    += panel.ui
