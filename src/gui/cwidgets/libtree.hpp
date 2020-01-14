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

#ifndef LIBTREE_HPP
#define LIBTREE_HPP

#include <QFrame>
#include <QObject>
#include <QPushButton>
#include <QTreeWidget>
#include <QWidget>

#include "src/gui/mwindow.hpp"
#include "src/gui/workbench.hpp"
#include "src/gui/dialogs/tageditor.hpp"
#include "src/library.hpp"
#include "src/vars.hpp"

class libtree : public QFrame {
    Q_OBJECT
private:
    enum CLICKBEHAVIOUR {
        REPLACE,
        REPLACE_AND_PLAY,
        APPEND_CUR,
        APPEND_AND_PLAY,
        APPEND_OTHER,
        SEND_TRANSC,
        EDIT_TAGS,
    };

	tageditor* tagedit;
    mwindow* win;
    workbench* wb;
    QMenu* ctx;
    QTreeWidget* tree;
    vars *jag;

    CLICKBEHAVIOUR mclick;
    CLICKBEHAVIOUR dclick;

    void showctx(const QPoint& pos);
    void recurtree(QTreeWidgetItem* parent, QStringList levels, QString conditions, QSqlDatabase* db);
    void listchildren(QTreeWidgetItem *item, QStringList *children);
    QStringList extract(QString vars);
private slots:
	void tageditor_spawn();
    void transc_append(bool discard = false);
    void transc_replace();
    void refresh_config();
    void doubleclick(QTreeWidgetItem* item);
    void middleclick(QTreeWidgetItem* item);
public slots:
    void populate(QSqlDatabase* db);
signals:
    void dispatch(QStringList* l);
    void transc_dispatch(QStringList* l, bool discard);
    void playlist_set(const QStringList files);
public:
    libtree(mwindow* win, workbench* wb, vars* jag);
    ~libtree();
};

#endif // LIBTREE_HPP
