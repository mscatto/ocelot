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
