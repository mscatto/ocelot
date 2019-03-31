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

#include "playlistview.hpp"
#include "src/mwindow.hpp"
#include "playlist.hpp"
#include "renamepbtn.hpp"

#include <QSqlQuery>
#include <QHeaderView>
#include <QPushButton>
#include <QGridLayout>
#include <QMenu>
#include <QIcon>
#include <QSqlRecord>
#include <QToolButton>
#include <QMap>
#include <QTabBar>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QString>
#include <taglib/tag.h>
#include <taglib/tpropertymap.h>
#include <taglib/tlist.h>
#include <QFocusEvent>

playlistview::playlistview(vars *jag, mwindow *win, workbench *wb) : QTabWidget(){
    this->jag = jag;
    this->win = win;
    this->wb = wb;
    this->setContentsMargins(0,0,0,0);
    this->renamer = new QDialog(this);
    this->renamer->setFixedSize(480,48);
    this->renamer->setModal(true);
    this->setObjectName("playlistview");
    QGridLayout *grid = new QGridLayout();
    QDialogButtonBox *bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Close);
    this->namebox = new QLineEdit();
    namebox->setPlaceholderText("Enter a new name here");
    grid->addWidget(namebox,0,0,1,1);
    grid->addWidget(bb,0,1,1,1);
    this->renamer->setLayout(grid);

    connect(bb, &QDialogButtonBox::accepted, this, &playlistview::renamer_ok);
    connect(bb, &QDialogButtonBox::rejected, this, &playlistview::renamer_cancel);

    /* fetches order from database */
    QSqlQuery *q = new QSqlQuery();
    q->exec("SELECT val FROM data WHERE var='playlist_columnorder'");
    q->next();
    this->order.clear();
    this->order.append(q->record().value(0).toString());
    q->~QSqlQuery();

    /* the tab context */
    this->ctx = new QMenu();

    ctx->addAction("Rename playlist...");
    ctx->addSeparator();
    ctx->addAction(new QAction(QString("Clear playlist")));
    connect(ctx->actions().last(), &QAction::triggered, this, &playlistview::clearpl);
    ctx->addAction(new QAction(QString("Clear Selection")));
    connect(ctx->actions().last(), &QAction::triggered, this, &playlistview::clearsel);
    ctx->addAction(new QAction(QString("Export playlist")));
    connect(ctx->actions().last(), &QAction::triggered, this, &playlistview::exportpl);

    /* then sets up header' context menu */
    QList<QAction*> *ctx = new QList<QAction*>();

    ctx->append(new QAction("#"));
    ctx->last()->setCheckable(true);
    if(this->order.contains("#INDEX#"))
        ctx->last()->toggle();
    connect(ctx->last(), &QAction::toggled, this, &playlistview::toggler);

    foreach(QString s, this->jag->dumpkeys()){
        ctx->append(new QAction(this->jag->translate_key(s)));
        ctx->last()->setCheckable(true);
        if(this->order.contains(s))
            ctx->last()->toggle();
        connect(ctx->last(), &QAction::toggled, this, &playlistview::toggler);
    }

    this->headermenu = new QMenu();
    headermenu->addActions(*ctx);
    ctx->~QList();

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    /* insert default playlist*/
    this->newplaylist();

    /* set up new tab button*/
    this->addbtn = new QToolButton();
    this->addbtn->setText("+");
    this->addbtn->setFixedSize(28,28);
    connect(this->addbtn, &QToolButton::clicked, this, &playlistview::newplaylist);
    this->setCornerWidget(this->addbtn, Qt::Corner::TopRightCorner);

    this->setMovable(true);
    this->setTabsClosable(true);

    connect(this, &QTabWidget::tabCloseRequested, this, &playlistview::tab_close);
    connect(this, &QTabWidget::customContextMenuRequested, this, &playlistview::showctx);
    connect(this, &QTabWidget::currentChanged, this, &playlistview::tab_switch);
    connect(win, &mwindow::plappend, this, &playlistview::viewappend);
    connect(win, &mwindow::plnext, this, &playlistview::next);
    connect(win, &mwindow::plprev, this, &playlistview::prev);
    connect(win, &mwindow::mediastatus, this, &playlistview::medistatus);
}

void playlistview::showctx(const QPoint &pos){
    if(this->wb->islocked())
        this->ctx->exec(this->mapToGlobal(pos));
    else{
        if(this->tabBar()->tabAt(pos)==-1) /* case there isnt a tab there */
            return;
        this->wb->setlastctx(this);
        this->wb->ctx_req(this->mapTo(this->wb, pos));
    }
}

void playlistview::newplaylist(){
    int i=1;
    while(this->pl.keys().contains("New Playlist "+QString::number(i))){
        i++;
    }
    QString name = "New Playlist "+QString::number(i);
    renamepbtn *ren = new renamepbtn(QIcon::fromTheme("edit-rename"),name);

    ren->setMaximumSize(18,18);
    ren->setFlat(true);//setStyleSheet("QPushButton { border : none }");
    ren->setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    connect(ren, &renamepbtn::idclicked, this, &playlistview::tab_rename);

    pl.insert(name, new playlist(&this->order, this->headermenu, jag, win, this));
    connect(this, &playlistview::exportpl, pl.value(name), &playlist::exportpl);
    connect(this, &playlistview::clearsel, pl.value(name), &playlist::clearSelection);
    connect(this, &playlistview::clearpl, pl.value(name), &playlist::clearchildren);
    this->plactive = pl.value(name);
    this->insertTab(this->count(), pl.value(name), name);
    this->setCurrentIndex(this->count()-1);
    this->tabBar()->setTabButton(this->currentIndex(), QTabBar::ButtonPosition::LeftSide, ren);

}


void playlistview::context(QPoint p){
    p = QPoint(); /* erase me¨ */
    //QModelIndex index = this->indexAt(p);
    //if (index.isValid() && index.row() % 2 == 0) {
        //hctx->popup(this->viewport()->mapToGlobal(p));
    //}
}

void playlistview::playing(QString f){
    f="0"; /* erase this one¨ */
    /*foreach(QStringList *sl, this->pl->values()){
        if(sl->contains(f)){
            QTreeWidgetItem *x = this->c
            qobject_cast<QTreeWidget*>(this->children().at(sl->indexOf(f)))->setData(0, Qt::UserRole, this->playchar);//->

        }
    }*/
}

void playlistview::refreshpl(){
    foreach(playlist *p, this->pl){
        p->rebuild_columns();
    }
}

void playlistview::next(){
    this->plactive->next();
}

void playlistview::prev(){
    this->plactive->prev();
}

void playlistview::medistatus(QMediaPlayer::MediaStatus status){
    status = QMediaPlayer::MediaStatus::EndOfMedia; /* erase me¨ */
    //qDebug() << status;
}

void playlistview::swapitem(QTreeWidgetItem *item){
    /* undo stuff */
    //qDebug() << "lel";
    this->currentitem = item;
    /* redo stuff */

}

void playlistview::toggler(bool checked){
    this->order.clear();
    //this->order.append(this->playchar+QString(";"));

    foreach(QAction *a, this->headermenu->actions())
        if(a->isChecked())
            this->order.append(this->jag->translate_val(a->text())+";");
    qDebug() << this->order;

    this->order.chop(1);
    this->refreshpl();

    if(checked)return;/* erase me¨ */
}

void playlistview::tab_close(int index){
    if(this->count()<=1)
        return;
    if(index==0)
        this->tab_switch(1);
    else
        this->tab_switch(index-1);

    this->pl.remove(this->tabText(index).remove("&"));
    this->removeTab(index);
}

void playlistview::tab_switch(int index){
    this->plactive = this->pl.value(this->tabText(index).remove("&"));
}

void playlistview::tab_rename(QString key){
    this->renamer->setWindowTitle("Renaming '"+key+"'");
    this->renamer->show();
    tmpkey = key;
}

void playlistview::renamer_ok(){
    this->pl.insert(this->namebox->text(), this->pl.value(this->tmpkey));
    this->pl.remove(tmpkey);

    /* just awful */
    for(int i=0; i<this->tabBar()->count();i++)
        if(this->tabBar()->tabText(i).remove("&")==tmpkey){
            this->tabBar()->setTabText(i, this->namebox->text());
            qobject_cast<renamepbtn*>(this->tabBar()->tabButton(i, QTabBar::ButtonPosition::LeftSide))->set_key(this->namebox->text());
        }

    tmpkey.clear();
    this->renamer->close();
    this->namebox->clear();
}

void playlistview::renamer_cancel(){
    this->renamer->close();
    this->namebox->clear();
}

void playlistview::viewappend(QStringList f){
    this->plactive->append(f);
}

playlistview::~playlistview(){

}
