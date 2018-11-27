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
#ifndef PLAYLISTVIEW_HPP
#define PLAYLISTVIEW_HPP

#include <QObject>
#include <QWidget>
#include <QTabWidget>
#include <QToolButton>
#include "src/mwindow.hpp"
#include "playlist.hpp"

class playlistview : public QTabWidget
{
    Q_OBJECT
private:
    QMap<QString, playlist*> pl;
    playlist *plactive; /* the one on display */
    playlist *plplaying; /* from currently playing track */
    QString order;
    QMenu *headermenu;
    QMenu *ctx;
    vars *jag;
    mwindow *win;
    QDialog *renamer;
    QLineEdit *namebox;
    QString tmpkey;
    QTreeWidgetItem *currentitem;

    QToolButton *addbtn;

    void ctxmenu(const QPoint &pos);
    void newplaylist();
    //QMap<QString, QAction*> menuctx;
public:
    playlistview(vars *jag, mwindow *parent);
    ~playlistview();
public slots:
    void viewappend(QStringList f);
    void context(QPoint p);
    void playing(QString f);
    void refreshpl();
    void next();
    void prev();
    void medistatus(QMediaPlayer::MediaStatus status);
    void swapitem(QTreeWidgetItem* item);
private slots:
    void toggler(bool checked);
    void tab_close(int index);
    void tab_switch(int index);
    void tab_rename(QString key);
    void renamer_ok();
    void renamer_cancel();
signals:
    void clearsel();
    void clearpl();
    void exportpl();
};

#endif // PLAYLISTVIEW_HPP
