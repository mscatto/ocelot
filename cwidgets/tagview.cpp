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
#include "tagview.hpp"
#include "src/mwindow.hpp"

#include <taglib/flacfile.h>
#include <taglib/oggfile.h>
#include <taglib/mpegfile.h>
#include <taglib/mp4file.h>
#include <taglib/wavfile.h>
#include <taglib/fileref.h>
#include <taglib/tpropertymap.h>
#include <taglib/tmap.h>

tagview::tagview(mwindow *parent) : QTreeWidget(parent){
    this->setColumnCount(2);
    this->setColumnWidth(0, 200);
    this->setHeaderHidden(false);

    /* todo non static customization */
    /* TagLib's property map and corresponding readable text for printing */
    this->table = new QMap<QString, QString>();
    this->table->insert("TITLE", "Title");
    this->table->insert("ALBUM", "Album");
    this->table->insert("ARTIST", "Artist");
    this->table->insert("YEAR", "Year");
    this->table->insert("GENRE", "Genre");
    this->table->insert("TRACKNUMBER", "Track");
    this->table->insert("DISCNUMBER", "Disc");
    /* */

    this->setHeaderLabels(QStringList() << "Field" << "Value");
    this->setRootIsDecorated(false);
    this->setAlternatingRowColors(true);

    connect(parent, &mwindow::selectionchanged, this, &tagview::swap);
}

tagview::~tagview(){

}

void tagview::swap(QString item){
    this->clear();

    TagLib::FileRef *x = new TagLib::FileRef(qPrintable(item));

    QStringList *keylist = new QStringList();
    keylist->append(QString(x->file()->properties().toString().toCString(true)).split("\n"));

    QString s;
    foreach(s, *keylist){
        if(s.contains("=", Qt::CaseInsensitive))
            this->addTopLevelItem(new QTreeWidgetItem(this, s.split("=")));
    }

    keylist->~QStringList();
    x->~FileRef();

    /*
    QMimeDatabase *mdb = new QMimeDatabase();
    QString sw = mdb->mimeTypeForFile(path).name().remove(0,6).toUpper();
    if(sw==format::FLAC){
        TagLib::FLAC::File *flacf = new TagLib::FLAC::File(qPrintable(path));

        flacf->~File();
    }else if(sw==format::MPEG){

    }else if(sw==format::OGG){
        qDebug() << "OGG";//TagLib::Ogg::File oggf(qPrintable(path));
    }else if(sw==format::WAV){

    }else if(sw==format::MP4){

    }

    mdb->~QMimeDatabase();*/
}
