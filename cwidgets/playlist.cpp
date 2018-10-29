/*
 * Ocelot Music Manager: A music player and library manager built using Qt
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
#include "playlist.hpp"

#include <QObject>
#include <QHeaderView>
#include <QMenu>
#include <QCollator>
#include <taglib/tpropertymap.h>
#include <cwidgets/playlistview.hpp>

playlist::playlist(QString *order, QMenu *headerctx, vars *jag, mwindow *win, QWidget *parent) : QTreeWidget (){
    this->order = order;
    this->headerctx = headerctx;
    this->bodyctx = new QMenu();
    this->playing = new QTreeWidgetItem();
    this->jag = jag;

    playlistview *p = qobject_cast<playlistview*>(parent);
    connect(this, &playlist::play, p, &playlistview::swapitem);

    QList<QAction*> *al = new QList<QAction*>();
    al->append(new QAction(QString("Clear playlist")));
    connect(al->back(), &QAction::triggered, this, &playlist::clear);
    al->append(new QAction(QString("Remove selected")));

    al->append(new QAction(QString("Export playlist")));

    this->bodyctx->addActions(*al);
    al->~QList();

    this->setAlternatingRowColors(true);
    this->setSortingEnabled(true);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setRootIsDecorated(false);
    this->setSelectionMode(QAbstractItemView::SelectionMode::MultiSelection);

    this->header()->setContextMenuPolicy(Qt::CustomContextMenu);
    this->header()->setFirstSectionMovable(false);

    this->rebuild_columns();

    this->header()->resizeSection(0, 40);
    this->header()->resizeSection(1, 40);

    connect(this,
            &playlist::customContextMenuRequested,
            this,
            &playlist::show_bodyctx);

    connect(this->header(),
            &QHeaderView::customContextMenuRequested,
            this,
            &playlist::show_headerctx);

    connect(this,
            &QTreeWidget::itemDoubleClicked,
            this,
            &playlist::doubleclick);
    connect(this,
            &playlist::play,
            win,
            &mwindow::play);
    connect(this,
            &playlist::EOP,
            win,
            &mwindow::toolbar_stop);
}

playlist::~playlist(){}

void playlist::show_headerctx(const QPoint & pos){
    /*QMenu *ctx = new QMenu("Columns");
    ctx->addActions(this->headerctx->actions()values());
    //qDebug() << this->menuctx.values();*************************************************/

    this->headerctx->exec(this->mapToGlobal(pos));
}

void playlist::show_bodyctx(const QPoint & pos){
    this->bodyctx->exec(this->mapToGlobal(pos));
}

void playlist::clear(){

}

void playlist::rebuild_columns(){
    QStringList *c = new QStringList;
    c->append(this->order->split(";"));
    this->setColumnCount(c->length());

    c->replace(0, QString(this->playchar));

    for(int i=1; i<c->length(); i++){
        if(c->at(i) == "#INDEX#") /* only index atm */
            c->replace(i, QString("#"));
        else
            c->replace(i, this->jag->translate_key(c->at(i)));}
    this->setHeaderLabels(*c);
    this->header()->setSortIndicator(1,Qt::SortOrder::AscendingOrder);
    c->~QStringList();
}

bool playlist::contains(QString path){
    foreach(QString s, this->pl){
        if(s==path)
            return true;
    }

    return false;
}

void playlist::append(QStringList f){
    TagLib::FileRef *fr;
    QTreeWidgetItem *nitem;

    QCollator collator;
    collator.setNumericMode(true);
    std::sort(f.begin(), f.end(),collator);
    //f.sort(collator);
    /* this will enforce that the first item will be playing */
    bool fuse = true;
    //f.sort(Qt::CaseInsensitive);
    foreach(QString track, f){
        if(!this->pl.contains(track)){
            this->pl.append(track); /* look at meeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee */
            fr = new TagLib::FileRef(qPrintable(track));

            QStringList tagbank;
            QMap<QString, QString> pr;
            QStringList lst;
            tagbank.append(QString(fr->tag()->properties().toString().toCString(true)).split("\n", QString::SkipEmptyParts));

            foreach(QString s, tagbank){
                lst << s.split("=", QString::SkipEmptyParts);
                pr.insert(lst.first(), lst.last());
                lst.clear();
            }

            nitem = new QTreeWidgetItem(this, QStringList(track));
            QStringList data;
            int z = 0;
            foreach(QString s, this->order->split(";", QString::SkipEmptyParts)){
                if(s=="#INDEX#")
                    nitem->setData(z, Qt::EditRole,this->pl.length()); /* me toooooooooooooooooooo *///----------
                else
                    nitem->setData(z, Qt::EditRole,pr.value(s));
                z++;
            }

            /* full path to file */
            nitem->setData(0, Qt::UserRole, track);
            //this->topl
            this->insertTopLevelItem(this->topLevelItemCount(), nitem);
            //this->addTopLevelItem(ni);

            if(fuse){
                fuse = false;

                /* TODO option to not append playing */
                this->clearSelection();
                this->setItemSelected(nitem,true);
                this->playing->setData(0, Qt::EditRole,".");
                this->playing = nitem;
                this->playing->setData(0, Qt::EditRole,this->playchar);
                this->playing_index++;
            }
            fr->~FileRef();
        }else{
            this->clearSelection();
            this->setItemSelected(this->playing, true);
        }
    }
}

void playlist::next(){
    if(this->playing_index-1 >= this->pl.length()-1){ /* case the end of playlist */
        this->EOP();
        return;
    }

    this->playing->setData(0, Qt::EditRole,"·");
    this->playing = this->itemBelow(this->topLevelItem(this->playing_index-1));
    this->playing->setData(0, Qt::EditRole,this->playchar);
    this->play(this->playing);
    this->playing_index++;

    this->clearSelection();
    this->setItemSelected(this->playing,true);
}

void playlist::prev(){
    if(this->playing_index-1 <= 0) /* case the first of playlist */
        return;

    this->playing->setData(0, Qt::EditRole,"·");
    this->playing = this->itemAbove(this->topLevelItem(this->playing_index-1));
    this->playing->setData(0, Qt::EditRole,this->playchar);
    this->play(this->playing);
    this->playing_index--;

    this->clearSelection();
    this->setItemSelected(this->playing,true);
}

void playlist::doubleclick(QTreeWidgetItem *item){
    this->playing->setData(0, Qt::EditRole,"·");
    this->playing = item;
    this->playing->setData(0, Qt::EditRole,this->playchar);
    this->play(this->playing);

    //this->playing_index = this->indexOfTopLevelItem(this->playing);
}

