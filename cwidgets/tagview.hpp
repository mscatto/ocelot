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
#ifndef TAGVIEW_HPP
#define TAGVIEW_HPP

#include <QObject>
#include <QWidget>
#include <QTreeWidget>
#include <taglib/fileref.h>

#include "src/vars.hpp"
#include "src/mwindow.hpp"

class tagview : public QTreeWidget{
private:
    vars *jag;
    const QStringList order = (QStringList()<<"TITLE"<<"ALBUM"<<"ARTIST"<<"YEAR"<<"GENRE"<<"TRACKNUMBER"<<"DISCNUMBER");
public:
    enum format {FLAC=0, MPEG=1, OGG=2, MP4=3, WAV=4};
    tagview(vars *jag, mwindow *parent);
    ~tagview();
public slots:
    void swap(QString item);
};

#endif // TAGVIEW_HPP
