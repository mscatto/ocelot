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

#ifndef LIBRARY_HPP
#define LIBRARY_HPP

#include <QList>

#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QtWidgets>

class library : public QObject {
	Q_OBJECT
  private:
	const QStringList formats = {"audio/mpeg", "audio/mp4", "audio/ogg", "audio/wav", "audio/flac"};
	QString path;
	QStringList pathlist;
	QSqlDatabase db;

	qint64 count = 0;

	void insert (QString fpath);
	void scan (QString dir);

  public:
	explicit library (QString* path, QSqlDatabase* db, QStringList pathlist);
	~library();
  public slots:
	void process();
  signals:
	void finished();
	void added (QString lib);
};

#endif	// LIBRARY_HPP
