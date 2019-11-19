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

#include "playlistview.hpp"
#include <QCollator>
#include <QHeaderView>
#include <QMenu>
#include <QObject>
#include <taglib/tpropertymap.h>

playlist::playlist(QString* order, QMenu* headerctx, vars* jag, mwindow* win, QWidget* parent) : QTreeWidget() {
    this->jag = jag;
    this->order = order;
    this->win = win;
    this->view = qobject_cast<playlistview*>(parent);
    this->headerctx = headerctx;
    this->bodyctx = new QMenu();
    this->emptyctx = new QMenu();

    QList<QAction*> al;
    al.append(new QAction(QString("Clear playlist")));
    connect(al.back(), &QAction::triggered, this, &playlist::clearchildren);
    al.append(new QAction(QString("Clear Selection")));
    connect(al.back(), &QAction::triggered, this, &QTreeWidget::clearSelection);
    al.append(new QAction(QString("Export playlist")));
    connect(al.back(), &QAction::triggered, this, &playlist::exportpl);
    this->emptyctx->addActions(al);

    this->setAlternatingRowColors(true);
    this->setSortingEnabled(true);
    this->setRootIsDecorated(false);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
    this->rebuild_columns();

    this->header()->setContextMenuPolicy(Qt::CustomContextMenu);
    this->header()->setFirstSectionMovable(false);
    this->header()->resizeSection(0, 40);
    this->header()->resizeSection(1, 40);

    connect(this->header(), &QHeaderView::customContextMenuRequested, this, &playlist::show_headerctx);
    connect(this, &playlist::play, qobject_cast<playlistview*>(parent), &playlistview::swapitem);
    connect(this, &playlist::customContextMenuRequested, this, &playlist::show_bodyctx);
    connect(this, &QTreeWidget::itemDoubleClicked, this, &playlist::doubleclick);
    connect(this, &playlist::play, win, &mwindow::toolbar_play);
    connect(this, &playlist::EOP, win, &mwindow::toolbar_stop);
    connect(this, &QTreeWidget::itemClicked, win, &mwindow::select);
}

void playlist::showctx(const QPoint& pos, bool is_header) {
    if(this->win->wb->islocked()) {
        if(is_header)
            this->headerctx->exec(this->mapToGlobal(QPoint(pos.x(), this->pos().y() + this->header()->height())));
        else {
            QPoint p = QPoint(pos.x(), pos.y() + this->header()->height());

            if(this->itemAt(pos) == nullptr)
                this->emptyctx->exec(this->mapToGlobal(p));
            else
                this->bodyctx->exec(this->mapToGlobal(p));
        }
    } else {
        this->win->wb->setlastctx(this);
        this->win->wb->ctx_req(this->mapTo(this->win->wb, pos));
    }
}


/* shows the context menu for the wiget's header bar */
void playlist::show_headerctx(const QPoint& pos) {
    this->showctx(pos, true);
}

/* this displays the context menu when right clicked inside the widget's body */
void playlist::show_bodyctx(const QPoint& pos) {
    this->showctx(pos, false);
}

void playlist::clearchildren() {
    this->pl.clear();
    this->clear();
    this->playing = 0;
    this->selected = 0;
}

void playlist::rebuild_columns() {
    QStringList* c = new QStringList;
    c->append(this->order->split(";"));
    this->setColumnCount(c->length());

    c->replace(0, QString(this->playchar));

    for(int i = 1; i < c->length(); i++) {
        if(c->at(i) == "#INDEX#") /* only index atm */
            c->replace(i, QString("#"));
        else
            c->replace(i, this->jag->translate_key(c->at(i)));
    }
    this->setHeaderLabels(*c);
    this->header()->setSortIndicator(1, Qt::SortOrder::AscendingOrder);
    c->~QStringList();
}

bool playlist::contains(QString *path) {
    foreach(QTreeWidgetItem *i, this->pl) {
        if(i->data(0, Qt::UserRole).toString() == *path)
            return true;
    }

    return false;
}

void playlist::media_dispatch(QTreeWidgetItem* item) {
    emit this->win->player_stop();
    emit this->win->player_set(item->data(0, Qt::UserRole).toString());
    emit this->win->player_play();
}

void playlist::replace(const QStringList files) {
    this->clearchildren();
    this->pl.clear();
    this->append(files);
}

void playlist::append(QStringList files) {
    this->setSortingEnabled(false);
    QTreeWidgetItem *ni;
    if(this->pl.length()!=0){
        this->pl.at(this->playing-1)->setData(0,Qt::EditRole,this->queuechar);
    }

    if(this->contains(&files.front())){

    }else{
        ni = this->gen_treeitem(files.front().toUtf8());
        this->insertTopLevelItem(0, ni);
        this->pl.append(ni);
        this->playing = this->pl.length();
        this->pl.at(this->playing-1)->setData(0,Qt::EditRole,this->playchar);
        files.pop_front();
    }

    if(files.length()!=0)
        foreach(QString s, files) {
            if(!this->contains(&s)){
                ni = this->gen_treeitem(s.toUtf8());
                this->insertTopLevelItem(0, ni);
                this->pl.append(ni);
            }
        }

    this->setSortingEnabled(true);
    this->sortColumn();
}

QTreeWidgetItem* playlist::gen_treeitem(const char* const file) {
    TagLib::FileRef ref = TagLib::FileRef(file);
    QTreeWidgetItem* nitem;

    QStringList tagbank;
    QMap<QString, QString> pr;
    QStringList lst;
    qInfo()<<file;
    tagbank.append(QString(ref.tag()->properties().toString().toCString(true)).split("\n", QString::SkipEmptyParts));

    foreach(QString s, tagbank) {
        lst << s.split("=", QString::SkipEmptyParts);
        pr.insert(lst.first(), lst.last());
        lst.clear();
    }

    nitem = new QTreeWidgetItem(this, QStringList(file));
    QStringList data;
    int z = 0;
    foreach(QString s, this->order->split(";", QString::SkipEmptyParts)) {
        if(s == "#INDEX#")
            nitem->setData(z, Qt::EditRole, this->pl.length()); // me toooooooooooooooooooo // //----------
        else {
            bool ok;
            int num = s.toInt(&ok);
            //qInfo() << s << pr.value(s);
            if(ok)
                nitem->setData(z, Qt::EditRole, num);
            else
                nitem->setData(z, Qt::EditRole, pr.value(s));
        }
        z++;
    }

    nitem->setData(0, Qt::UserRole, file);
    nitem->setData(0, Qt::EditRole, this->queuechar);

    return nitem;
}

void playlist::next() {
    if(this->pl.length()==0)
            return QToolTip::showText(QCursor::pos(), "Playlist empty");

    if(this->playing - 1 >= this->pl.length() - 1) /* case the end of playlist */
        return QToolTip::showText(QCursor::pos(), "End of playlist");


    this->pl.at(this->playing-1)->setData(0,Qt::EditRole,this->queuechar);
    this->playing++;
    this->pl.at(this->playing-1)->setData(0,Qt::EditRole,this->playchar);
    this->media_dispatch(this->pl.at(this->playing-1));
}

void playlist::prev() {
    if(this->pl.length()==0)
        return QToolTip::showText(QCursor::pos(), "Playlist empty");

    if(this->playing - 1 <= 0) /* case the beginning of playlist */
        return QToolTip::showText(QCursor::pos(), "Beginning of playlist");

    this->pl.at(this->playing-1)->setData(0,Qt::EditRole,this->queuechar);
    this->playing--;
    this->pl.at(this->playing-1)->setData(0,Qt::EditRole,this->playchar);
    this->media_dispatch(this->pl.at(this->playing-1));
}

void playlist::doubleclick(QTreeWidgetItem* item) {
    this->pl.at(this->playing-1)->setData(0,Qt::EditRole,this->queuechar);
    this->playing = this->pl.indexOf(item)+1;
    this->pl.at(this->playing-1)->setData(0,Qt::EditRole,this->playchar);
    this->media_dispatch(item);

}

void playlist::exportpl() {
    /* get path from file picker */
    QString path
    = QFileDialog::getSaveFileName(this, "Exporting playlist '" + this->view->tabText(this->view->currentIndex()).remove("&") + "'",
                                   QDir::homePath(), QString("Playlists (*.m3u *.pls)"));

    if(path.isEmpty() || this->topLevelItemCount() == 0)
        return;

    qInfo() << "[INFO] Exporting playlist...";
    QFile npl(path);
    npl.open(QIODevice::WriteOnly);

    /* check it opened ok */
    if(!npl.isOpen()) {
        qFatal("%s", qPrintable(QString("[ERROR] Unable to open '" + path) + "' for output"));
    }

    /* point a QTextStream object at the file */
    QTextStream outStream(&npl);

    /* dump data to chosen file */
    outStream << "#EXTM3U\n\n";
    TagLib::FileRef* f;
    for(int i = 0; i < this->topLevelItemCount(); i++) {
        f = new TagLib::FileRef(qPrintable(this->topLevelItem(i)->data(0, Qt::UserRole).toString()));
        outStream << "#EXTINF:" + QString::number(f->audioProperties()->lengthInSeconds()) + ", "
                     + f->tag()->artist().toCString(true) + " - " + f->tag()->title().toCString(true);
        outStream << "\n" + this->topLevelItem(i)->data(0, Qt::UserRole).toString();
        outStream << "\n\n";
        f->~FileRef();
    }
    npl.close();

    qInfo() << qPrintable("  -> playlist exported to: " + path);
}
