#-------------------------------------------------
#
# Project created by QtCreator 2018-03-07T16:49:55
#
#-------------------------------------------------

QT += core gui multimedia sql widgets

TARGET = ocelot
TEMPLATE = app
CONFIG += link_pkgconfig
PKGCONFIG += taglib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RESOURCES += \
	pak.qrc

DISTFILES += \
	README.md

HEADERS += \
	cwidgets/coverview.hpp \
	cwidgets/dummywidget.hpp \
	cwidgets/libtree.hpp \
	cwidgets/playlist.hpp \
	cwidgets/playlistview.hpp \
	cwidgets/progslider.hpp \
	cwidgets/renamepbtn.hpp \
	cwidgets/tagview.hpp \
	cwidgets/toolbar.hpp \
	cwidgets/volslider.hpp \
	dialogs/about.hpp \
	dialogs/settings.hpp \
	dialogs/tageditor.hpp \
	dialogs/transcoder.hpp \
	dialogs/wizard.hpp \
	src/library.hpp \
	src/mwindow.hpp \
	src/player.hpp \
	src/vars.hpp \
	src/workbench.hpp \
	src/topostfix.hpp \
    cwidgets/visualizer.hpp

SOURCES += \
	cwidgets/coverview.cpp \
	cwidgets/dummywidget.cpp \
	cwidgets/libtree.cpp \
	cwidgets/playlist.cpp \
	cwidgets/playlistview.cpp \
	cwidgets/progslider.cpp \
	cwidgets/renamepbtn.cpp \
	cwidgets/tagview.cpp \
	cwidgets/toolbar.cpp \
	cwidgets/volslider.cpp \
	dialogs/about.cpp \
	dialogs/settings.cpp \
	dialogs/tageditor.cpp \
	dialogs/transcoder.cpp \
	dialogs/wizard.cpp \
	src/library.cpp \
	src/main.cpp \
	src/mwindow.cpp \
	src/player.cpp \
	src/vars.cpp \
	src/workbench.cpp \
	src/topostfix.cpp \
    cwidgets/visualizer.cpp
