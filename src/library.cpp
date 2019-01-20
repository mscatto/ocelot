/*
 * The MIT License (MIT)
 * Copyright (c) 2018 Matheus Scattolin Anselmo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * The Software is provided “as is”, without warranty of any kind, express or implied,
 * including but not limited to the warranties of merchantability, fitness for a
 * particular purpose and noninfringement. In no event shall the authors or copyright
 * holders be liable for any claim, damages or other liability, whether in an action
 * of contract, tort or otherwise, arising from, out of or in connection with the
 * software or the use or other dealings in the Software.
 */

#include "library.hpp"
#include "vars.hpp"

#include <QObject>
#include <QStringList>
#include <QFuture>
#include <QFutureWatcher>
#include <QString>
#include <QMimeDatabase>
#include <QSqlRecord>
#include <QDebug>
#include <QSqlError>
#include <QRegularExpression>
#include <QSqlQueryModel>
#include <taglib/taglib.h>
#include <taglib/fileref.h>
#include <taglib/tpropertymap.h>
#include <qstandardpaths.h>
#include <QtConcurrent/QtConcurrent>

library::library(QString *path, QSqlDatabase *db, QStringList *pathlist):QObject(){
    this->db = *db;
    this->path = *path;
    this->pathlist = *pathlist;
}

library::~library(){

}

void library::process(){
    //this->db.open();
    this->scan(this->path);
    qInfo() << qPrintable("  -> "+QString::number(this->count).toUtf8()+" new tracks found at "+path);

    if(this->count>0){
        QSqlQuery *q = new QSqlQuery(this->db);
        q->prepare("INSERT INTO libs(path) VALUES(:path)");
        q->bindValue(":path", this->path);
        q->exec();

        //emit added(this->path);
    }
    //qInfo() << qPrintable("[INFO] Directory scan complete.");

    emit finished();
}

/* recursively scans for valid files */
void library::scan(QString dir){
    dir.replace("//","/");

    QDir *iter = new QDir(dir);
    iter->setFilter(QDir::NoSymLinks | QDir::Readable | QDir::Files);
    QStringList l = iter->entryList();

    if(l.size()>0){
        QMimeDatabase mimedb;
        QString fname;
        for(int i=0; i<l.size(); i++){
            fname = dir+"/"+l.at(i);
            if(formats.contains(mimedb.mimeTypeForFile(fname).name())){ /* case file has a valid mimetype from enum */
                if(!this->pathlist.contains(fname))
                    library::insert(fname);
            }
        }
    }

    iter->setFilter(QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Readable | QDir::Dirs); //now filters directories
    l = iter->entryList();
    if(l.size()>0){
        for(int i=0; i<l.size(); i++){ // recur each directory
            library::scan(QString(dir + "/" + l.at(i)));
        }
    }

    iter->~QDir();

}

/* appends and extracts metadata from track fpath to the database */
void library::insert(QString fpath){
    this->count++;
    QSqlQuery *q = new QSqlQuery(this->db);

    TagLib::FileRef *file = new TagLib::FileRef(qPrintable(fpath));
    QString root = fpath.chopped(fpath.length()-fpath.lastIndexOf("/"));
    QString fname = fpath.remove(0, fpath.lastIndexOf("/")+1);

    q->prepare("INSERT INTO songs (root, path, filename, title, artist, album, track, year, genre, discNumber, lib)"
              "VALUES(:root, :path, :filename, :title, :artist, :album, :track, :year, :genre, :discNumber, :lib)");


    q->bindValue(":root", root);
    q->bindValue(":path", root+"/"+fname);
    q->bindValue(":filename", fname);
    /* SEGFAULT HAPPENED HERE --SHITTY TAG */
    q->bindValue(":title", file->tag()->title().toCString(true));
    q->bindValue(":artist", file->tag()->artist().toCString(true));
    q->bindValue(":album", file->tag()->album().toCString(true));
    q->bindValue(":track", std::to_string(file->tag()->track()).c_str());
    q->bindValue(":year", std::to_string(file->tag()->year()).c_str());
    q->bindValue(":genre", file->tag()->genre().toCString(true));
    q->bindValue(":discNumber", file->tag()->properties().operator []("DISCNUMBER").toString().toCString());
    q->bindValue(":lib", this->path);

    q->exec();

    q->~QSqlQuery();
    file->~FileRef();
}
