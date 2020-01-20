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

#include "workbench.hpp"
#include "../library.hpp"
#include "../utils/topostfix.hpp"
#include "../vars.hpp"
#include "mwindow.hpp"

#include "cwidgets/coverview.hpp"
#include "cwidgets/dummywidget.hpp"
#include "cwidgets/libtree.hpp"
#include "cwidgets/playlistview.hpp"
#include "cwidgets/splitter.hpp"
#include "cwidgets/tagview.hpp"

#include <QByteArray>
#include <QDebug>
#include <QFrame>
#include <QGridLayout>
#include <QMainWindow>
#include <QObject>
#include <QSizePolicy>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QWidget>
#include <libprojectM/projectM.hpp>

workbench::workbench(vars* jag, QWidget* win) : QWidget() {
    /* private allocations */
    this->jag = jag;
    this->win = win;
    this->lastctx = this;
    this->ml = new QGridLayout();

	/* setters */
    this->setObjectName("workbench");
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setContentsMargins(0, 0, 0, 0);
    this->ml->setContentsMargins(0, 0, 0, 0);
    this->setLayout(this->ml);
    this->setGeometry(win->rect());

    /* checks if there's a layout stored. if there isnt one, present a blank workbench */
    QString tmp = this->jag->fetchdbdata("ui_scheme").toString();

    this->setlayout(&tmp);

    /* sets up the layout editor's replace context menu */
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

    /* connecting everything */
    QList<QAction*> act = this->ctx_replace->actions();
    connect(act.at(1), &QAction::triggered, this, &workbench::ctx_vsplitter);
    connect(act.at(2), &QAction::triggered, this, &workbench::ctx_hsplitter);
    connect(act.at(4), &QAction::triggered, this, &workbench::ctx_libtree);
    connect(act.at(6), &QAction::triggered, this, &workbench::ctx_playlistmgr);
    connect(act.at(7), &QAction::triggered, this, &workbench::ctx_tagview);
    connect(act.at(8), &QAction::triggered, this, &workbench::ctx_coverview);
    connect(this, &QWidget::customContextMenuRequested, this, &workbench::ctx_req);
}

workbench::~workbench() {
}

bool workbench::islocked() {
    return this->locked;
}

void workbench::setlastctx(QWidget* w) {
    this->lastctx = w;
}

/* this reparents *w to the widget under ctx_lastpos click */
void workbench::inject(QWidget* w) { /////////////////////////
    /* case it is the root widget */
    if(this == this->lastctx) {
        this->ml->addWidget(w);
        this->root = w;
    } else if(this == this->lastctx->parent()) {
        QWidget* old = this->lastctx;
        this->ml->removeWidget(old);
        this->ml->addWidget(w);
        // if(this->root == nullptr)
        this->root = w;
        old->~QWidget();
    }

    /* if it isnt, expect the parent to be a splitter */
    else if(this->lastctx->parent()->metaObject()->className() == QString("splitter")) {
        splitter* par = qobject_cast<splitter*>(this->lastctx->parent());
        int z = par->indexOf(this->lastctx);
        par->widget(z)->~QWidget();
        par->insertWidget(z, w);
    }
    /* panic */
    else {
        qWarning() << "[WARNING] at workbench::inject - " << w;
    }
    this->refreshdb();
}

/* refreshdb handles writing the current layout to the database, converting it to postfix */
void workbench::refreshdb() {
    QString s;
    QList<QSplitter*> spl;

    this->dumplayout(this->root, &s, &spl);

    QSqlQuery* q = new QSqlQuery(*this->jag->DB_REF);
    q->prepare("UPDATE data SET val = :scheme WHERE var LIKE 'ui_scheme'");
    q->bindValue(":scheme", translate(s.toStdString()).c_str());
    q->exec();

    q->~QSqlQuery();
}

/* this function purpose is to rebuild the UI according to the postfix code on *l */
void workbench::setlayout(QString* l) {
    if(l->isEmpty() || l->at(0) == "$" || l->at(0) == "0") {
        this->clear();
        return;
    }

    QWidgetList stack;
    while(l->length() > 0) {
        if(l->front() != "+" && l->front() != "-") { // checks if it is a widget
            stack.push_back(fetchwidget(qPrintable(l->at(0))));
            /*QStringList s(l->section("", 3, l->indexOf("]")).split(","));
            QSize size(s.first().toInt(), s.last().toInt());
            qInfo() << size;
            stack.back()->setBaseSize(size);*/

            l->remove(0, 1); // l->indexOf("]") + 1);
        } else {
            splitter* ns;
            if(l->front() == "-")
				ns = new splitter(Qt::Vertical, static_cast<mwindow*>(this->win), static_cast<mwindow*>(win)->wb);
            else
				ns = new splitter(Qt::Horizontal, static_cast<mwindow*>(this->win), static_cast<mwindow*>(win)->wb);

            // connect(ns, &QSplitter::splitterMoved, qobject_cast<mwindow*>(win), &mwindow::child_resized);

            ns->insertWidget(1, stack.back());
            stack.pop_back();
            ns->insertWidget(0, stack.back());
            stack.pop_back();

            stack.push_back(ns);
            l->remove(0, 1);
        }
    }
    this->ml->addWidget(stack.first());
    this->root = stack.first();
    qobject_cast<mwindow*>(this->win)->libchanged(this->jag->DB_REF);
}

char workbench::fetchid(QString objname) {
    if(objname == "libtree")
        return defwidgets::TREELIB;
    if(objname == "coverview")
        return defwidgets::COVERVIEW;
    if(objname == "tagview")
        return defwidgets::TAGVIEW;
    if(objname == "playlistview")
        return defwidgets::PLMGR;
    if(objname == "dummywidget")
        return defwidgets::DUMMY;
    else {
        qWarning() << "[WARNING] at workbench::fetchid: Unknown widget: " << objname;
        return '$';
    }
}

/* this one recurs down each splitter generating an infix expression representing the layout */
void workbench::dumplayout(QWidget* n, QString* out, QList<QSplitter*>* spl) {
    if(n->metaObject()->className() == QString("splitter")) { //////////////////////////
        spl->append(static_cast<QSplitter*>(n));
        out->append("(");
        workbench::dumplayout(qobject_cast<splitter*>(n)->widget(0), out, spl);
        if(qobject_cast<splitter*>(n)->orientation() == Qt::Orientation::Vertical)
            out->append(defwidgets::VSPLIT);
        else
            out->append(defwidgets::HSPLIT);
        workbench::dumplayout(qobject_cast<splitter*>(n)->widget(1), out, spl);
        out->append(")");
    } else {
        char c = workbench::fetchid(n->metaObject()->className());
        if(strcmp(&c, "$") == 0)
            return;
        out->append(c);
    }
}

QWidget* workbench::fetchwidget(const char* id) {
    if(strcmp(id, "z") == 0)
        return new dummywidget(this);
    else if(strcmp(id, "a") == 0)
        return workbench::_libtree();
    else if(strcmp(id, "c") == 0)
        return workbench::_playlistmgr();
    else if(strcmp(id, "e") == 0)
        return workbench::_coverview();
    else if(strcmp(id, "d") == 0)
        return workbench::_tagview();
    else {
		this->jag->PANIC("at fetchwidget: bad id");
        return new QWidget;
    }
}

/********
 * SLOTS
 * ******/
void workbench::ctx_req(QPoint p) {
    if(this->islocked())
        return;

    QMenu ctx;
    QString* s;
    ctx.addSection("Layout Editor");
    if(this->childAt(p) != nullptr) {
        s = new QString(this->childAt(p)->metaObject()->className());
        if(*s == "QLabel")
            *s = "placeholder";
        else if(*s == "splitter")
            *s = "splitter";
    } else
        s = new QString("emptiness");



    ctx.addMenu(this->ctx_replace);

    QAction* remobj = new QAction();
    if(this->childAt(p) == nullptr) {
        remobj->setText("Can't remove workbench");
        remobj->setEnabled(false);

    } else {
        remobj->setText("Remove " + *s);
    }
    connect(remobj, &QAction::triggered, this, &workbench::remove_widget);
	ctx.addSeparator();

	if(strcmp(this->lastctx->parentWidget()->metaObject()->className(), "splitter") == 0) {
		QAction* lock = new QAction();
		connect(lock, &QAction::triggered, this, &workbench::lock_widget);
		if(static_cast<splitter*>(this->lastctx->parentWidget())->orientation()==Qt::Horizontal){
			if(this->lastctx->maximumWidth()<5000) // meaning a maximum size has been set. very ugly way to do it
				lock->setText("Unlock horizontal size");
			else
				lock->setText("Lock horizontal size");
		}else{
			if(this->lastctx->maximumHeight()<5000) // meaning a maximum size has been set. very ugly way to do it
				lock->setText("Unlock vertical size");
			else
				lock->setText("Lock vertical size");
		}
		ctx.addAction(lock);

        QAction* rempar = new QAction();
        rempar->setText("Remove underlying splitter");
        connect(rempar, &QAction::triggered, this, &workbench::remove_parentwidget);
        ctx.addAction(rempar);
    }

    QAction* clear = new QAction();
    clear->setText("Clear workbench");
    connect(clear, &QAction::triggered, this, &workbench::clear);
    ctx.addAction(clear);

    ctx.exec(this->mapToGlobal(p));
}

void workbench::lock_flip() {
    this->locked = !this->locked;

    if(!this->locked)
        qInfo("%s", "[INFO] Layout editor enabled!");
    else
        qInfo("%s", "[INFO] Layout editor disabled!");
}

void workbench::ctx_vsplitter() {
    this->inject(workbench::_vsplitter(true));
}

void workbench::ctx_hsplitter() {
    this->inject(workbench::_hsplitter(true));
}

void workbench::ctx_libtree() {
    this->inject(workbench::_libtree());
}

void workbench::ctx_playlistmgr() {
    this->inject(workbench::_playlistmgr());
}

void workbench::ctx_coverview() {
    this->inject(workbench::_coverview());
}

void workbench::ctx_tagview() {
    this->inject(workbench::_tagview());
}

void workbench::clear() {
    this->root->~QWidget();
    this->root = new dummywidget(this);
    this->ml->addWidget(this->root);
	workbench::refreshdb();
}

void workbench::lock_widget(){
	if(strcmp(this->lastctx->parentWidget()->metaObject()->className(), "splitter") != 0)
		return this->jag->PANIC("at lock_widget: unexpected parent for widget to be locked");

	if(static_cast<splitter*>(this->lastctx->parentWidget())->orientation()==Qt::Horizontal){
		if(this->lastctx->maximumWidth()<5000) // meaning a maximum size has been set. very ugly way to do it
			this->lastctx->setFixedWidth(QWIDGETSIZE_MAX);
		else
			this->lastctx->setFixedWidth(this->lastctx->width());
	}else{
		if(this->lastctx->maximumHeight()<5000) // meaning a maximum size has been set. very ugly way to do it
			this->lastctx->setFixedHeight(QWIDGETSIZE_MAX);
		else
			this->lastctx->setFixedHeight(this->lastctx->height());
	}
}


/* PURE CRAP */

/* removes the parent of a widget located at ctx_lastpos */
/* --- apropriate checks should've already been made */
void workbench::remove_parentwidget() {
    QWidget* rem = this->lastctx->parentWidget();
    QWidget* parent = rem->parentWidget();
    if(parent == this) {
        this->ml->replaceWidget(rem, new dummywidget(this));
        rem->~QWidget();
    } else {
        int pos = qobject_cast<splitter*>(parent)->indexOf(rem);
        qobject_cast<splitter*>(parent)->replaceWidget(pos, new dummywidget(this));
        rem->~QWidget();
    }
}

/* removes the widget at ctx_lastpos */
void workbench::remove_widget() {
    if(this == this->lastctx->parent()) {
        QWidget* w = this->lastctx;
        this->ml->removeWidget(w);
        w->~QWidget();
    } else if(this->lastctx->parent()->metaObject()->className() == QString("splitter")) {
        // QSplitter *par = qobject_cast<QSplitter*>(this->lastctx->parent());
        this->lastctx->~QWidget();
    }

    if(this->ml->children().count() == 0)
        this->ml->addWidget(new dummywidget(this));
}

/*************
 * ALLOCATORS
 * ***********/
QWidget* workbench::_vsplitter(bool filled) {
	splitter* ns = new splitter(Qt::Vertical, static_cast<mwindow*>(this->win), static_cast<mwindow*>(win)->wb);
    // ns->setObjectName("wbsplitter");
    // connect(ns->handle(), &QSplitterHandle::, qobject_cast<mwindow*>(win), &mwindow::child_resized);
    if(filled) {
        ns->insertWidget(0, new dummywidget(this, "<b>:: PLACEHOLDER ::</b><br>[VERTICAL SPLITTER - Widget #0]"));
        ns->insertWidget(1, new dummywidget(this, "<b>:: PLACEHOLDER ::</b><br>[VERTICAL SPLITTER - Widget #1]"));
    }

    return ns;
}

QWidget* workbench::_hsplitter(bool filled) {
	splitter* ns = new splitter(Qt::Horizontal, static_cast<mwindow*>(this->win), static_cast<mwindow*>(win)->wb);
    // ns->setObjectName("wbsplitter");

    // connect(ns, &splitter::splitterMoved, qobject_cast<mwindow*>(win), &mwindow::child_resized);
    if(filled) {
        ns->addWidget(new dummywidget(this, "<b>:: PLACEHOLDER ::</b><br>[HORIZONTAL SPLITTER - Widget #0]"));
        ns->addWidget(new dummywidget(this, "<b>:: PLACEHOLDER ::</b><br>[HORIZONTAL SPLITTER - Widget #1]"));
    }

    return ns;
}

QWidget* workbench::_libtree() {
    libtree* nltree = new libtree(qobject_cast<mwindow*>(this->win), this, this->jag);
    nltree->populate(this->jag->DB_REF);
    return nltree;
}

QWidget* workbench::_coverview() {
    return new coverview(qobject_cast<mwindow*>(win), this);
}

QWidget* workbench::_tagview() {
    return new tagview(this->jag, qobject_cast<mwindow*>(win), this);
}

QWidget* workbench::_playlistmgr() {
    return new playlistview(this->jag, qobject_cast<mwindow*>(win), this);
}
