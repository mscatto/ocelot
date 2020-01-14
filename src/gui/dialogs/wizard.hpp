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

#ifndef WIZARD_H
#define WIZARD_H

#include <QDialog>
#include "src/vars.hpp"
#include "src/gui/mwindow.hpp"

class wizard : public QDialog
{
    Q_OBJECT
private:
    vars *jag;
	mwindow *win;
    void closeEvent(QCloseEvent *event);
    QVBoxLayout *l;
    QWidget *gen_start();
    QWidget *gen_libs();
    QWidget *gen_ui();
    QWidget *current;
public:
	wizard(mwindow* win, vars *jag);
    ~wizard();
private slots:
    //void on_close();
    void switch_start();
    void switch_libs();
    void switch_ui();
signals:
    void show_mwindow();
};

#endif // WIZARD_H
