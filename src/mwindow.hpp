/*
 * The MIT License (MIT)
 * Copyright (c) 2018 Matheus Scattolin Anselmo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * The Software is provided “as is”, without warranty of any kind, express or implied,
 * including but not limited to the warranties of merchantability, fitness for a
 * particular purpose and noninfringement. In no event shall the authors or copyright
 * holders be liable for any claim, damages or other liability, whether in an action
 * of contract, tort or otherwise, arising from, out of or in connection with the
 * software or the use or other dealings in the Software.
 */

#ifndef MWINDOW_HPP
#define MWINDOW_HPP

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QMediaPlayer>

#include "workbench.hpp"
#include "vars.hpp"

#include "dialogs/settings.hpp"
#include "dialogs/about.hpp"
#include "dialogs/transcoder.hpp"
#include "dialogs/tageditor.hpp"

#include "cwidgets/volslider.hpp"
#include "cwidgets/progslider.hpp"
#include "cwidgets/toolbar.hpp"

/*
 * This class represents the main window. duh.
 *
 * it holds the statusbar, toolbar and the workbench,
 * but it also parents all the other dialogs and reroutes some signals
 * between its children, ensuring modularity between each other.
 */
class mwindow : public QMainWindow
{
    Q_OBJECT
public:
    mwindow(vars *jag);
    ~mwindow();
private:
    bool paused = false; //ugly
    TagLib::FileRef *fr;
    vars *jag;
    QLabel status;
    QLabel proglabel;
    volslider *vol;
    progslider *prog;
    workbench *wb;
    QMenu *configmenu;
    toolbar *bar;
    settings *sdiag;
    transcoder *transcdiag;
    tageditor *tagdiag;
    about *adiag;
    void closeEvent(QCloseEvent *event);
    QMediaPlayer::MediaStatus playerstatus = QMediaPlayer::MediaStatus::UnknownMediaStatus;

    /* the dumping of the window size to the db works behind a timer.
     * every time the user resizes the window, a 1sec timer is started.
     * new resize events while the timer is active will just reset it
     * in an attempt to reduce db writes */
    QTimer *resizetimer;
    void resizeEvent(QResizeEvent* event);
    void restorewinsize();
    void dumpwinsize();
public slots:
    /* responsible for handling the toolbar buttons actions */
    void toolbar_pause();
    void toolbar_play();
    void toolbar_stop();
    void toolbar_next();
    void toolbar_prev();
    void toolbar_menu();
    void config_spawn();
    void about_spawn();

    /* spawns the transcoder window and contents */
    void transcoder_spawn();

    /* spawns the tag editor window and children */
    void tageditor_spawn(QStringList *l);

    /* it plays the QURL inside 'item' data */
    void play(QTreeWidgetItem* item);

    /* stores the last clicked item */
    void select(QTreeWidgetItem *item);

    void show();
private slots:
    /* syncs the playback position to the progslider state */
    void progslider_sync(qint64 pos);

    /* called when progslider is dragged */
    void progslider_moved(int x);
    /* called when progslider is clicked somewhere along its body */
    void progslider_clicked(int action);
    void progslider_changed(int x);
    void volslider_moved(int x);
    void player_respond(QMediaPlayer::MediaStatus status);
    void uilock_respond();
signals:
    /* signals when the layout editor lock has changed */
    void uilock_flip();
    void player_play(QString *file);
    void clearcover();
    void selectionchanged(QString item);
    void coverchanged(QPixmap *cover);
    void libchanged(QSqlDatabase *db);
    void plappend(QStringList l);
    void convhandler(QStringList files);
    void plnext();
    void plprev();
    void mediastatus(QMediaPlayer::MediaStatus status);
    void volumechanged(int vol);
    void progsliderchanged(int pos);
};

#endif // MWINDOW_HPP
