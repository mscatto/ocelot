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
#ifndef LIBRARY_HPP
#define LIBRARY_HPP

#include <QList>

#include "database.hpp"
//#include "vars.hpp"
#include <QSqlQueryModel>
#include <QSqlDatabase>

class library
{
private:
    const QStringList formats = {"audio/mpeg",
                                 "audio/mp4",
                                 "audio/ogg",
                                 "audio/wav",
                                 "audio/flac"
    };
    QSqlDatabase *db;
    qint64 size = 0;
    QStringList tracks;
    QString libpath;

    void insert(QString *fpath);
    void scan(QString *dir);
public:
    library(QString path, QSqlDatabase *db);
    QString* dumpinfo();
    ~library();
};

#endif // LIBRARY_HPP
