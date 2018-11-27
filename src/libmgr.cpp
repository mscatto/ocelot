#include <QSqlError>

libmgr::libmgr(){
    this->libs = new QList<library*>();
    //this->initdb(jag);

    qInfo() << "[INFO] Scanning directories for file changes...";
    QString s;
    /*foreach(s, src){
        libs->append(new library(jag, &s, this->db));
    }*/
}

libmgr::~libmgr(){

}



