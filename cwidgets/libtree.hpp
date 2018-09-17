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
#ifndef LIBTREE_HPP
#define LIBTREE_HPP

#include <QFrame>
#include <QObject>
#include <QWidget>
#include <QTreeWidget>
#include <QPushButton>
#include <library.hpp>

#include "mwindow.hpp"

class libtree : public QFrame
{
private:
    QTreeWidget *tree;
    void recurtree(QTreeWidgetItem *parent, QStringList levels, QString conditions, QSqlDatabase *db);
    QStringList *extract(QString vars);
public:
    libtree(mwindow *win);
    void populate(QSqlDatabase *db);
    ~libtree();
};

#endif // LIBTREE_HPP
