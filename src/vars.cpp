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

    initpmap();
    initdb();
    initlibs();
}

vars::~vars(){

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
    for(int i=this->libs->length()-1; i>=0; i--){
        if(this->libs->at(i)->dumpinfo()->split(";").first() == path){
            this->libs->removeAt(i);
        }
    }

    this->DB_REF->exec("DELETE FROM songs WHERE lib='"+path+"'");
    this->DB_REF->exec("DELETE FROM libs WHERE path='"+path+"'");
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

        x->exec("SELECT _rowid_ FROM songs");
        x->last();
        if(x->isNull(0))
            qInfo("%s",qPrintable("  -> 0 tracks on record"));
        else
            qInfo("%s",qPrintable("  -> "+QString::number(x->value(0).toInt())+" tracks on record"));
    }
    x->~QSqlQuery();
    qInfo() << "[INFO] Database initialized successfully.";
}

void vars::initlibs(){
    qInfo() << "[INFO] Initializing saved libraries...";

    QSqlQuery *q = new QSqlQuery();
    *q = this->DB_REF->exec("SELECT path FROM libs");
    q->next();
    if(!q->isNull(0)){
        do{
            this->libs->append(new library(q->value(0).toString(),this->DB_REF));
        }while(q->next());
    }
    q->~QSqlQuery();
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
}

void vars::initpmap(){
    this->pmap.insert("TITLE", "Title");
    this->pmap.insert("ALBUM","Album");
    this->pmap.insert("ARTIST","Artist");
    this->pmap.insert("ALBUMARTIST","Album Artist");
    this->pmap.insert("SUBTITLE","Subtitle");
    this->pmap.insert("TRACKNUMBER","Track #");
    this->pmap.insert("TRACKTOTAL","Track Count");
    this->pmap.insert("DISCNUMBER", "Disc #");
    this->pmap.insert("DISCTOTAL", "Disc Count");
    this->pmap.insert("DATE","Year");
    this->pmap.insert("ORIGINALDATE","Original Year");
    this->pmap.insert("GENRE","Genre");
    this->pmap.insert("COMMENT","Comment");
    this->pmap.insert("COMPOSER","Composer");
    this->pmap.insert("LYRICS","Lyrics");
}

