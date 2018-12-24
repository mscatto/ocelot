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

class mwindow : public QMainWindow
{
    Q_OBJECT
public:
    mwindow(vars *jag);
    ~mwindow();

    //vars *jaguatirica();
private:
    bool uimanager = false;
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
    QMediaPlayer::MediaStatus prev = QMediaPlayer::MediaStatus::UnknownMediaStatus;
public slots:
    void toolbar_pause(bool res=false);
    void toolbar_play();
    void toolbar_stop();
    void toolbar_next();
    void toolbar_prev();
    void toolbar_menu();
    void config_spawn();
    void about_spawn();
    void transcoder_spawn();
    void tageditor_spawn(QStringList *l);
    void play(QTreeWidgetItem* item);
    void select(QTreeWidgetItem *item);
private slots:
    void progslider_sync(qint64 pos);
    void progslider_moved(int x);
    void progslider_clicked(int action);
    void progslider_changed(int x);
    void volslider_moved(int x);
    void player_respond(QMediaPlayer::MediaStatus status);
    void uilock_respond();
signals:
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
