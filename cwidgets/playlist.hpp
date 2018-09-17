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
#ifndef PLAYLIST_HPP
#define PLAYLIST_HPP

#include <QObject>
#include <QWidget>
#include <QTreeWidget>
#include "mwindow.hpp"

class playlist : public QTreeWidget
{
private:
    QMenu *hctx;
    QMenu *bctx;
public:
    playlist(mwindow *parent);
    ~playlist();
public slots:
    void append(QStringList f);
    void headerctx(QPoint p);
};

#endif // PLAYLIST_HPP
