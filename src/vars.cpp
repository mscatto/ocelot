#include "vars.hpp"
#include <QDir>
#include <QDebug>
#include <QStringList>
#include <QSqlError>
#include <QSqlQuery>
#include <QTreeWidget>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QSqlResult>

vars::vars(){
    qInfo("[INFO] Starting Ocelot Media Manager v%s", this->VERSION);

    initdb();
    initlibs();

}

vars::~vars(){

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
    *x = this->DB_REF->exec("SELECT _rowid_ FROM songs");
    x->last();

    if(x->isNull(0)){
        qInfo() << "[INFO] No database found! generating a new one...";
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
            discNumber INTEGER\
        )");
        this->DB_REF->exec("CREATE TABLE properties(\
            song_id INTEGER,\
            format TEXT,\
            duration REAL,\
            bitrate REAL,\
            bits_per_sample INTEGER,\
            sample_rate INTEGER,\
            channels INTEGER,\
            audio_sha256sum TEXT,\
            FOREIGN KEY(song_id) REFERENCES songs(_rowid_) ON DELETE CASCADE\
        )");
        this->DB_REF->exec("CREATE TABLE libs(\
            path TEXT,\
            trackcount INTEGER,\
            filesize INTEGER\
        )");
        qInfo() << "  -> created at" << this->DATA_PATH+"/ocelot.database";
    }else{
        qInfo("%s",qPrintable("[INFO] Database found at "+this->DB_REF->databaseName()+"..."));
        qInfo("%s",qPrintable("  -> "+QString::number(x->value(0).toInt())+" tracks on record"));
    }
    x->~QSqlQuery();
    qInfo() << "[INFO] Database initialized successfully.";
}

void vars::initlibs(){
    qInfo() << "[INFO] Initializing saved libraries...";

    QSqlQuery *q = new QSqlQuery();
    *q = this->DB_REF->exec("SELECT path FROM libs");
    while(q->next()){
       this->libs->append(new library(q->value(0).toString(),this->DB_REF));
    }
    q->~QSqlQuery();

    qInfo() << "[INFO] Done.";
}
