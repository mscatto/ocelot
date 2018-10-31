#-------------------------------------------------
#
# Project created by QtCreator 2018-03-07T16:49:55
#
#-------------------------------------------------

QT       += core gui multimedia sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ocelot
TEMPLATE = app
OUTPUT += console
QT_CONFIG -= no-pkg-config
CONFIG += link_pkgconfig debug
CONFIG += static
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


SOURCES += \
		src/main.cpp \
	src/library.cpp \
	src/workbench.cpp \
	cwidgets/coverview.cpp \
	cwidgets/libtree.cpp \
	cwidgets/progslider.cpp \
	cwidgets/splitter.cpp \
	cwidgets/tagview.cpp \
	cwidgets/volslider.cpp \
	dialogs/about.cpp \
	cwidgets/toolbar.cpp \
	dialogs/settings.cpp \
	src/mwindow.cpp \
	src/vars.cpp \
    dialogs/transcoder.cpp \
    cwidgets/playlistview.cpp \
    cwidgets/playlist.cpp \
    cwidgets/renamepbtn.cpp \
    dialogs/tageditor.cpp \
    src/worker.cpp

HEADERS += \
	src/library.hpp \
	src/workbench.hpp \
	cwidgets/coverview.hpp \
	cwidgets/tagview.hpp \
	cwidgets/splitter.hpp \
	cwidgets/progslider.hpp \
	cwidgets/libtree.hpp \
	cwidgets/volslider.hpp \
	dialogs/about.hpp \
	cwidgets/toolbar.hpp \
	dialogs/settings.hpp \
	src/mwindow.hpp \
	src/vars.hpp \
    dialogs/transcoder.hpp \
    cwidgets/playlistview.hpp \
    cwidgets/playlist.hpp \
    cwidgets/renamepbtn.hpp \
    dialogs/tageditor.hpp \
    src/worker.hpp

FORMS += \

RESOURCES += \
	pak.qrc
