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
#include "src/gui/mwindow.hpp"
#include <QFrame>
#include <QLabel>
#include <QMainWindow>
#include <QPainter>

coverview::coverview(mwindow* win, workbench* wb) : QLabel(win) {
	this->win = win;
	this->wb = wb;

	this->init();
}

void coverview::init(){
	this->setObjectName("coverview");
	this->setText("<b>[No Media]</b>");
	this->setMinimumSize(200, 200);
	this->setAutoFillBackground(true);
	this->setContextMenuPolicy(Qt::CustomContextMenu);
	this->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	this->setFrameStyle(QFrame::Sunken);
	this->setFrameShape(QFrame::StyledPanel);
	this->setFrameShadow(QFrame::Sunken);

	this->cover = QPixmap();

	this->ctx = new QMenu();
	this->ctx->setTitle("Cover View");
	this->ctx->addAction("Set cover...");
	this->ctx->addAction("Keep cover ratio");

	connect(this, &QLabel::customContextMenuRequested, this, &coverview::showctx);
	connect(this->win, &mwindow::cover_set, this, &coverview::set);
	// connect(win, &mwindow::clearcover, this, &coverview::clear);
}

void coverview::showctx(QPoint pos) {
    if(this->wb->islocked()) {
        this->ctx->exec(this->mapToGlobal(pos));
    } else {
        this->wb->setlastctx(this);
        this->wb->ctx_req(this->mapTo(this->wb, pos));
    }
}

void coverview::set(QPixmap cover) {
    this->cover = cover;
    this->setPixmap(this->cover.scaled(this->width(),this->height(),Qt::KeepAspectRatio));
}

void coverview::clear() {
    this->setText("<b>[No Media]</b>");
}

coverview::~coverview() {
}

void coverview::resizeEvent(QResizeEvent *event){
    event->accept();
    if(this->cover.isNull())
        return;
    this->setPixmap(this->cover.scaled(this->width(),this->height(),Qt::KeepAspectRatio));

}
