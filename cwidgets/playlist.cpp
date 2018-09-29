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
#include "src/mwindow.hpp"
#include <qheaderview.h>
#include <QGridLayout>
#include <QListWidget>
#include <QPushButton>
#include <QMenu>
#include <QIcon>
#include <taglib/tag.h>
#include <taglib/tpropertymap.h>

playlist::playlist(mwindow *parent) : QTreeWidget(parent){
    this->setAlternatingRowColors(true);
    this->header()->setContextMenuPolicy(Qt::CustomContextMenu);
    this->config = new QDialog(this);

    config->setModal(false);
    config->setFixedSize(QSize(480, 320));
    config->setMaximumSize(QSize(480,320));
    config->setWindowTitle("Column Editor");
    config->setSizeGripEnabled(false);
    //config->setSizePolicy(0);

    QGridLayout *l = new QGridLayout;
    l->setSizeConstraint(QLayout::SetFixedSize);
    l->setSpacing(8);
    l->setMargin(8);
    config->setLayout(l);

    QTreeWidget *avail = new QTreeWidget(this);
    QTreeWidget *picked = new QTreeWidget(this);

    avail->setHeaderLabel("Available columns");
    avail->setSelectionBehavior(QAbstractItemView::SelectRows);
    avail->setSelectionMode(QAbstractItemView::ExtendedSelection);
    avail->setDragDropMode(QAbstractItemView::DragOnly);

    picked->setHeaderLabel("Columns shown");
    picked->setDragDropMode(QAbstractItemView::DragOnly);
    picked->setSelectionBehavior(QAbstractItemView::SelectRows);
    picked->setSelectionMode(QAbstractItemView::ExtendedSelection);

    QPushButton *bleft = new QPushButton(QIcon::fromTheme("arrow-left"), "");
    QPushButton *bright = new QPushButton(QIcon::fromTheme("arrow-right"), "");

    //bleft->setFixedHeight(260);
    bleft->setToolTip("Remove from shown columns");
    //bright->setFixedHeight(260);
    bright->setToolTip("Move to shown columns");

    l->setSizeConstraint(QLayout::SizeConstraint::SetMaximumSize);
    l->setAlignment(bleft, Qt::AlignBottom);

    l->addWidget(avail, 0,0,4,1);
    l->addWidget(bleft, 1,1,1,1);
    l->addWidget(bright,2,1,1,1);
    l->addWidget(picked,0,2,4,1);

    connect(parent, &mwindow::plappend, this, &playlist::append);
    connect(this->header(), &QHeaderView::customContextMenuRequested, this, &playlist::context);
    connect(this, &QTreeWidget::itemDoubleClicked, parent, &mwindow::play);
}

playlist::~playlist(){

}

void playlist::spawn_config(){
    this->config->show();
}

void playlist::append(QStringList f){
    while(f.length()>0){
        this->addTopLevelItem(new QTreeWidgetItem(this, QStringList(f.first())));
        f.pop_front();
    }
}

void playlist::context(QPoint p){
    QMenu *ctx = new QMenu("Column Editor");
    QAction *config = new QAction("Configure columns...");
    ctx->addAction(config);
    connect(config, &QAction::triggered, this, &playlist::spawn_config);

    QPoint pt(p);
    ctx->exec(this->mapToGlobal(p));
    //QModelIndex index = this->indexAt(p);
    //if (index.isValid() && index.row() % 2 == 0) {
        //hctx->popup(this->viewport()->mapToGlobal(p));
    //}
}
