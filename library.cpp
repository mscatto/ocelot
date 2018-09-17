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

library::library(QString path, QSqlDatabase *db){
    this->db = db;
    this->libpath = path;
    this->scan(&path);

    qInfo() << qPrintable("  -> "+QString::number(this->tracks.length()).toUtf8()+" tracks found at "+path);
}

QString *library::dumpinfo(){
    QString *s = new QString();

    s->append(this->libpath+";");
    s->append(QString::number(this->tracks.length())+";");
    s->append(QString::number(this->size));

    return s;
}

/* recursively scans for valid files */
void library::scan(QString *dir){
    dir->replace("//","/");

    QDir *iter = new QDir(*dir);
    iter->setFilter(QDir::NoSymLinks | QDir::Readable | QDir::Files); // set filters to just files
    QStringList l = iter->entryList();
    QFileInfo *info;

    if(l.size()>0){
        QMimeDatabase mimedb;
        QString *file;

        for(int i=0; i<l.size(); i++){
            file = new QString(*dir+"/"+l.at(i));

            QSqlQuery *x = new QSqlQuery();
            *x = this->db->exec("SELECT path FROM songs WHERE path='"+*file+"'");
            x->last();
            if(formats.contains(mimedb.mimeTypeForFile(*file).name())){
                if(x->isNull(0))
                    this->insert(file);
                this->tracks.append(*file);
                info = new QFileInfo(*file);
                this->size += info->size();

                info->~QFileInfo();
            }

            file->~QString();
            x->~QSqlQuery();
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
void library::insert(QString *fpath){
    TagLib::FileRef file(qPrintable(*fpath));
    QString root = fpath->chopped(fpath->length()-fpath->lastIndexOf("/"));
    QString fname = fpath->remove(0, fpath->lastIndexOf("/")+1);

    QStringList data;

    data << root << root+"/"+fname << fname << file.tag()->title().toCString(true)/*title*/
    << file.tag()->artist().toCString(true)/*artist*/
    << file.tag()->album().toCString(true)/*album*/
    << std::to_string(file.tag()->track()).c_str()/*track*/
    << std::to_string(file.tag()->year()).c_str()/*year*/
    << file.tag()->genre().toCString(true)/*genre*/
    << file.tag()->properties().operator []("DISCNUMBER").toString().toCString();/*discnumber*/

    int i=0;
    QString out;
    while(i < data.length()){
        if(data.at(i)=="")
            data.replace(i, "[UNKNOWN]");

        out.append("'"+data.at(i)+"', ");
        i++;
    }
    out.chop(2);/*remove last ', '*/
    out.append(")");

    this->db->exec("INSERT INTO songs VALUES("+out);
}


