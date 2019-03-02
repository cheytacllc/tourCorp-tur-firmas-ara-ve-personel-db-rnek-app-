#-------------------------------------------------
#
# Project created by QtCreator 2018-12-15T18:52:12
#
#-------------------------------------------------

QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tourCorp
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

```qmake
# QXlsx code for Application Qt project
QXLSX_PARENTPATH=./         # current QXlsx path is . (. means curret directory)
QXLSX_HEADERPATH=./header/  # current QXlsx header path is ./header/
QXLSX_SOURCEPATH=./source/  # current QXlsx source path is ./source/
include(./QXlsx.pri)
```

#Adming Rights and Setting ICON
win32 {
    RC_ICONS = icons/icon.ico
    RC_FILE = tourCorp.rc

}

SOURCES += \
    dialog.cpp \
    main.cpp \
    mainwindow.cpp \
    serverdialog.cpp \
    dbconnection.cpp \
    filterdialog.cpp \
    importexport.cpp

HEADERS += \
    dialog.h \
    mainwindow.h \
    serverdialog.h \
    settings.h \
    dbconnection.h \
    filterdialog.h \
    version.h \
    importexport.h

FORMS += \
        dialog.ui \
    mainwindow.ui \
    dialog.ui \
    mainwindow.ui \
    serverdialog.ui \
    filterdialog.ui \
    importexport.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc

DISTFILES += \
    QXlsx/QXlsx.pri
