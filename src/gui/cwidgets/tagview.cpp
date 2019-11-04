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

#include "tagview.hpp"
#include "../mwindow.hpp"

#include <taglib/fileref.h>
#include <taglib/flacfile.h>
#include <taglib/mp4file.h>
#include <taglib/mpegfile.h>
#include <taglib/oggfile.h>
#include <taglib/tmap.h>
#include <taglib/tpropertymap.h>
#include <taglib/wavfile.h>

tagview::tagview(vars* jag, mwindow* parent, workbench* wb) : QTreeWidget() {
    this->jag = jag;
    this->wb = wb;
    this->ctx = new QMenu;
    this->setColumnCount(2);
    this->setColumnWidth(0, 200);
    this->setHeaderHidden(false);
    this->setObjectName("tagview");

    this->setHeaderLabels(QStringList() << "Tag"
                                        << "Data");
    this->setRootIsDecorated(false);
    this->setAlternatingRowColors(true);
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(parent, &mwindow::selectionchanged, this, &tagview::swap);
    connect(this, &QTreeWidget::customContextMenuRequested, this, &tagview::showctx);
}

tagview::~tagview() {
}

void tagview::showctx(const QPoint& pos) {
    if(this->wb->islocked())
        this->ctx->exec(this->mapToGlobal(pos));
    else {
        this->wb->setlastctx(this);
        this->wb->ctx_req(this->mapTo(this->wb, pos));
    }
}

void tagview::swap(QString item) {
    this->clear();

    TagLib::FileRef* x = new TagLib::FileRef(qPrintable(item));

    QStringList* keylist = new QStringList();
    keylist->append(QString(x->file()->properties().toString().toCString(true)).split("\n"));

    QString s;
    foreach(s, *keylist) {
        if(s.contains("=", Qt::CaseInsensitive)) {
            QStringList* qsl = new QStringList(s.split("="));
            if(this->jag->translate_key(qsl->first()) != "") /* case there's a translation on record */
                qsl->replace(0, this->jag->translate_key(qsl->first()));
            this->addTopLevelItem(new QTreeWidgetItem(this, *qsl)); /* or else just show the taglib name for it */
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
