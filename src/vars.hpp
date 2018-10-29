#ifndef VARS_HPP
#define VARS_HPP
#include <QStringList>
#include <QStandardPaths>
#include <QSqlDatabase>
#include <QMap>
#include "library.hpp"

class vars
{
public:
    const char* VERSION = "0.3a";
    QString DATA_PATH = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/ocelot";
    QString DB_PATH = DATA_PATH+"/ocelot.database";
    QSqlDatabase *DB_REF = new QSqlDatabase();
    QList<library*> *libs = new QList<library*>();
    QString translate_key(QString key);
    QString translate_val(QString val);
    QStringList dumpvars();
    QStringList dumpkeys();
    QString fetchdata(QString *var);
    void setdata(QString *var, QString *data);
    void remlibs(QString path);

    vars();
    ~vars();
private:
    QMap<QString,QString> pmap;
    QMap<QString,bool> ptmap;
    void initpmap();
    void initdb();
    void initlibs();
    void initdata();
};
#endif // VARS_HPP
