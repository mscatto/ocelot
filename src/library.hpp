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
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QtWidgets>

class library : public QObject
{
    Q_OBJECT
private:
    const QStringList formats = {"audio/mpeg",
                                 "audio/mp4",
                                 "audio/ogg",
                                 "audio/wav",
                                 "audio/flac"
    };
    QString path;
    QStringList pathlist;
    QSqlDatabase db;

    qint64 count=0;

    void insert(QString fpath);
    void scan(QString dir);
public:
    explicit library(QString *path, QSqlDatabase *db, QStringList *pathlist);
    ~library();
public slots:
    void process();
signals:
    void finished();
    void added(QString lib);
};

#endif // LIBRARY_HPP
