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

#ifndef PLAYLIST_HPP
#define PLAYLIST_HPP

#include <QTreeWidget>
#include "../mwindow.hpp"

class playlist : public QTreeWidget
{
    Q_OBJECT
private:
    QString *order;
    vars *jag;
    QMenu *headerctx;
    QMenu *bodyctx;
    QMenu *emptyctx;
    ushort playing_index=0;
    QTreeWidgetItem *playing;
    QTreeWidgetItem *selected;
    QStringList pl;
    QTabWidget *view;

    const char* playchar = "►";
    bool contains(QString path);
    void dump(QString *path);
public:
    playlist(QString *order, QMenu *headerctx, vars *jag, mwindow *win, QWidget *parent);
    ~playlist();
private slots:
    void show_headerctx(const QPoint & pos);
    void show_bodyctx(const QPoint &pos);
public slots:
    void rebuild_columns();
    void append(QStringList f);
    void next();
    void prev();
    void doubleclick(QTreeWidgetItem*item);
    void exportpl();
    void clearchildren();
signals:
    void changesel(TagLib::FileRef *f);
    void play(QTreeWidgetItem*);
    void EOP();
};

#endif // PLAYLIST_HPP
