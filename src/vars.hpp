/*
 * The MIT License (MIT)
 * Copyright (c) 2018 Matheus Scattolin Anselmo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * The Software is provided “as is”, without warranty of any kind, express or
 * implied, including but not limited to the warranties of merchantability,
 * fitness for a particular purpose and noninfringement. In no event shall the
 * authors or copyright holders be liable for any claim, damages or other
 * liability, whether in an action of contract, tort or otherwise, arising from,
 * out of or in connection with the software or the use or other dealings in the
 * Software.
 */

#ifndef VARS_HPP
#define VARS_HPP
#include "src/library.hpp"
#include "src/player.hpp"
#include <QMap>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QStringList>
#include <QThread>
#include <QtDBus/QtDBus>

class vars : QObject {
    Q_OBJECT
public:
    /* ocelot version */
    const char* VERSION = "0.3b";
    /* data root path */
    const QString DATA_PATH = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    /* DB file path */
    const QString DB_PATH = DATA_PATH + "/ocelot.database";

    /* shared DB instance */
    QSqlDatabase* DB_REF = new QSqlDatabase();
    /* player instance */
    player* pctx;

    QDBusInterface* bus;

    //*// THESE DEAL WITH THE PROPERTY MAP TRANSLATION FROM INTERNAL TAGLIB TAG
    // PROPERTIES
    // TO A MORE READABLE FORM. ie var "ALBUMARTIST" to val "Album Artist".
    //
    /* returns the corresponding value according to key from property table */
    QString translate_key(QString key);
    /* fetches the key from a given value from pmap*/
    QString translate_val(QString val);
    /* dumps values from property table pmap*/
    QStringList dumpval();
    /* dump pmap keys */
    QStringList dumpkeys();
    //*//---

    /* remove lib indicated by path */
    void remlibs(QString path);
    /* set var on DB data table to var */
    void setdbdata(const char* var, QVariant val);
    /* fetch var's val from DB data table */
    QVariant fetchdbdata(const char* var);

    QStringList* dumppaths();
    QStringList* dumplibinfo();
    QStringList* libs; // this is bad

    vars();
    ~vars() {
    }

private:
    QMap<QString, QString> pmap;
    QMap<QString, bool> ptmap;
    void initpmap();
    void initdb();
    void initlibs();
    void initdata();
    void initaudio();
public slots:
    // void libfinished(uint *num);
    // void p();
};
#endif // VARS_HPP
