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
#include "playlistview.hpp"
#include "src/mwindow.hpp"
#include "playlist.hpp"
#include "renamepbtn.hpp"

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

playlistview::playlistview(vars *jag, mwindow *parent) : QTabWidget(parent){
    this->jag = jag;
    this->win = parent;

    this->renamer = new QDialog(this);
    this->renamer->setFixedSize(480,48);
    this->renamer->setModal(true);
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
    connect(this, &QTabWidget::customContextMenuRequested, this, &playlistview::ctxmenu);
    connect(this, &QTabWidget::currentChanged, this, &playlistview::tab_switch);
    connect(parent, &mwindow::plappend, this, &playlistview::viewappend);
    connect(parent, &mwindow::plnext, this, &playlistview::next);
    connect(parent, &mwindow::plprev, this, &playlistview::prev);
    connect(parent, &mwindow::mediastatus, this, &playlistview::medistatus);
}

void playlistview::ctxmenu(const QPoint &pos){
    if(this->tabBar()->tabAt(pos)==-1) /* case there isnt a tab there */
        return;

    //connect(ctx->actions().first(), &QAction::triggered, this)///wronggggg
    ctx->exec(this->mapToGlobal(pos));
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

    //QModelIndex index = this->indexAt(p);
    //if (index.isValid() && index.row() % 2 == 0) {
        //hctx->popup(this->viewport()->mapToGlobal(p));
    //}
}

void playlistview::playing(QString f){
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
