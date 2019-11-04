/*
 * The MIT License (MIT)
 * Copyright (c) 2018 Matheus Scattolin Anselmo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * The Software is provided “as is”, without warranty of any kind, express or
 * implied, including but not limited to the warranties of merchantability,
 * fitness for a particular purpose and noninfringement. In no event shall the
 * authors or copyright holders be liable for any claim, damages or other
 * liability, whether in an action of contract, tort or otherwise, arising from,
 * out of or in connection with the software or the use or other dealings in the
 * Software.
 */

#ifndef MWINDOW_HPP
#define MWINDOW_HPP

#include <QLabel>
#include <QMainWindow>
#include <QObject>
#include <QWidget>

#include "src/gui/workbench.hpp"
#include "src/libs/QSingleInstance/qsingleinstance.h"
#include "src/trackdata.hpp"
#include "src/vars.hpp"

#include "src/gui/dialogs/about.hpp"
#include "src/gui/dialogs/settings.hpp"
#include "src/gui/dialogs/tageditor.hpp"
#include "src/gui/dialogs/transcoder.hpp"

#include "src/gui/cwidgets/progslider.hpp"
#include "src/gui/cwidgets/toolbar.hpp"
#include "src/gui/cwidgets/volslider.hpp"

/*
 * This class represents the main window. duh.
 *
 * it holds the statusbar, toolbar and the workbench,
 * but it also parents all the other dialogs and reroutes some signals
 * between its children, ensuring some modularity between each other.
 */
class mwindow : public QMainWindow {
    Q_OBJECT
public:
    mwindow(vars* jag);
    ~mwindow() {
    }
    workbench* wb;

protected:
    void closeEvent(QCloseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void moveEvent(QMoveEvent* event);

private:
    enum pstate { PLAYING, PAUSED, IDLE };
    pstate state = pstate::IDLE;
    // TagLib::FileRef* curtag;
    vars* jag;
    QLabel status;
    QLabel proglabel;
    volslider* vol;
    progslider* prog;

    QMenu* configmenu;
    QThread* playert;
    toolbar* bar;
    settings* sdiag;
    transcoder* transcdiag;
    tageditor* tagdiag;
    about* adiag;
    // QString playing;

    trackdata* track = nullptr;

    void loadstate();

    void resizeEvent(QResizeEvent* event);
    // void restore_state();
    // void restorewinsize();
    // void dumpwinsize();
    void update_proglabel(uint pos, uint len);
public slots:
    void newinstance_act(const QStringList args);
    /* responsible for handling the toolbar buttons actions */
    void toolbar_pause();
    void toolbar_play();
    void toolbar_stop();
    void toolbar_next();
    void toolbar_prev();
    void config_spawn();
    void about_spawn();

    void playlist_enqueue(const QStringList files);

    /* spawns the transcoder window and contents */
    void transcoder_spawn(QStringList* l, bool discard);

    /* spawns the tag editor window and children */
    void tageditor_spawn(QStringList* l);

    /* stores the last clicked item */
    void select(QTreeWidgetItem* item);


    void show();
    void savestate();
private slots:
    /* syncs the playback position to the progslider state */

    void progslider_moved(int x);
    // void progslider_clicked();
    // void progslider_changed(int x);
    void volslider_moved(int x);
    // void player_respond(int status);
    void uilock_respond();
    void notify(bool playing, QString summary, QString body);

    /* these will come back from the player instance */
    void on_player_set(QString file);
    void on_player_EOM();

    void progslider_sync(QTime pos);
    void progslider_set(QTime length);
signals:
    /* these go directly to gstreamer. will also act as a proxy
     * for workbench's children to indirectly manipulate the player */
    void player_play();
    void player_set(QString file);
    void player_stop();
    void player_pause();
    void player_setvol(uint vol);
    void player_seek(short sec);

    void playlist_append(const QStringList files, const int playlist = -1);
    void playlist_replace(const QStringList files, const int playlist = -1);
    void playlist_next();
    void playlist_prev();
    void cover_set(QPixmap cover);

    /* signals when the layout editor lock has changed */
    void uilock_flip();

    void clearcover();
    void selectionchanged(QString item);
    void coverchanged(QPixmap* cover);
    void libchanged(QSqlDatabase* db);
    // void plappend(QStringList l);
    void convhandler(QStringList* files);
    void mediastatus(int status);
    void volumechanged(int vol);
    // void progsliderchanged(int pos);
};

#endif // MWINDOW_HPP
