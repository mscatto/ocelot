/*
 * Ocelot Music Manager: music player and library manager built using Qt
 * Copyright (C) 2018 Matheus Scattolin Anselmo
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MWINDOW_HPP
#define MWINDOW_HPP

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QMediaPlayer>

#include "libmgr.hpp"
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
    vars *jag;
    libmgr *lib;
    QMediaPlayer *player;
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
public slots:
    void toolbar_pause();
    void toolbar_play();
    void toolbar_stop();
    void toolbar_next();
    void toolbar_prev();
    void toolbar_menu();
    void config_spawn();
    void about_spawn();
    void transcoder_spawn();
    void tageditor_spawn();
    void play(QTreeWidgetItem* item);
    void select(QTreeWidgetItem *item);
private slots:
    void progslider_sync(qint64 x);
    void progslider_moved(int x);
    void progslider_clicked(int action);
    void progslider_released();
    void volslider_moved(int x);
    void media_status(QMediaPlayer::MediaStatus status);
signals:
    void selectionchanged(QString item);
    void coverchanged(QPixmap *cover);
    void libchanged(QSqlDatabase *db);
    void plappend(QStringList l);
    void convhandler(QStringList files);
    void plnext();
    void plprev();
    void mediastatus(QMediaPlayer::MediaStatus status);
};

#endif // MWINDOW_HPP
