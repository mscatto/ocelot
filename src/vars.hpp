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
    const char* VERSION = "0.2c";
    QString DATA_PATH = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/ocelot";
    QString DB_PATH = DATA_PATH+"/ocelot.database";
    QSqlDatabase *DB_REF = new QSqlDatabase();
    QList<library*> *libs = new QList<library*>();
    QString translate_key(QString key);
    QString translate_val(QString val);
    QStringList dumpvars();
    QStringList dumpkeys();
    void remlibs(QString path);

    vars();
    ~vars();
private:
    QMap<QString,QString> pmap;
    void initpmap();
    void initdb();
    void initlibs();
    void initdata();
};
#endif // VARS_HPP
