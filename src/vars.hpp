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
	// this should be moved to a dedicated file
	const char* VERSION = "0.3c";
	const QString DATA_PATH = QStandardPaths::writableLocation (QStandardPaths::ConfigLocation);
	const QString DB_PATH = DATA_PATH + "/ocelot.db";
	QSqlDatabase* DB_REF = new QSqlDatabase();
	player* pctx;

	QDBusInterface* bus;
	QStringList* libs;	// this is bad

	// -- FUNCTIONS --
	void remlibs (QString path);
	void setdbdata (const char* var, QVariant val);
	QVariant fetchdbdata (const char* var);
	QStringList dumppaths();
	QStringList dumplibinfo();

	// DEBUG HELPERS
	void TODO (const char* msg) {
		qInfo ("[TODO] %s", msg);
	};
	void PANIC (const char* msg) {
		qWarning ("[PANIC] %s", msg);
	};

	// TAG-RELATED FUNCTIONS
	QString translate_key (QString key);
	QString translate_val (QString val);
	QStringList dumpval();
	QStringList dumpkeys();

	vars();
	~vars() {}

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
  signals:
	void sendargs (int argc, char** argv);
};
#endif	// VARS_HPP
