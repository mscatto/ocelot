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

#include <QHeaderView>
#include <QMenu>
#include <taglib/tpropertymap.h>

playlist::playlist(QString *order, QMenu *headerctx, vars *jag, mwindow *win) : QTreeWidget (){
    this->order = order;
    this->headerctx = headerctx;
    this->bodyctx = new QMenu();
    this->jag = jag;

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
            win,
            &mwindow::play);
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
    while(f.length()>0 && !playlist::contains(f.first())){

        TagLib::FileRef *fr;
        fr = new TagLib::FileRef(qPrintable(f.first()));

        QStringList tagbank;
        QMap<QString, QString> pr;
        QStringList lst;
        tagbank.append(QString(fr->tag()->properties().toString().toCString(true)).split("\n", QString::SkipEmptyParts));

        foreach(QString s, tagbank){
            lst << s.split("=", QString::SkipEmptyParts);
            pr.insert(lst.first(), lst.last());
            lst.clear();
        }

        QTreeWidgetItem *ni = new QTreeWidgetItem(this, QStringList(f.first()));
        QStringList data;
        int z = 0;
        foreach(QString s, this->order->split(";", QString::SkipEmptyParts)){
            if(s=="#INDEX#")
                ni->setData(z, Qt::EditRole,QString::number(this->pl.length())); /* me toooooooooooooooooooo */
            else
                ni->setData(z, Qt::EditRole,pr.value(s));
            z++;
        }


        ni->setData(0, Qt::UserRole, f.first());
        this->insertTopLevelItem(this->children().count(), ni);
        //this->addTopLevelItem(ni);
        this->pl.append(f); /* look at meeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee */
        fr->~FileRef();
        f.pop_front();

        //this->
    }
}

