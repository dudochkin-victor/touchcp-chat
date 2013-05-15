LANGUAGES =  # We only create engineering English in the application package
CATALOGNAME = libmeegochatapplet.so
SOURCEDIR = ..//src
TRANSLATIONDIR = $$PWD
include($$[QT_INSTALL_DATA]/mkspecs/features/meegotouch_defines.prf)
include($$[QT_INSTALL_DATA]/mkspecs/features/meegotouch_translations.prf)
