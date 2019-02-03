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

#ifndef TAGVIEW_HPP
#define TAGVIEW_HPP

#include <QObject>
#include <QWidget>
#include <QTreeWidget>
#include <taglib/fileref.h>

#include "src/vars.hpp"
#include "src/mwindow.hpp"
#include "src/workbench.hpp"

class tagview : public QTreeWidget{
    Q_OBJECT
private:
    const QStringList order = (QStringList()<<"TITLE"<<"ALBUM"<<"ARTIST"<<"YEAR"<<"GENRE"<<"TRACKNUMBER"<<"DISCNUMBER");
    vars *jag;
    workbench *wb;
    QMenu *ctx;
    void showctx(const QPoint &pos);
public:
    enum format {FLAC=0, MPEG=1, OGG=2, MP4=3, WAV=4};
    tagview(vars *jag, mwindow *parent, workbench *wb);
    ~tagview();
public slots:
    void swap(QString item);
};

#endif // TAGVIEW_HPP
