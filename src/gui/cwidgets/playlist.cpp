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

#include "playlist.hpp"

#include <QObject>
#include <QHeaderView>
#include <QMenu>
#include <QCollator>
#include <taglib/tpropertymap.h>
#include "playlistview.hpp"

playlist::playlist(QString *order, QMenu *headerctx, vars *jag, mwindow *win, QWidget *parent) : QTreeWidget (){
    this->order = order;
    this->view = qobject_cast<QTabWidget*>(parent);
    this->headerctx = headerctx;
    this->bodyctx = new QMenu();
    this->emptyctx = new QMenu();
    //this->playing = new QTreeWidgetItem();
    this->jag = jag;


    playlistview *p = qobject_cast<playlistview*>(parent);
    connect(this, &playlist::play, p, &playlistview::swapitem);

    QList<QAction*> *al = new QList<QAction*>();
    al->append(new QAction(QString("Clear playlist")));
    connect(al->back(), &QAction::triggered, this, &playlist::clearchildren);
    al->append(new QAction(QString("Clear Selection")));
    connect(al->back(), &QAction::triggered, this, &QTreeWidget::clearSelection);
    al->append(new QAction(QString("Export playlist")));
    connect(al->back(), &QAction::triggered, this, &playlist::exportpl);

    this->emptyctx->addActions(*al);
    al->~QList();

    this->setAlternatingRowColors(true);
    this->setSortingEnabled(true);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setRootIsDecorated(false);
    this->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);

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
    connect(this,
            &QTreeWidget::itemClicked,
            win,
            &mwindow::select);
}

playlist::~playlist(){}

/* shows the context menu for the wiget's header bar */
void playlist::show_headerctx(const QPoint & pos){
    QPoint *p = new QPoint(pos.x(),this->pos().y()+this->header()->height());
    this->headerctx->exec(this->mapToGlobal(*p));
    p->~QPoint();
}

/* this displays the context menu when right clicked inside the widget's body */
void playlist::show_bodyctx(const QPoint & pos){
    QPoint *p = new QPoint(pos.x(), pos.y()+this->header()->height());

    if(this->itemAt(pos)==nullptr){
        this->emptyctx->exec(this->mapToGlobal(*p));
    }else{
        this->bodyctx->exec(this->mapToGlobal(*p));
    }


    p->~QPoint();
}

void playlist::clearchildren(){
    this->pl.clear();
    this->clear();
    this->playing_index = 0;
    this->playing = nullptr;
    this->selected = nullptr;
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

                if(this->playing != nullptr)
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
        //this->EOP();
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

void playlist::exportpl(){
    /* get path from file picker */
    QString path = QFileDialog::getSaveFileName(
        this,
        "Exporting playlist '"+this->view->tabText(this->view->currentIndex()).remove("&")+"'",
        QDir::homePath(), QString("Playlists (*.m3u *.pls)")
    );

    if(path.isEmpty() || this->topLevelItemCount()==0)
        return;

    qInfo() << "[INFO] Exporting playlist...";
    QFile npl(path);
    npl.open(QIODevice::WriteOnly);

    /* check it opened ok */
    if(!npl.isOpen()){
        qFatal("%s", qPrintable(QString("[ERROR] Unable to open '"+path)+"' for output"));
    }

    /* point a QTextStream object at the file */
    QTextStream outStream(&npl);

    /* dump data to chosen file */
    outStream << "#EXTM3U\n\n";
    TagLib::FileRef *f;
    for(int i=0;i<this->topLevelItemCount();i++){
        f = new TagLib::FileRef(qPrintable(this->topLevelItem(i)->data(0, Qt::UserRole).toString()));
        outStream << "#EXTINF:"+QString::number(f->audioProperties()->lengthInSeconds())+", "+f->tag()->artist().toCString(true)+" - "+f->tag()->title().toCString(true);
        outStream << "\n"+this->topLevelItem(i)->data(0, Qt::UserRole).toString();
        outStream << "\n\n";
        f->~FileRef();
    }
    npl.close();

    qInfo() << qPrintable("  -> playlist exported to: "+path);
}

