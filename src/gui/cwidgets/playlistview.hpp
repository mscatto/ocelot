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

#ifndef PLAYLISTVIEW_HPP
#define PLAYLISTVIEW_HPP

#include "../mwindow.hpp"
#include "../workbench.hpp"
#include "playlist.hpp"
#include <QObject>
#include <QTabWidget>
#include <QToolButton>
#include <QWidget>

class playlistview : public QTabWidget {
    Q_OBJECT
private:
    vars* jag;
    mwindow* win;
    workbench* wb;

    QMap<QString, playlist*> pl;
    playlist* plactive;  /* the one on display */
    playlist* plplaying; /* from currently playing track */
    QString order;
    QMenu* headermenu;
    QMenu* ctx;

    QDialog* renamer;
    QLineEdit* namebox;
    QString tmpkey;
    QTreeWidgetItem* currentitem;
    QToolButton* addbtn;

    void showctx(const QPoint& pos);
    // void ctxmenu(const QPoint &pos);
    void newplaylist();
    // QMap<QString, QAction*> menuctx;
public:
    playlistview(vars* jag, mwindow* win, workbench* wb);
    ~playlistview();
public slots:
    void playlist_append(const QStringList files, const int playlist);
    void playlist_replace(const QStringList files, const int playlist);

    void context(QPoint p);
    void playing(QString f);
    void refreshpl();
    void next();
    void prev();
    void medistatus(int status);
    void swapitem(QTreeWidgetItem* item);
private slots:
    void toggler(bool checked);
	void tab_close(int index);
	void renamer_ok();
	void renamer_cancel();
	void tabbutton_rename(QString key);
    void tab_switch(int index);
signals:
    void clearsel();
    void clearpl();
    void exportpl();
};

/*class tabbar : public QTabBar
{
	Q_OBJECT
	public:
		tabbar(){};
		virtual ~tabbar() {}

	protected:
	   //QSize tabSizeHint (int) const {  return QSize(70,17);}
	   void paintEvent(QPaintEvent *event);
};*/

#endif // PLAYLISTVIEW_HPP