# Devin White
# Xuyen Nguyen
# Taylore Adamson
# Ansam Al Sharif
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Serialization.cpp \
    main.cpp \
    panels/editor.cpp \
    panels/frame.cpp \
    panels/palette.cpp \
    view.cpp

HEADERS += \
    Serialization.h \
    panels/editor.h \
    panels/frame.h \
    panels/palette.h \
    view.h

FORMS += \
    view.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
