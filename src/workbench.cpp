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
#include <QSqlRecord>
#include <QByteArray>

#include "workbench.hpp"
#include "library.hpp"
#include "mwindow.hpp"
#include "vars.hpp"
#include "topostfix.hpp"

#include "cwidgets/coverview.hpp"
#include "cwidgets/libtree.hpp"
#include "cwidgets/tagview.hpp"
#include "cwidgets/playlistview.hpp"
#include "cwidgets/dummywidget.hpp"

#include <libprojectM/projectM.hpp>

workbench::workbench(vars *jag, QWidget *win) : QWidget(){
    this->jag = jag;
    this->win = win;
    this->lastctx = this;
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setContentsMargins(0,0,0,0);
    connect(qobject_cast<mwindow*>(win), &mwindow::uilock_flip, this, &workbench::lock_flip);
    connect(this, &QWidget::customContextMenuRequested, this, &workbench::ctx_req);
    this->ml = new QGridLayout();
    //mwindow *mainwin = qobject_cast<mwindow*>(win);
    this->ml->setContentsMargins(0,0,0,0);
    this->setLayout(this->ml);
    this->setGeometry(win->rect());

    QSqlQuery *q = new QSqlQuery(*this->jag->DB_REF);
    q->exec("select val from data where var='ui_scheme'");
    q->next();
    if(q->value(0)=="0"){
        this->ml->addWidget(new dummywidget("First, unlock the widgets on the cogwheel toolbar button\nThen replace this by right-clicking on an empty space!", this));
    }else
        this->setlayout(new QString(q->record().value(0).toString()));
    //projectM::
    this->ctx_replace = new QMenu();
    this->ctx_replace->setTitle("Replace with...");
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
    connect(act.at(1), &QAction::triggered, this, &workbench::ctx_vsplitter);
    connect(act.at(2), &QAction::triggered, this, &workbench::ctx_hsplitter);
    connect(act.at(4), &QAction::triggered, this, &workbench::ctx_libtree);
    connect(act.at(6), &QAction::triggered, this, &workbench::ctx_playlistmgr);
    connect(act.at(7), &QAction::triggered, this, &workbench::ctx_tagview);
    connect(act.at(8), &QAction::triggered, this, &workbench::ctx_coverview);
}

workbench::~workbench(){

}

/*void workbench::ctx_updlastpos(QPoint pos){
    this->lastctxpos = pos;
}*/

bool workbench::islocked(){
    return this->locked;
}

void workbench::setlastctx(QWidget *w){
    this->lastctx = w;
}

void workbench::lock_flip(){
    this->locked = !this->locked;

    if(!this->locked)
        qInfo() << "  -> Layout editor enabled!";
    else
        qInfo() << "  -> Layout editor disabled!";

}

void workbench::ctx_req(QPoint p){
    if(this->islocked())
        return;

    QMenu ctx;
    QString *s;
    ctx.addSection("Layout Editor");
    if(this->childAt(p)!=nullptr){
        s = new QString(this->childAt(p)->metaObject()->className());
        if(*s=="QLabel")
            *s = "placeholder";
        else if(*s=="QSplitter")
            *s = "splitter";
    }else
        s = new QString("emptiness");

    //ctx.addSeparator();
    ctx.addMenu(this->ctx_replace);

    QAction *remobj = new QAction();
    if(this->childAt(p)==nullptr){
        remobj->setText("Can't remove workbench");
        remobj->setEnabled(false);

    }else{
        remobj->setText("Remove "+*s);
    }
    connect(remobj, &QAction::triggered, this, &workbench::remove_widget);
    //ctx.addAction(remobj);


    if(this->childAt(p)->parent()->metaObject()->className()==QString("QSplitter")){
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

/* this reparents *w to the widget under ctx_lastpos click */
void workbench::inject(QWidget *w){/////////////////////////
    if(this==this->lastctx)
        this->ml->addWidget(w);
    /* case it is the root widget */
    else if(this==this->lastctx->parent()){
        QWidget *old = this->lastctx;
        this->ml->removeWidget(old);
        this->ml->addWidget(w);
        old->~QWidget();
    }

    /* if it is not, expect the parent to be a splitter */
    else if(this->lastctx->parent()->metaObject()->className()==QString("QSplitter")){
        QSplitter *par = qobject_cast<QSplitter*>(this->lastctx->parent());
        int z = par->indexOf(this->lastctx);
        par->widget(z)->~QWidget();
        par->insertWidget(z, w);
    }
    /* but if it isn't, then replace it */
    else {
        qWarning() << "[WARNING] at workbench::inject - "<< w;
    }
    this->refreshdb();
}

/* refreshdb handles writing the current layout to the database, converting it to postfix */
void workbench::refreshdb(){
    QString s;
    this->dumplayout(this->findChildren<QWidget*>().first(), &s);

    QSqlQuery *q = new QSqlQuery(*this->jag->DB_REF);
    q->prepare("UPDATE data SET val = :scheme WHERE var LIKE 'ui_scheme'");
    q->bindValue(":scheme", translate(s.toStdString()).c_str());
    q->exec();

    q->~QSqlQuery();
}

/* this function purpose is to rebuild the UI according to the postfix code on *l */
void workbench::setlayout(QString *l){
    if(l->isEmpty() || l->at(0)=="$"){
        this->clear();
        return;
    }
    QWidgetList stack;
    while(l->length()>0){
        if(l->front()=="{"){
            QString ss = l->section("}",0,0); /* regex would be cuter */
            l->remove(0,l->indexOf("}"));
            ss.remove("{");

            QSize ws(ss.split(";").first().toInt(),ss.split(";").last().toInt());
            stack.back()->resize(ws);
        }
        else if(l->front()!="+" && l->front()!="-"){ /* case it's a widget */
            stack.push_back(fetchwidget(qPrintable(l->at(0))));
        }else{
            QSplitter *ns;
            if(l->front()=="-"){
                ns = qobject_cast<QSplitter*>(workbench::_vsplitter(false));
            }else if(l->front()=="+"){
                ns = qobject_cast<QSplitter*>(workbench::_hsplitter(false));
            }else
                return;

            //connect(ns, &QSplitter::splitterMoved, this, &workbench::refreshdb);
            ns->insertWidget(1,stack.back());
            stack.pop_back();
            ns->insertWidget(0,stack.back());
            stack.pop_back();

            stack.push_back(ns);
        }
        l->remove(0,1);
    }
    this->ml->addWidget(stack.first());
    qobject_cast<mwindow*>(this->win)->libchanged(this->jag->DB_REF);
    l->~QString();
}

char workbench::fetchid(QString objname){
    if(objname=="libtree")
        return defwidgets::TREELIB;
    if(objname=="coverview")
        return defwidgets::COVERVIEW;
    if(objname=="tagview")
        return defwidgets::TAGVIEW;
    if(objname=="playlistview")
        return defwidgets::PLMGR;
    if(objname=="dummywidget")
        return defwidgets::DUMMY;
    else{
        qWarning() << "[WARNING] at workbench::fetchid: Unknown widget! _"<<objname;
        return '$';
    }
}

/* this one recurs down each splitter generating an infix expression representing the layout */
void workbench::dumplayout(QObject *n, QString *out){
    if(n->metaObject()->className()==QString("QPropertyAnimation"))
        return;
    if(n->metaObject()->className()==QString("QSplitter")){
        out->append("(");
        workbench::dumplayout(qobject_cast<QSplitter*>(n)->widget(0), out);
        if(qobject_cast<QSplitter*>(n)->orientation()==Qt::Orientation::Vertical)
            out->append(defwidgets::VSPLIT);
        else
            out->append(defwidgets::HSPLIT);
        workbench::dumplayout(qobject_cast<QSplitter*>(n)->widget(1), out);
        out->append(")");
    }else{
        char c = workbench::fetchid(n->metaObject()->className());
        if(strcmp(&c,"$")==0)
            return;
        out->append(c);
        QString size = "{"+QString::number(qobject_cast<QWidget*>(n)->size().width())
                +";"
                +QString::number(qobject_cast<QWidget*>(n)->size().height())
                +"}";

        out->append(size);
    }
}

QWidget *workbench::fetchwidget(const char *id){
    if(strcmp(id,"z")==0)
        return new dummywidget("Right-click anywhere to replace this.", this);
    else if(strcmp(id,"a")==0)
        return workbench::_libtree();
    else if(strcmp(id,"c")==0)
        return workbench::_playlistmgr();
    else if(strcmp(id,"e")==0)
        return workbench::_coverview();
    else if(strcmp(id,"d")==0)
        return workbench::_tagview();
    else{
        qWarning() << "[WARNING] switcheroo can't handle a '" << id << "'!";
        return new QWidget;
    }
}


QWidget* workbench::_vsplitter(bool filled){
    QSplitter *z = new QSplitter();
    z->setOrientation(Qt::Orientation::Vertical);
    z->setChildrenCollapsible(false);
    //connect(z, &QSplitter::splitterMoved, this, &workbench::refreshdb);
    if(filled){
        z->insertWidget(0, new dummywidget("[<b>VERTICAL SPLITTER</b> :: Widget #0]", this));
        z->insertWidget(1, new dummywidget("[<b>VERTICAL SPLITTER</b> :: Widget #1]", this));
    }

    return z;
}

void workbench::ctx_vsplitter(){
    this->inject(workbench::_vsplitter(true));
}

QWidget* workbench::_hsplitter(bool filled){
    QSplitter *z = new QSplitter();
    z->setOrientation(Qt::Orientation::Horizontal);
    z->setChildrenCollapsible(false);
    //connect(z, &QSplitter::splitterMoved, this, &workbench::refreshdb);
    if(filled){
        z->addWidget(new dummywidget("[<b>HORIZONTAL SPLITTER</b> :: Widget #0]", this));
        z->addWidget(new dummywidget("[<b>HORIZONTAL SPLITTER</b> :: Widget #1]", this));
    }

    return z;
}

void workbench::ctx_hsplitter(){
    this->inject(workbench::_hsplitter(true));
}

QWidget* workbench::_libtree(){
    libtree *nltree = new libtree(qobject_cast<mwindow*>(this->win), this);
    nltree->populate(this->jag->DB_REF);
    return nltree;
}

void workbench::ctx_libtree(){
    this->inject(workbench::_libtree());///////
}

QWidget* workbench::_playlistmgr(){
    return new playlistview(this->jag, qobject_cast<mwindow*>(win), this);
}

void workbench::ctx_playlistmgr(){
    this->inject(workbench::_playlistmgr());
}

QWidget* workbench::_coverview(){
    return new coverview(qobject_cast<mwindow*>(win), this);
}

void workbench::ctx_coverview(){
    this->inject(workbench::_coverview());
}

QWidget* workbench::_tagview(){
    return new tagview(this->jag, qobject_cast<mwindow*>(win), this);
}

void workbench::ctx_tagview(){
    this->inject(workbench::_tagview());
}

/* removes the parent of a widget located at ctx_lastpos */
/* --- apropriate checks should've already been made */
void workbench::remove_parentwidget(){
    QWidget *rem = this->lastctx->parentWidget();
    QWidget *parent = rem->parentWidget();
    if(parent==this){
        this->ml->replaceWidget(rem, new dummywidget("I'm just a placeholder.\nRight-click to replace me!", this));
        rem->~QWidget();
    }else{
        int pos = qobject_cast<QSplitter*>(parent)->indexOf(rem);
        qobject_cast<QSplitter*>(parent)->replaceWidget(pos, new dummywidget("I'm just a placeholder.\nRight-click to replace me!", this));
        rem->~QWidget();
    }
}

/* removes the widget at ctx_lastpos */
void workbench::remove_widget(){
    if(this==this->lastctx->parent()){
        QWidget *w = this->lastctx;
        this->ml->removeWidget(w);
        w->~QWidget();
    }else if(this->lastctx->parent()->metaObject()->className()==QString("QSplitter")){
        //QSplitter *par = qobject_cast<QSplitter*>(this->lastctx->parent());
        this->lastctx->~QWidget();
    }

    if(this->ml->children().count()==0)
        this->ml->addWidget(new dummywidget("First, unlock the widgets on the cogwheel toolbar button\nThen replace this by right-clicking on an empty space!", this));
}

/* removes all the widgets from the workbench's layout */
/* it seems that it will always be 3 children from workbench: QGridLayout, QPropertyAnimation, */
/* and whichever widget was first added. splitter's children do not show */
void workbench::clear(){
    foreach(QObject *o, this->children())
        if(o->metaObject()->className()!=QString("QGridLayout") &&
        o->metaObject()->className()!=QString("QPropertyAnimation"))
           o->~QObject();

    this->ml->addWidget(new dummywidget("<b>[WORKBENCH]</b> :: You can right-click anywhere to add something useful to it.", this));
    workbench::refreshdb();
}

