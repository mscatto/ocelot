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

#ifndef VARS_HPP
#define VARS_HPP
#include <QStringList>
#include <QStandardPaths>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMap>
#include <QThread>
#include "library.hpp"
#include "player.hpp"

class vars : QObject
{
public:
    const char* VERSION = "0.3b";
    QString DATA_PATH = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);//+"/ocelot";
    QString DB_PATH = DATA_PATH+"/ocelot.database";
    QSqlDatabase *DB_REF = new QSqlDatabase();
    QString translate_key(QString key);
    QString translate_val(QString val);
    QStringList dumpvars();
    QStringList dumpkeys();
    QString fetchdata(QString *var);
    void setdata(QString *var, QString *data);
    void remlibs(QString path);
    QStringList *dumppaths();
    QStringList *dumplibinfo();
    QStringList *libs;
    player *mp;
    vars();
    ~vars();
private:
    QMap<QString,QString> pmap;
    QMap<QString,bool> ptmap;
    void initpmap();
    void initdb();
    void initlibs();
    void initdata();
    void initplayer();
public slots:
    void libfinished(uint *num);
};
#endif // VARS_HPP
