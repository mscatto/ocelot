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

#include "transcoder.hpp"

#include <QDialog>
#include <QGridLayout>
#include <QSplitter>

#include "src/mwindow.hpp"


transcoder::transcoder(vars *jag, QWidget *win) : QDialog(win){
    QGridLayout *l = new QGridLayout(this);
    QSplitter *s = new QSplitter(this);
    l->addWidget(s);

    this->setLayout(l);
    this->setWindowTitle(QString("Transcoder :: OCELOT v")+jag->VERSION);
    this->setFixedSize(900,560);
    this->setModal(true);

    connect(qobject_cast<mwindow*>(win), &mwindow::convhandler, this, &transcoder::handle);
}

transcoder::~transcoder(){

}

void transcoder::handle(QStringList files){
    QStringList()<<files;
}


