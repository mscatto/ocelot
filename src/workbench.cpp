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
#include <QObject>
#include <QMediaPlayer>
#include <QMainWindow>
#include <QGridLayout>
#include <QSizePolicy>
#include <QWidget>
#include <QFrame>
#include <QTreeWidgetItem>
#include <QTreeWidget>

#include "workbench.hpp"
#include "library.hpp"
#include "mwindow.hpp"
#include "vars.hpp"

#include "cwidgets/coverview.hpp"
#include "cwidgets/libtree.hpp"
#include "cwidgets/tagview.hpp"
#include "cwidgets/playlistview.hpp"
#include "cwidgets/splitter.hpp"

workbench::workbench(vars *jag, QWidget *win) : QFrame(win){
    QGridLayout *grid = new QGridLayout();

    grid->setContentsMargins(0,0,0,0);
    this->setLayout(grid);
    this->setGeometry(win->rect());

    splitter *x = new splitter(Qt::Horizontal, this);
    splitter *y = new splitter(Qt::Vertical, this);
    splitter *z = new splitter(Qt::Horizontal, this);
    libtree *j = new libtree(qobject_cast<mwindow*>(win));
    j->populate(jag->DB_REF);
    //connect(j->tree, )
    x->addWidget(j);
    x->addWidget(y);

    y->addWidget(new playlistview(jag, qobject_cast<mwindow*>(win)));
    y->addWidget(z);

    z->addWidget(new coverview(qobject_cast<mwindow*>(win)));
    z->addWidget(new tagview(jag, qobject_cast<mwindow*>(win)));

    grid->addWidget(x);
    /* expand further...*/
}

workbench::~workbench(){

}
