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

tagview::tagview(vars *jag, mwindow *parent) : QTreeWidget(parent){
    this->jag = jag;
    this->setColumnCount(2);
    this->setColumnWidth(0, 200);
    this->setHeaderHidden(false);

    this->setHeaderLabels(QStringList() << "Tag" << "Data");
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
        if(s.contains("=", Qt::CaseInsensitive)){
            QStringList *qsl = new QStringList(s.split("="));
            if(this->jag->translate_key(qsl->first())!="")/* case there's a translation on record */
                qsl->replace(0,this->jag->translate_key(qsl->first()));
            this->addTopLevelItem(new QTreeWidgetItem(this, *qsl));/* or else just show the taglib name for it */
            qsl->~QStringList();
        }
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
