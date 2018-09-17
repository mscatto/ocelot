#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QAudioDecoder>
#include <QDir>
#include <QFile>
#include <QSqlQuery>
#include <QMimeDatabase>
#include <QTreeWidget>

class database
{
private:

    QSqlQuery query;
    bool status;
public:
    database();
    //QStringList query(QString column);
     // maybe enum in the future?
};

#endif // DATABASE_H
