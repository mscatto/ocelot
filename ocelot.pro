#-------------------------------------------------
#
# Project created by QtCreator 2018-03-07T16:49:55
#
#-------------------------------------------------

QT += core gui sql widgets dbus network multimedia

TARGET = ocelot
#DESTDIR = bin
#OBJECTS_DIR = /tmp/ocelotbuild
#MOC_DIR = /tmp/ocelotbuild
TEMPLATE = app
CONFIG += link_pkgconfig
#LIBS += -L/usr/lib/libprojectM -lprojectM -L/usr/lib/libtag -ltag -L/usr/lib/libQt5GStreamer-1.0 -lQt5GStreamer-1.0
#LIBS += -L/usr/lib/libQt5GLib-2.0 -lQt5GLib-2.0
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

DISTFILES += \
	LICENSE \
	README.md \
	README.md \ \
	src/libs/QSingleInstance/LICENSE \
	src/libs/libnotify-qt/LICENSE \
	uncrustify.cfg

HEADERS += \
	src/gui/cwidgets/coverview.hpp \
	src/gui/cwidgets/dummywidget.hpp \
	src/gui/cwidgets/libtree.hpp \
	src/gui/cwidgets/playlist.hpp \
	src/gui/cwidgets/playlistview.hpp \
	src/gui/cwidgets/progslider.hpp \
	src/gui/cwidgets/splitter.hpp \
	src/gui/cwidgets/tabbutton.hpp \
	src/gui/cwidgets/tagview.hpp \
	src/gui/cwidgets/toolbar.hpp \
	src/gui/cwidgets/visualizer.hpp \
	src/gui/cwidgets/volslider.hpp \
	src/gui/dialogs/about.hpp \
	src/gui/dialogs/settings.hpp \
	src/gui/dialogs/tageditor.hpp \
	src/gui/dialogs/transchelper.hpp \
	src/gui/dialogs/transcoder.hpp \
	src/gui/dialogs/wizard.hpp \
	src/gui/helpers/flachelper.hpp \
	src/gui/helpers/intro.hpp \
	src/gui/mwindow.hpp \
	src/gui/workbench.hpp \
	src/library.hpp \
	src/libs/QSingleInstance/clientinstance.h \
	src/libs/QSingleInstance/qsingleinstance.h \
	src/libs/QSingleInstance/qsingleinstance_p.h \
	src/libs/libnotify-qt/Notification.h \
	src/libs/libnotify-qt/OrgFreedesktopNotificationsInterface.h \
	src/player.hpp \
	src/trackdata.hpp \
	src/utils/topostfix.hpp \
	src/vars.hpp


SOURCES += \
	src/_main.cpp \
	src/gui/cwidgets/coverview.cpp \
	src/gui/cwidgets/dummywidget.cpp \
	src/gui/cwidgets/libtree.cpp \
	src/gui/cwidgets/playlist.cpp \
	src/gui/cwidgets/playlistview.cpp \
	src/gui/cwidgets/progslider.cpp \
	src/gui/cwidgets/splitter.cpp \
	src/gui/cwidgets/tabbutton.cpp \
	src/gui/cwidgets/tagview.cpp \
	src/gui/cwidgets/toolbar.cpp \
	src/gui/cwidgets/visualizer.cpp \
	src/gui/cwidgets/volslider.cpp \
	src/gui/dialogs/about.cpp \
	src/gui/dialogs/settings.cpp \
	src/gui/dialogs/tageditor.cpp \
	src/gui/dialogs/transchelper.cpp \
	src/gui/dialogs/transcoder.cpp \
	src/gui/dialogs/wizard.cpp \
	src/gui/helpers/flachelper.cpp \
	src/gui/helpers/intro.cpp \
	src/gui/mwindow.cpp \
	src/gui/workbench.cpp \
	src/library.cpp \
	src/libs/QSingleInstance/clientinstance.cpp \
	src/libs/QSingleInstance/qsingleinstance.cpp \
	src/libs/QSingleInstance/qsingleinstance_p.cpp \
	src/libs/libnotify-qt/Notification.cpp \
	src/libs/libnotify-qt/OrgFreedesktopNotificationsInterface.cpp \
	src/player.cpp \
	src/trackdata.cpp \
	src/utils/topostfix.cpp \
	src/vars.cpp

RESOURCES += \
	res/package.qrc
