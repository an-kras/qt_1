QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    btn.cpp \
    clcltr.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    btn.h \
    clcltr.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
# install
 target.path = $$[QT_INSTALL_EXAMPLES]/widgets/calculator
 sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS calculator.pro
 sources.path = $$[QT_INSTALL_EXAMPLES]/widgets/calculator
 INSTALLS += target sources

 symbian {
     TARGET.UID3 = 0xA000C602
     include($$QT_SOURCE_TREE/examples/symbianpkgrules.pri)
 }
