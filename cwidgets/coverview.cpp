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

#include "coverview.hpp"
#include "src/mwindow.hpp"
#include <QMainWindow>
#include <QLabel>
#include <QFrame>
#include <QPainter>

coverview::coverview(mwindow *win, workbench *wb) : QLabel(win){
    this->setMinimumSize(200, 200);
    this->setAutoFillBackground(true);
    this->setScaledContents(true);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->win = win;
    this->wb = wb;

    this->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    this->setFrameStyle(QFrame::Sunken);
    this->setFrameShape(QFrame::StyledPanel);
    this->setFrameShadow(QFrame::Sunken);

    this->setText("<b>[No Media]</b>");

    this->ctx = new QMenu();
    this->ctx->setTitle("Cover View");
    this->ctx->addAction("Set cover...");
    this->ctx->addAction("Keep cover ratio");

    connect(this, &QLabel::customContextMenuRequested, this, &coverview::showctx);
    connect(win, &mwindow::coverchanged, this, &coverview::coverchanged);
    connect(win, &mwindow::clearcover, this, &coverview::clear);
}

void coverview::showctx(QPoint pos){
    if(this->wb->islocked())
        this->ctx->exec(this->mapToGlobal(pos));
    else{
        this->wb->setlastctx(this);
        this->wb->ctx_req(this->mapTo(this->wb, pos));
    }
}

void coverview::coverchanged(QPixmap *nc){
    this->setPixmap(*nc);
}

void coverview::clear(){
    this->setText("<b>[No Media]</b>");
}

coverview::~coverview(){

}
