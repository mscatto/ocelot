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

#ifndef TOOLBAR_HPP
#define TOOLBAR_HPP

#include "src/gui/cwidgets/progslider.hpp"
//#include "src/gui/cwidgets/splitter.hpp"
#include "src/gui/cwidgets/volslider.hpp"
#include "src/vars.hpp"
#include <QSplitter>
#include <QToolBar>

class toolbar : public QToolBar {
    Q_OBJECT
private:
	enum PROGDSP {REMAINING, ELAPSED, REMTOTAL, ELAPTOTAL};
	void init();

	vars* jag;
	QWidget *win;

	QSplitter* progvol;
	progslider* prog;
	volslider* vol;
	QMenu* conf;
	QMenu* lmenu;
	QLabel* progdsp;

	QString DSPSEP;
    QList<QWidget*> docked;
	PROGDSP dspmode = PROGDSP::ELAPTOTAL;
    QList<QAction*> *acts;
public:
    toolbar(QWidget* win, QMenu* conf, progslider* prog, volslider* vol, vars* jag);
	~toolbar(){};
    QByteArray splitterstate();
    void restore_splitterstate(QByteArray ba);
public slots:
    void rotate(Qt::Orientation o);
private slots:
	void update_progdsp(int position);
	void update_dspmode(QAction *act);
	void show_labelmenu();
    void on_progvol_resize();
    void store_volsize();
    void menu_show();
};

#endif // TOOLBAR_HPP
