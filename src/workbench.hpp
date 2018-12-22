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

#ifndef WORKBENCH_HPP
#define WORKBENCH_HPP

#include <QWidget>
#include <QFrame>
#include <QMediaPlayer>
#include <QMainWindow>
#include <taglib/fileref.h>
#include "library.hpp"
#include "vars.hpp"

class workbench : public QWidget
{
private:
    bool locked = true;
    QGridLayout *ml;
    QWidget *win;
    QPoint ctx_lastpos;
    QMenu *ctx_replace;
    void inject(QWidget *w);
    void refreshdb();
    void recurscan(QWidget *w, QString *out);
    vars *jag;
public:
    workbench(vars *jag, QWidget* win);
    ~workbench();
public slots:
    void lock_flip();
    void ctx_req(QPoint p);
    void add_vsplitter();
    void add_hsplitter();
    void add_libtree();
    void add_playlistmgr();
    void add_coverview();
    void add_tagview();
    void remove_parentwidget();
    void remove_widget();
    void clear();

private slots:
    //void a
};

#endif // WORKBENCH_HPP
