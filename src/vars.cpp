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
 * The Software is provided “as is”, without warranty of any kind, sress or implied,
 * including but not limited to the warranties of merchantability, fitness for a
 * particular purpose and noninfringement. In no event shall the authors or copyright
 * holders be liable for any claim, damages or other liability, whether in an action
 * of contract, tort or otherwise, arising from, out of or in connection with the
 * software or the use or other dealings in the Software.
 */

#include "vars.hpp"
#include "player.hpp"

#include <QDir>
#include <QDebug>
#include <QStringList>
#include <QSqlError>
#include <QSqlQuery>
#include <QTreeWidget>
#include <QtConcurrent/QtConcurrent>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QSqlResult>
#include<bits/stdc++.h>

vars::vars(){
    qInfo("[INFO] Starting Ocelot Media Manager v%s", this->VERSION);

    initpmap();
    initdb();
    initlibs();
    initplayer();
}

vars::~vars(){

}

void vars::initplayer(){
    this->mp = new player();
}

QString vars::translate_key(QString key){
    return this->pmap.value(key);
}

QString vars::translate_val(QString val){
    return this->pmap.key(val);
}

QStringList vars::dumpvars(){
    return this->pmap.values();
}

QStringList vars::dumpkeys(){
    return this->pmap.keys();
}

QString vars::fetchdata(QString *var){
    QSqlQuery q;
    q.exec("SELECT val FROM data WHERE var='"+*var+"'");
    if(!q.isNull(0))
        return QString();
    q.next();
    return q.value(0).toString();
}

void vars::remlibs(QString path){
    this->libs->removeAll(path);
    /*for(int i=this->libs->length()-1; i>=0; i--){
        if(this->libs->at(i)->dumpinfo()->split(";").first() == path){
            this->libs->removeAt(i);
        }
    }*/

    this->DB_REF->exec("DELETE FROM songs WHERE lib='"+path+"'");
    this->DB_REF->exec("DELETE FROM libs WHERE path='"+path+"'");
}

QStringList *vars::dumppaths(){
    QSqlQuery *x = new QSqlQuery(this->DB_REF->exec("SELECT path FROM songs"));
    QStringList *out = new QStringList();

    while(x->next())
        out->append(x->value(0).toString());

    return out;
}

QStringList *vars::dumplibinfo(){
    QStringList *out = new QStringList();
    foreach(QString s, *this->libs){
        QFileInfo i;
        uint count=0;
        qint64 size=0;
        QSqlQuery x = this->DB_REF->exec("SELECT path FROM songs WHERE lib='"+s+"'");
        while(x.next()){
            count++;
            i.setFile(x.value(0).toString());
            size+=i.size();
        }

        s.append(";"+QString::number(count)+";");

        if(size>0)
            s.append(QString::number(size/1024/1024));
        else
            s.append(QString::number(0));
        out->append(s);
    }
    return out;
}

void vars::initdb(){
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    *this->DB_REF = QSqlDatabase::addDatabase("QSQLITE");
    this->DB_REF->setDatabaseName(this->DB_PATH);
    this->DB_REF->setHostName("BABOU");

    QDir().mkdir(this->DATA_PATH);
    if(!QDir(this->DATA_PATH).exists()){
        qFatal("%s",qPrintable("[FATAL] Unable to create data directory in '"+this->DATA_PATH+"' !"));
    }

    if(!this->DB_REF->open()){
        qFatal("%s",qPrintable("[FATAL] Could not open database: "+this->DB_REF->lastError().driverText()));
    }

    QSqlQuery *x = new QSqlQuery();
    *x = this->DB_REF->exec("SELECT _rowid_ FROM data");
    x->last();

    if(x->isNull(0)){
        qInfo() << "[INFO] No valid database found! generating defaults...";
        this->DB_REF->exec("CREATE TABLE libs(\
            path TEXT NOT NULL, PRIMARY KEY('path')\
        )");
        this->DB_REF->exec("CREATE TABLE data(\
            var TEXT NOT NULL, val TEXT NOT NULL\
        )");
        this->DB_REF->exec("CREATE TABLE songs (\
            root TEXT,\
            path TEXT,\
            filename TEXT,\
            title TEXT,\
            artist TEXT,\
            album TEXT,\
            track INTEGER,\
            year TEXT,\
            genre TEXT,\
            discNumber INTEGER,\
            lib TEXT, FOREIGN KEY('lib') REFERENCES 'libs'('path')\
        )");
        /*this->DB_REF->exec("CREATE TABLE properties(\
            song_id INTEGER,\
            format TEXT,\
            duration REAL,\
            bitrate REAL,\
            bits_per_sample INTEGER,\
            sample_rate INTEGER,\
            channels INTEGER,\
            audio_sha256sum TEXT,\
            FOREIGN KEY(song_id) REFERENCES songs(_rowid_) ON DELETE CASCADE\
        )");*/

        /* initialize default values */
        initdata();

        qInfo() << "  -> created at" << this->DATA_PATH+"/ocelot.database";
    }else{
        qInfo("%s",qPrintable("[INFO] Database found at "+this->DB_REF->databaseName()+"..."));

        x->exec("SELECT path FROM songs");
        x->last();

        if(x->isNull(0))
            qInfo("%s",qPrintable("  -> 0 tracks on record."));
        else
            qInfo("%s",qPrintable("  -> "+QString::number(x->at()+1)+" tracks on record"));

        qInfo() << "[INFO] Running sanity check...";

        x->seek(0);
        QFile dummy;
        uint inv=0;
        while(x->next()){
            dummy.setFileName(x->value(0).toString());
            if(!dummy.exists()){
                inv++;
                this->DB_REF->exec("DELETE FROM songs WHERE path='"+x->value(0).toString()+"'");
            }
        }
        qInfo("%s", qPrintable("  -> "+QString::number(inv)+" entries were removed."));
    }
    x->~QSqlQuery();
    //qInfo() << "[INFO] Database initialized successfully.";
}

void vars::initlibs(){
    qInfo() << "[INFO] Scanning libraries...";
    this->libs = new QStringList();

    QSqlQuery q = this->DB_REF->exec("SELECT path FROM libs");
    QThread *t;
    //q.next();
    if(!q.isNull(0)){
        do{
            this->libs->append(q.value(0).toString());
            t = new QThread();
            library *l = new library(new QString(q.value(0).toString()),this->DB_REF,this->dumppaths());
            l->moveToThread(t);
            connect(t, SIGNAL(started()), l, SLOT(process()));
            connect(l, SIGNAL(finished()), t, SLOT(quit()));
            connect(l, SIGNAL(finished()), l, SLOT(deleteLater()));
            connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
            //connect(t, &QThread::finished, this, );
            t->start();
        }while(q.next());
    }
    qInfo() << "[INFO] Done.";
}

void vars::initdata(){
    this->DB_REF->exec("INSERT INTO data VALUES(\
        'playlist_columnorder',\
        '#PLAY#;#INDEX#;TRACKNUMBER;TITLE;ARTIST;ALBUM;YEAR')");
    this->DB_REF->exec("INSERT INTO data VALUES(\
        'general_doubleclick',\
        '1')");
    this->DB_REF->exec("INSERT INTO data VALUES(\
        'general_middleclick',\
        '0')");
    this->DB_REF->exec("INSERT INTO data VALUES(\
        'general_appendbehaviour',\
        '0')");
    this->DB_REF->exec("INSERT INTO data VALUES(\
        'general_runwizard',\
        '1')");
    this->DB_REF->exec("INSERT INTO data VALUES(\
        'ui_scheme',\
        '0')");
}

void vars::initpmap(){
    this->pmap.insert("TITLE", "Title");
    this->pmap.insert("ALBUM","Album");
    this->pmap.insert("ARTIST","Artist");
    this->pmap.insert("ALBUMARTIST","Album Artist");
    this->pmap.insert("SUBTITLE","Subtitle");
    this->pmap.insert("TRACKNUMBER","Track #");
    this->pmap.insert("TRACKTOTAL","Tracks Total");
    this->pmap.insert("DISCNUMBER", "Disc #");
    this->pmap.insert("DISCTOTAL", "Disc Count");
    this->pmap.insert("DATE","Year");
    this->pmap.insert("ORIGINALDATE","Original Year");
    this->pmap.insert("GENRE","Genre");
    this->pmap.insert("COMMENT","Comment");
    this->pmap.insert("COMPOSER","Composer");
    this->pmap.insert("LYRICS","Lyrics");
}
