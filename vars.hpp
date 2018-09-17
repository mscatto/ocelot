#ifndef VARS_HPP
#define VARS_HPP
#include <QStringList>
#include <QStandardPaths>
#include <QSqlDatabase>
#include <QMap>
#include <library.hpp>

class vars
{
public:
    const char* VERSION = "0.2b";
    QString DATA_PATH = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/ocelot";
    QString DB_PATH = DATA_PATH+"/ocelot.database";
    QSqlDatabase *DB_REF = new QSqlDatabase();

    //static QList<QTreeWidgetItem *> *dumplibs();
    QList<library*> *libs = new QList<library*>();

    vars();
    ~vars();
private:
    void initdb();
    void initlibs();
};
#endif // VARS_HPP
