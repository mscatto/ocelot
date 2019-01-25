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
    enum defwidgets{
        VSPLIT = '-',
        HSPLIT = '+',
        DUMMY = 'z',
        TREELIB = 'a',
        PANELLIB = 'b',
        PLMGR = 'c',
        TAGVIEW = 'd',
        COVERVIEW = 'e',
        VISUALIZER = 'f'
    };

private:
    /* this controls the layout editor toggler */
    bool locked = true;

    /* the workbench's layout */
    QGridLayout *ml;

    /* mwindow pointer, in QWidget form to avoi circular dependencies */
    QWidget *win;

    /* last right-click position, stored to keep track to where to inject new widgets */
    QPoint ctx_lastpos;

    /* menu shown when the user right-clicks on an existing widget */
    QMenu *ctx_replace;

    /* this function inserts the widget *w on the position stored in ctx_lastpos; */
    void inject(QWidget *w);

    /* refreshdb stores the current layout on the database */
    void refreshdb();

    /* this applies the layout *l to the workbench */
    void setlayout(QString *l);

    /* responsible for fetching the objname layout letter code for database storage */
    char fetchid(QString objname);

    /* recursively dumps the workbench layout string to the *out pointer */
    void dumplayout(QObject *n, QString *out);

    /* returns a pointer a for newly created widget corresponding to the letter code *id */
    QWidget *fetchwidget(const char *id);

    /* globals */
    vars *jag;

    /* these _functions are responsible for new allocating new widgets */
    QWidget *_vsplitter(bool filled);
    QWidget *_hsplitter(bool filled);
    QWidget *_libtree();
    QWidget *_playlistmgr();
    QWidget *_coverview();
    QWidget *_tagview();
public:
    workbench(vars *jag, QWidget* win);
    bool islocked();
    ~workbench();
public slots:
    /* ctx_ slots are called by the context menu for the layout editor */
    void ctx_req(QPoint p);
    void ctx_vsplitter();
    void ctx_hsplitter();
    void ctx_libtree();
    void ctx_playlistmgr();
    void ctx_coverview();
    void ctx_tagview();

    /* flips the layout editor lock */
    void lock_flip();

    /* it removes the widget parent to the widget under ctx_lastpos */
    void remove_parentwidget();

    /* destroy the widget under ctx_lastpos */
    void remove_widget();

    /* clears the current layout */
    void clear();

private slots:
    //void a
};

#endif // WORKBENCH_HPP
