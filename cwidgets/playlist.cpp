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
#include "playlist.hpp"
#include "mwindow.hpp"
#include <qheaderview.h>
#include <QMenu>
playlist::playlist(mwindow *parent) : QTreeWidget(parent){
    this->setAlternatingRowColors(true);
    this->header()->setContextMenuPolicy(Qt::CustomContextMenu);
    this->hctx = new QMenu("Column Editor");
    this->hctx->addSeparator();
    this->hctx->addAction(new QAction("Niggers"));
    this->hctx->addAction(new QAction("Wiggaz"));
    connect(parent, &mwindow::plappend, this, &playlist::append);
    connect(this->header(), &QHeaderView::customContextMenuRequested, this, &playlist::headerctx);
    connect(this, &QTreeWidget::itemDoubleClicked, parent, &mwindow::play);
}

playlist::~playlist(){

}

void playlist::append(QStringList f){
    while(f.length()>0){
        this->addTopLevelItem(new QTreeWidgetItem(this, QStringList(f.first())));
        f.pop_front();
    }
}

void playlist::headerctx(QPoint p){
    qDebug() << p;
    //QModelIndex index = this->indexAt(p);
    //if (index.isValid() && index.row() % 2 == 0) {
        hctx->popup(this->viewport()->mapToGlobal(p));
    //}
}
