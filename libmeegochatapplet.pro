include($$[QT_INSTALL_DATA]/mkspecs/dcpconfig.pri)
include(common.pri)
TEMPLATE = subdirs
SUBDIRS += src translations
include(theme/theme.pri)
OTHER_FILES += meegotouchcp-chat.desktop

#FIXME: should not hardcode
desktop_entry.path = /usr/lib/duicontrolpanel
desktop_entry.files = meegotouchcp-chat.desktop


INSTALLS += desktop_entry
