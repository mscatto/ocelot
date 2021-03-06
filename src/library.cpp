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

#include "library.hpp"
#include "vars.hpp"

#include <QDebug>
#include <QFuture>
#include <QFutureWatcher>
#include <QMimeDatabase>
#include <QObject>
#include <QRegularExpression>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QString>
#include <QStringList>
#include <QtConcurrent/QtConcurrent>
#include <qstandardpaths.h>
#include <taglib/fileref.h>
#include <taglib/taglib.h>
#include <taglib/tpropertymap.h>

library::library (QString* path, QSqlDatabase* db, QStringList pathlist) : QObject() {
	this->db = *db;
	this->path = *path;
	this->pathlist = pathlist;
}

library::~library() {}

void library::process() {
	this->scan (this->path.replace ("//", "/"));
	qInfo() << qPrintable ("  -> " + QString::number (this->count).toUtf8() + " new tracks found at " + path);

	if (this->count > 0) {
		QSqlQuery q (this->db);
		q.prepare ("INSERT INTO libs(path) VALUES(:path)");
		q.bindValue (":path", this->path);
		q.exec();

		emit added (this->path);
	}

	emit finished();
}

/* recursively scans for valid files */
void library::scan (QString dir) {
	QDir iter (dir);
	iter.setFilter (QDir::NoSymLinks | QDir::Readable | QDir::Files);
	QStringList l = iter.entryList();

	if (l.size() > 0)
		for (int i = 0; i < l.size(); i++) {
			QString fname = dir + "/" + l.at (i);
			if (formats.contains (QMimeDatabase().mimeTypeForFile (fname).name()) &&
				!this->pathlist.contains (fname)) /* case file has a valid mimetype from enum */
				library::insert (fname);
		}

	iter.setFilter (QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Readable | QDir::Dirs);	 // now filters directories
	l = iter.entryList();
	if (l.size() > 0)
		for (int i = 0; i < l.size(); i++)	// recur each directory
			library::scan (dir + "/" + l.at (i));
}

/* appends and extracts metadata from track fpath to the database */
void library::insert (QString fpath) {
	this->count++;
	QSqlQuery q (this->db);
	TagLib::FileRef file (qPrintable (fpath));
	QString root = fpath.chopped (fpath.length() - fpath.lastIndexOf ("/"));
	QString fname = fpath.remove (0, fpath.lastIndexOf ("/") + 1);

	q.prepare ("INSERT INTO songs (root, path, filename, title, artist, album, "
			   "track, year, genre, discNumber, lib)"
			   "VALUES(:root, :path, :filename, :title, :artist, :album, :track, "
			   ":year, :genre, :discNumber, :lib)");

	q.bindValue (":root", root);
	q.bindValue (":path", root + "/" + fname);
	q.bindValue (":filename", fname);
	/* SEGFAULT HAPPENED HERE --SHITTY TAG */
	q.bindValue (":title", file.tag()->title().toCString (true));
	q.bindValue (":artist", file.tag()->artist().toCString (true));
	q.bindValue (":album", file.tag()->album().toCString (true));
	q.bindValue (":track", std::to_string (file.tag()->track()).c_str());
	q.bindValue (":year", std::to_string (file.tag()->year()).c_str());
	q.bindValue (":genre", file.tag()->genre().toCString (true));
	q.bindValue (":discNumber", file.tag()->properties().operator[] ("DISCNUMBER").toString().toCString());
	q.bindValue (":lib", this->path);

	q.exec();
}
