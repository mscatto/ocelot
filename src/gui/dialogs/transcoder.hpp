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

#ifndef TRANSCODER_HPP
#define TRANSCODER_HPP

#include <QObject>
#include <QWidget>
#include <QDialog>

#include "../../vars.hpp"
#include "transchelper.hpp"

class transcoder : public QDialog
{
    Q_OBJECT
private:
    bool qpreset = false;
    QStringList *input;
    QTreeWidget *inlist;
    QTreeWidget *outlist;
    QProgressBar *prog;
    transchelper *helper;

    QPushButton *preset_add;
    QPushButton *preset_rem;
    QPushButton *preset_adv;
    QComboBox *preset_cbx;
    QGroupBox *preset_gbx;

    QWidget *init_prev();
    QWidget *init_input();
    QWidget *init_enc();
    QWidget *init_output();
    QWidget *init_conf();
private slots:
    void flip_qpreset();
    void refresh_output();
public:
    transcoder(vars *jag, QWidget *win);
    ~transcoder();
public slots:
    void clear();
    void append(QStringList *files);
    void replace(QStringList *files);
    void spawnhelper();
signals:

};

#endif // TRANSCODER_HPP
