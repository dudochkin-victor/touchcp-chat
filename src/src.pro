include(../common.pri)
include($$[QT_INSTALL_DATA]/mkspecs/dcpconfig.pri)
TEMPLATE = lib
CONFIG += qt \
    meegotouch \
    duicontrolpanel \
    plugin \
    link_pkgconfig \
    mobility \
    debug

PKGCONFIG += meegotouch libmeegochat TelepathyQt4

QT += dbus mobility
MOBILITY += contacts
target.path = $$DCP_APPLET_DIR


STYLE_HEADERS += tpacctlistitemstyle.h

HEADERS += $$STYLE_HEADERS \ 
    tpacctsetupwidget.h \
    tpacctsettingsdefaultdefines.h \
    tpacctlistitemcreator.h \
    tpacctlistitem.h \
    tpaccteditpage.h \
    appletif.h \
    tpacctlistwidget.h \
    chatbrief.h

SOURCES += \ 
    tpacctsetupwidget.cpp \
    tpacctlistitemcreator.cpp \
    tpacctlistitem.cpp \
    tpaccteditpage.cpp \
    appletif.cpp \
    tpacctlistwidget.cpp \
    chatbrief.cpp


INSTALLS += target
