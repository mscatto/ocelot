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

#include "toolbar.hpp"
#include "src/mwindow.hpp"

#include <QToolBar>
#include <QToolButton>
#include <QMenu>

toolbar::toolbar(QWidget *win, QMenu *conf) : QToolBar(win){
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    conf = new QMenu(qobject_cast<mwindow*>(win));
    conf->addAction(new QAction("Open File..."));
    conf->addAction(new QAction("Play Disc..."));
    conf->addAction(new QAction("Play from network..."));
    conf->addSeparator();
    conf->addAction(new QAction("Settings"));
    conf->addSeparator();
    conf->addAction(new QAction("Toggle Layout Editor"));
    conf->addSeparator();
    conf->addAction(new QAction("About"));
    conf->addAction(new QAction("Exit"));

    this->addAction(new QAction(QIcon(":/internals/play"), "Resume media playback"));
    this->addAction(new QAction(QIcon(":/internals/pause"), "Pause media playback"));
    this->addAction(new QAction(QIcon(":/internals/stop"), "Stop media playback"));
    this->addAction(new QAction(QIcon(":/internals/prev"), "Previous on playlist"));
    this->addAction(new QAction(QIcon(":/internals/next"), "Next on playlist"));
    QAction *a = new QAction(QIcon(":/internals/gear"), "Tweak settings");
    a->setMenu(conf);

    this->addAction(a);

    QList<QAction*> act = this->actions();
    connect(act.at(0), &QAction::triggered, qobject_cast<mwindow*>(win), &mwindow::toolbar_play);
    connect(act.at(1), &QAction::triggered, qobject_cast<mwindow*>(win), &mwindow::toolbar_pause);
    connect(act.at(2), &QAction::triggered, qobject_cast<mwindow*>(win), &mwindow::toolbar_stop);
    connect(act.at(3), &QAction::triggered, qobject_cast<mwindow*>(win), &mwindow::toolbar_prev);
    connect(act.at(4), &QAction::triggered, qobject_cast<mwindow*>(win), &mwindow::toolbar_next);
    connect(act.at(5), &QAction::triggered, qobject_cast<mwindow*>(win), &mwindow::toolbar_menu);

    connect(act.last(), &QAction::triggered, conf, &QMenu::show);
    connect(conf->actions().at(4), &QAction::triggered, qobject_cast<mwindow*>(win), &mwindow::config_spawn);
    connect(conf->actions().at(6), &QAction::triggered, qobject_cast<mwindow*>(win), &mwindow::uilock_flip);
    connect(conf->actions().at(8), &QAction::triggered, qobject_cast<mwindow*>(win), &mwindow::about_spawn);
    //btn->setAutoRaise(true);
    this->addSeparator();
}

toolbar::~toolbar(){

}

void toolbar::pop(){
    qDebug() << this->mapToGlobal(this->pos());
    this->conf->popup(this->mapToGlobal(QPoint(this->actionGeometry(this->actions().last()).x(), this->actionGeometry(this->actions().last()).x())), this->actions().last());
}

void toolbar::addWidget(QWidget *widget){
    QToolBar::addWidget(widget);
    docked.append(widget);
}

void toolbar::orient(Qt::Orientation o){
    Qt::Orientation x = o;
    x = o;
    /*QWidget *w;
    foreach(w, this->docked){
        qobject_cast<?>(w).rotate(o);
    }*/
}
