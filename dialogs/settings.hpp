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

#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <QObject>
#include <QWidget>
#include <QDialog>


#include "src/vars.hpp"

class settings : public QDialog
{
private:
    const uint APPEND=0;
    const uint REPLACE=1;
    const uint NOTHING=2;

    const uint PLAY=0;
    const uint IFIDLE=1;
    const uint DONTPLAY=2;

    vars *jag;
    QWidget *win;
    QTreeWidget *libstree;
    QWidget* spawn_maintab(QWidget *win);
    QWidget* spawn_libtab(QWidget *win);

private slots:
    void lib_dirpicker();
    void lib_treerefresh();
    void lib_remdir();

    void gen_mdclick(int index);
    void gen_mmclick(int index);
    void gen_mappend(int index);

    void thread_libscan();
    void thread_newlib(QString lib);
public:
    settings(QWidget *parent, vars *jag);
    ~settings();
};

#endif // SETTINGS_HPP
