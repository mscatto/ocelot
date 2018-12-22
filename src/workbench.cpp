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
#include <QObject>
#include <QMediaPlayer>
#include <QMainWindow>
#include <QGridLayout>
#include <QSizePolicy>
#include <QWidget>
#include <QFrame>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QDebug>
#include <QSqlQuery>

#include "workbench.hpp"
#include "library.hpp"
#include "mwindow.hpp"
#include "vars.hpp"

#include "cwidgets/coverview.hpp"
#include "cwidgets/libtree.hpp"
#include "cwidgets/tagview.hpp"
#include "cwidgets/playlistview.hpp"
#include "cwidgets/splitter.hpp"
#include "cwidgets/dummywidget.h"

workbench::workbench(vars *jag, QWidget *win) : QWidget(){
    //this->setFrameStyle(QFrame::Shape::StyledPanel);
    //this->setFrameStyle(QFrame::Sunken);
    this->jag = jag;
    this->win = win;
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(qobject_cast<mwindow*>(win), &mwindow::uilock_flip, this, &workbench::lock_flip);
    connect(this, &QWidget::customContextMenuRequested, this, &workbench::ctx_req);
    this->ml = new QGridLayout();
    //mwindow *mainwin = qobject_cast<mwindow*>(win);
    this->ml->setContentsMargins(0,0,0,0);
    this->setLayout(this->ml);
    this->setGeometry(win->rect());

    this->ml->addWidget(new dummywidget("First, unlock the widgets on the cogwheel toolbar button\nThen replace this by right-clicking on an empty space!"));

    this->ctx_replace = new QMenu();
    this->ctx_replace->addSection("Splitters");
    this->ctx_replace->addAction("Vertical (top and bottom)");
    this->ctx_replace->addAction("Horizontal (left and right)");
    this->ctx_replace->addSection("Default Widgets");
    this->ctx_replace->addAction("Tree-based library viewer");
    this->ctx_replace->addAction("Column-based library viewer");
    this->ctx_replace->addAction("Playlist Manager");
    this->ctx_replace->addAction("Tag widget");
    this->ctx_replace->addAction("Cover widget");
    this->ctx_replace->addAction("Lyrics widget");
    this->ctx_replace->addAction("Milkdrop Visualization");

    QList<QAction*> act = this->ctx_replace->actions();
    connect(act.at(1), &QAction::triggered, this, &workbench::add_vsplitter);
    connect(act.at(2), &QAction::triggered, this, &workbench::add_hsplitter);
    connect(act.at(4), &QAction::triggered, this, &workbench::add_libtree);
    connect(act.at(6), &QAction::triggered, this, &workbench::add_playlistmgr);
    connect(act.at(7), &QAction::triggered, this, &workbench::add_tagview);
    connect(act.at(8), &QAction::triggered, this, &workbench::add_coverview);

    //grid->addWidget(new libtree(qobject_cast<mwindow*>(win)));

    /*splitter *x = new splitter(Qt::Horizontal, this);
    splitter *y = new splitter(Qt::Vertical, this);
    splitter *z = new splitter(Qt::Horizontal, this);
    libtree *j = new libtree(qobject_cast<mwindow*>(win));
    j->populate(jag->DB_REF);
    //connect(j->tree, )
    x->addWidget(j);
    x->addWidget(y);

    y->addWidget(new playlistview(jag, qobject_cast<mwindow*>(win)));
    y->addWidget(z);

    z->addWidget(new coverview(qobject_cast<mwindow*>(win)));
    z->addWidget(new tagview(jag, qobject_cast<mwindow*>(win)));

    grid->addWidget(x);*/
    /* expand further...*/
}

workbench::~workbench(){

}

void workbench::lock_flip(){
    this->locked = !this->locked;

    if(!this->locked){
        qDebug() << "UNLOCKED!";
    }

}

void workbench::ctx_req(QPoint p){
    if(this->locked)
        return;

    /* this will be used elsewhere by the actions below */
    this->ctx_lastpos = p;

    QMenu ctx;
    QString *s;
    ctx.addSection("Layout Editor");
    if(this->childAt(this->ctx_lastpos)!=nullptr){
        s = new QString(this->childAt(this->ctx_lastpos)->metaObject()->className());
        if(*s=="QLabel")
            *s = "placeholder";
        else if(*s=="QSplitter")
            *s = "splitter";
    }else
        s = new QString("emptiness");

    //ctx.addSeparator();
    this->ctx_replace->setTitle("Replace "+*s+" with...");
    ctx.addMenu(this->ctx_replace);

    QAction *remobj = new QAction();
    if(this->childAt(this->ctx_lastpos)==nullptr){
        remobj->setText("Can't remove workbench");
        remobj->setEnabled(false);

    }else{
        remobj->setText("Remove "+*s);
    }
    connect(remobj, &QAction::triggered, this, &workbench::remove_widget);
    //ctx.addAction(remobj);


    if(this->childAt(this->ctx_lastpos)->parent()->metaObject()->className()==QString("QSplitter")){
        QAction *rempar = new QAction();
        rempar->setText("Remove underlying splitter");
        connect(rempar, &QAction::triggered, this, &workbench::remove_parentwidget);
        ctx.addAction(rempar);
    }

    QAction *clear = new QAction();
    clear->setText("Clear workbench");
    connect(clear, &QAction::triggered, this, &workbench::clear);
    ctx.addAction(clear);

    ctx.exec(this->mapToGlobal(p));
}

void workbench::inject(QWidget *w){
    if(this==this->childAt(this->ctx_lastpos))
        this->ml->addWidget(w);
    /* case it is the root widget */
    else if(this==this->childAt(this->ctx_lastpos)->parent()){
        //if(this->children().count()>0){
            QWidget *old = this->childAt(this->ctx_lastpos);
            this->ml->replaceWidget(old, w);
            old->~QWidget();
        /*}else{
            this->ml->addWidget(w);
        }*/
    }

    /* if it is not, expect the parent to be a splitter */
    else if(this->childAt(this->ctx_lastpos)->parent()->metaObject()->className()==QString("QSplitter")){
        QSplitter *par = qobject_cast<QSplitter*>(this->childAt(this->ctx_lastpos)->parent());
        QWidget *old = par->widget(par->indexOf(this->childAt(this->ctx_lastpos)));
        par->replaceWidget(par->indexOf(this->childAt(this->ctx_lastpos)), w);
        old->~QWidget();
    }
    /* but if it isn't, something went wrong */
    else qWarning() << "[WARNING] at workbench::inject - widget is neither a splitter or a dummy!";

    this->refreshdb();
}

void workbench::refreshdb(){
    QString out;
    this->recurscan(this, &out);
    out.chop(1); /* to remove trailing ; */

    qInfo() << out;

    QSqlQuery *q = new QSqlQuery(*this->jag->DB_REF);
    q->prepare("UPDATE data SET val = :scheme WHERE var LIKE 'ui_scheme'");
    q->bindValue(":scheme", out);
    q->exec();

    q->~QSqlQuery();
}

void workbench::recurscan(QWidget *w, QString *out){
    if(w==this){
        foreach(QObject *o, this->children()){
            if(o->metaObject()->className()!=QString("QPropertyAnimation")
            && o->metaObject()->className()!=QString("QGridLayout"))
                this->recurscan(qobject_cast<QWidget*>(o), out);
        }
        return;
    }else if(w->metaObject()->className()==QString("QSplitter")){
        QObjectList z = qobject_cast<QSplitter*>(w)->children();
        foreach(QObject *o, z)
            if(o->metaObject()->className()!=QString("QSplitterHandle")
            && !QString(o->metaObject()->className()).contains("Proxy"))
                this->recurscan(qobject_cast<QWidget*>(o), out);
    }
    *out+=QString(w->metaObject()->className()+QString(";"));

}

void workbench::add_vsplitter(){
    QSplitter *z = new QSplitter();
    z->setOrientation(Qt::Orientation::Vertical);
    z->setChildrenCollapsible(false);
    z->addWidget(new dummywidget("Replace me, ya dingus!"));
    z->addWidget(new dummywidget("Bottoms' up, silly!"));

    this->inject(z);
}

void workbench::add_hsplitter(){
    QSplitter *z = new QSplitter();
    z->setOrientation(Qt::Orientation::Horizontal);
    z->setChildrenCollapsible(false);
    z->addWidget(new dummywidget("Left-winger"));
    z->addWidget(new dummywidget("I'm being crushed sideways!"));

    this->inject(z);
}

void workbench::add_libtree(){
    libtree *lib = new libtree(qobject_cast<mwindow*>(this->win));
    this->inject(lib);
}

void workbench::add_playlistmgr(){
    playlistview *pl = new playlistview(this->jag, qobject_cast<mwindow*>(win));
    this->inject(pl);
}

void workbench::add_coverview(){
    coverview *cv = new coverview(qobject_cast<mwindow*>(win));
    this->inject(cv);
}

void workbench::add_tagview(){
    tagview *t = new tagview(this->jag, qobject_cast<mwindow*>(win));
    this->inject(t);
}

/* removes the parent of a widget located at ctx_lastpos */
/* --- apropriate checks should've already been made */
void workbench::remove_parentwidget(){
    QWidget *rem = this->childAt(this->ctx_lastpos)->parentWidget();
    QWidget *parent = rem->parentWidget();
    if(parent==this){
        this->ml->replaceWidget(rem, new dummywidget("I'm just a placeholder.\nRight-click to replace me!"));
        rem->~QWidget();
    }else{
        int pos = qobject_cast<QSplitter*>(parent)->indexOf(rem);
        qobject_cast<QSplitter*>(parent)->replaceWidget(pos, new dummywidget("I'm just a placeholder.\nRight-click to replace me!"));
        rem->~QWidget();
    }
}

/* removes the widget at ctx_lastpos */
void workbench::remove_widget(){
    qDebug() << this->childAt(this->ctx_lastpos);
    if(this==this->childAt(this->ctx_lastpos)->parent()){
        QWidget *w = this->childAt(this->ctx_lastpos);
        this->ml->removeWidget(w);
        w->~QWidget();
    }else if(this->childAt(this->ctx_lastpos)->parent()->metaObject()->className()==QString("QSplitter")){
        QSplitter *par = qobject_cast<QSplitter*>(this->childAt(this->ctx_lastpos)->parent());
        par->childAt(this->ctx_lastpos)->~QWidget();
    }

    if(this->ml->children().count()==0)
        this->ml->addWidget(new dummywidget("First, unlock the widgets on the cogwheel toolbar button\nThen replace this by right-clicking on an empty space!"));
}

/* removes all the widgets from the workbench's layout */
/* it seems that it will always be 3 children from workbench: QGridLayout, QPropertyAnimation, */
/* and whichever widget was first added. splitter's children do not show */
void workbench::clear(){
    foreach(QObject *o, this->children())
        if(o->metaObject()->className()!=QString("QGridLayout") &&
        o->metaObject()->className()!=QString("QPropertyAnimation"))
           o->~QObject();

    this->ml->addWidget(new dummywidget("I'm just a placeholder.\nRight-click to replace me!"));
}

