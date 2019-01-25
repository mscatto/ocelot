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

#include "tageditor.hpp"

#include <QGridLayout>
#include <QTableWidget>
#include <QDialogButtonBox>
#include <QButtonGroup>
#include <QHeaderView>
#include <QTableView>
#include <taglib/tag.h>
#include <taglib/fileref.h>

tageditor::tageditor(QWidget *parent, vars *jag) : QDialog(parent){
    this->setWindowTitle(QString("Editing tags from ")+QString::number(0)+" items :: OCELOT v"+jag->VERSION);
    this->setFixedSize(400,500);
    this->setModal(false);
    queue = new QStringList;

    QGridLayout *ml = new QGridLayout();
    QGridLayout *fl = new QGridLayout();
    QFrame *frame = new QFrame();
    frame->setLayout(fl);
    fl->setContentsMargins(0,0,0,0);

    QDialogButtonBox *bb = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Discard);
    QTableWidget *tbl = new QTableWidget();
    tbl->setRowCount(jag->dumpvars().count());
    tbl->setColumnCount(1);
    tbl->setColumnWidth(0,264);
    tbl->setHorizontalHeaderLabels(QStringList()<<"Tag Data");
    tbl->setVerticalHeaderLabels(jag->dumpvars());
    for(int i=0;i<jag->dumpvars().count();i++){
        //qDebug() << tbl->verticalHeaderItem(i)->text();
    }

    tbl->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tbl->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    fl->addWidget(tbl);

    ml->addWidget(frame);
    ml->addWidget(bb);
    this->setLayout(ml);
}

tageditor::~tageditor(){

}

void tageditor::init(QStringList *l){
    foreach(QString path, *l){
        qDebug() << path;
    }

}
