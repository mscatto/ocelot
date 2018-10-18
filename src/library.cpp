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
#include "library.hpp"
#include "database.hpp"
//#include "vars.hpp"
#include <QStringList>
#include <QString>
#include <QMimeDatabase>
#include <QSqlRecord>
#include <QDebug>
#include <QSqlError>
#include <QRegularExpression>
#include <QSqlQueryModel>
#include <taglib.h>
#include <taglib/fileref.h>
#include <tpropertymap.h>
#include <qstandardpaths.h>
#include <QtConcurrent/QtConcurrent>

library::library(QString path, QSqlDatabase *db){
    this->db = db;
    this->libpath = path;
    this->size = 0;
    this->scan(&this->libpath);
    qInfo() << qPrintable("  -> "+QString::number(this->tracks.length()).toUtf8()+" tracks found at "+path);

}

QString *library::dumpinfo(){
    QString *s = new QString();
    s->append(this->libpath+";");
    s->append(QString::number(this->tracks.length())+";");

    if(this->size>0)
        s->append(QString::number(this->size/1024/1024));
    else
        s->append(QString::number(0));
    return s;
}

/* recursively scans for valid files */
void library::scan(QString *dir){
    dir->replace("//","/");

    QDir *iter = new QDir(*dir);
    iter->setFilter(QDir::NoSymLinks | QDir::Readable | QDir::Files);
    QStringList l = iter->entryList();
    QFileInfo info;
    //iter->~QDir();

    if(l.size()>0){
        QMimeDatabase mimedb;

        QString fname;
        QSqlQuery *nq;

        for(int i=0; i<l.size(); i++){
            fname = *dir+"/"+l.at(i);
            if(formats.contains(mimedb.mimeTypeForFile(fname).name())){ /* case file has a valid mimetype from enum */
                nq = new QSqlQuery();
                nq->prepare("SELECT path FROM songs WHERE path=:fname");
                nq->bindValue(":fname", fname);
                nq->exec();
                nq->next();

                if(nq->record().value(0).isNull())/* not found inside the database */
                    this->insert(fname);

                info.setFile(fname);
                this->size += info.size();
                this->tracks.append(fname);

                nq->~QSqlQuery();
            }

        }
    }

    iter->setFilter(QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Readable | QDir::Dirs); //now filters directories
    l = iter->entryList();
    if(l.size()>0){
        for(int i=0; i<l.size(); i++){ // recur each directory
            library::scan(new QString(*dir + "/" + l.at(i)));
        }
    }

    iter->~QDir();
}

/* appends and extracts metadata from track fpath to the database */
void library::insert(QString fpath){
    QSqlQuery q;
    TagLib::FileRef file(qPrintable(fpath));
    QString root = fpath.chopped(fpath.length()-fpath.lastIndexOf("/"));
    QString fname = fpath.remove(0, fpath.lastIndexOf("/")+1);

    q.prepare("INSERT INTO songs (root, path, filename, title, artist, album, track, year, genre, discNumber, lib)"
              "VALUES(:root, :path, :filename, :title, :artist, :album, :track, :year, :genre, :discNumber, :lib)");

    q.bindValue(":root", root);
    q.bindValue(":path", root+"/"+fname);
    q.bindValue(":filename", fname);
    q.bindValue(":title", file.tag()->title().toCString(true));
    q.bindValue(":artist", file.tag()->artist().toCString(true));
    q.bindValue(":album", file.tag()->album().toCString(true));
    q.bindValue(":track", std::to_string(file.tag()->track()).c_str());
    q.bindValue(":year", std::to_string(file.tag()->year()).c_str());
    q.bindValue(":genre", file.tag()->genre().toCString(true));
    q.bindValue(":discNumber", file.tag()->properties().operator []("DISCNUMBER").toString().toCString());
    q.bindValue(":lib", this->libpath);

    q.exec();
}


