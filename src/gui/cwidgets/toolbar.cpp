﻿/*
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

#include "toolbar.hpp"
#include "../mwindow.hpp"
#include "src/gui/cwidgets/splitter.hpp"
#include <QMenu>
#include <QToolBar>
#include <QToolButton>

toolbar::toolbar(QWidget* win, QMenu* conf, progslider* prog, volslider* vol, vars* jag) : QToolBar(win) {
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setObjectName("toolbar");
	this->progvol = new splitter(Qt::Horizontal, static_cast<mwindow*>(win), static_cast<mwindow*>(win)->wb);
    this->progvol->setObjectName("progvol_splitter");
    this->progvol->setChildrenCollapsible(false);
    this->progvol->setContentsMargins(0,0,0,0);
    this->vol = vol;
    this->prog = prog;
    this->jag = jag;
	this->DSPSEP = " | ";
	this->dspmode = PROGDSP(this->jag->fetchdbdata("toolbar_dspmode").toInt());
	this->progdsp = new QLabel();
	this->progdsp->setMinimumWidth(88);
	this->lmenu = new QMenu("Progress Viewer");

	QActionGroup *inf = new QActionGroup(this);
	QAction *x;
	x = new QAction("Time remaining");
	x->setCheckable(true);
	x->setData(PROGDSP::REMAINING);
	inf->addAction(x);

	x = new QAction("Time elapsed");
	x->setCheckable(true);
	x->setData(PROGDSP::ELAPSED);
	inf->addAction(x);

	x = new QAction("Time remaining out of total");
	x->setCheckable(true);
	x->setData(PROGDSP::REMTOTAL);
	inf->addAction(x);

	x = new QAction("Time elapsed out of total");
	x->setCheckable(true);
	x->setData(PROGDSP::ELAPTOTAL);
	inf->addAction(x);

	inf->actions().at(this->dspmode)->setChecked(true);
	connect(this->lmenu, &QMenu::triggered, this, &toolbar::update_dspmode);

	inf->setExclusive(true);
	this->lmenu->addSection("Information shown");
	this->lmenu->addActions(inf->actions());
	this->lmenu->addSeparator();
	this->lmenu->addAction("Hide from toolbar");
	this->lmenu->actions().last()->setData(4);

	this->progdsp->setMargin(1);
	this->progdsp->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	this->progdsp->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
	connect(this->progdsp, &QLabel::customContextMenuRequested, this, &toolbar::show_labelmenu);

    QWidget* volw = new QWidget;
	QGridLayout* voll = new QGridLayout;
    voll->setSpacing(0);
    voll->setContentsMargins(6, 1, 6, 1);
    voll->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    voll->addWidget(this->vol);
    volw->setLayout(voll);
    volw->setObjectName("volume_encaps");

    QWidget* progw = new QWidget;
    QGridLayout* progl = new QGridLayout;
    progl->setSpacing(0);
    progl->setContentsMargins(6, 1, 6, 1);
    progl->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    progl->addWidget(this->prog);
    progw->setLayout(progl);
    progw->setObjectName("prog_encaps");

    this->progvol->addWidget(volw);
    this->progvol->addWidget(progw);

    conf = new QMenu(qobject_cast<mwindow*>(win));
    conf->addAction(new QAction("Open File..."));
    conf->addAction(new QAction("Play Disc..."));
    conf->addAction(new QAction("Play from network..."));
    conf->addSeparator();
    conf->addAction(new QAction("Settings"));
    conf->addSeparator();
    conf->addAction(new QAction("Toggle Layout Editor"));
    conf->addSeparator();
    conf->addAction(new QAction("About"));
    conf->addAction(new QAction("Exit"));

    this->addAction(new QAction(QIcon(":/ui/start"), "Resume media playback"));
    this->addAction(new QAction(QIcon(":/ui/pause"), "Pause media playback"));
    this->addAction(new QAction(QIcon(":/ui/stop"), "Stop media playback"));
    this->addAction(new QAction(QIcon(":/ui/prev"), "Previous on playlist"));
    this->addAction(new QAction(QIcon(":/ui/next"), "Next on playlist"));
    this->addAction(new QAction(QIcon(":/ui/cog"), "Tweak settings"));

    QList<QAction*> act = this->actions();
    connect(act.at(0), &QAction::triggered, qobject_cast<mwindow*>(win), &mwindow::toolbar_play);
    connect(act.at(1), &QAction::triggered, qobject_cast<mwindow*>(win), &mwindow::toolbar_pause);
    connect(act.at(2), &QAction::triggered, qobject_cast<mwindow*>(win), &mwindow::toolbar_stop);
    connect(act.at(3), &QAction::triggered, qobject_cast<mwindow*>(win), &mwindow::toolbar_prev);
    connect(act.at(4), &QAction::triggered, qobject_cast<mwindow*>(win), &mwindow::toolbar_next);
    //connect(act.at(5), &QAction::triggered, qobject_cast<mwindow*>(win), &mwindow::toolbar_menu);

    QToolButton *gear = static_cast<QToolButton*>(this->widgetForAction(act.last()));
    gear->setMenu(conf);
    gear->setArrowType(Qt::ArrowType::NoArrow);
    gear->setPopupMode(QToolButton::ToolButtonPopupMode::InstantPopup);

    //connect(gear, &QToolButton::triggered, gear, &QToolButton::showMenu);
    connect(conf->actions().at(4), &QAction::triggered, qobject_cast<mwindow*>(win), &mwindow::config_spawn);
    connect(conf->actions().at(6), &QAction::triggered, qobject_cast<mwindow*>(win), &mwindow::uilock_flip);
    connect(conf->actions().at(8), &QAction::triggered, qobject_cast<mwindow*>(win), &mwindow::about_spawn);
    connect(this, &toolbar::orientationChanged, this, &toolbar::rotate);
    connect(this->progvol, &QSplitter::splitterMoved, this, &toolbar::on_progvol_resize);
	connect(this->prog, &QSlider::valueChanged, this, &toolbar::update_progdsp);

    // btn->setAutoRaise(true);
    this->addSeparator();
    this->addWidget(this->progvol);
	this->addWidget(this->progdsp);

	emit this->prog->valueChanged(0);
}

toolbar::~toolbar() {
}

QByteArray toolbar::splitterstate() {
    return this->progvol->saveState();
}

void toolbar::restore_splitterstate(QByteArray ba) {
    this->progvol->restoreState(ba);
}

void toolbar::rotate(Qt::Orientation o) {
	if(o==Qt::Horizontal){
		this->DSPSEP=" | ";
		this->progdsp->setMinimumWidth(88);
	}else{
		this->DSPSEP="<br>-<br>";
		this->progdsp->setMinimumWidth(38);
	}
    this->prog->rotate(o);
    this->vol->rotate(o);
	this->progvol->setOrientation(o);
	this->update_progdsp(this->prog->value());
}

void toolbar::update_progdsp(int position){
	switch(dspmode){
		case PROGDSP::REMAINING:
			this->progdsp->setText(QDateTime::fromTime_t(this->prog->maximum()-position).toString("<b>mm:ss</b>"));
		break;
		case PROGDSP::ELAPSED:
			this->progdsp->setText(QDateTime::fromTime_t(position).toString("<b>mm:ss</b>"));
		break;
		case PROGDSP::REMTOTAL:
			this->progdsp->setText(QDateTime::fromTime_t(this->prog->maximum()-position).toString("<b>mm:ss</b>")+this->DSPSEP+QDateTime::fromTime_t(this->prog->maximum()).toString("<b>mm:ss</b>"));
		break;
		case PROGDSP::ELAPTOTAL:
			this->progdsp->setText(QDateTime::fromTime_t(position).toString("<b>mm:ss</b>")+this->DSPSEP+QDateTime::fromTime_t(this->prog->maximum()).toString("<b>mm:ss</b>"));
		break;
		default:
			this->jag->PANIC("at 'toolbar::update_progdsp': arg out of index");
		break;
	}
}

void toolbar::update_dspmode(QAction* act){
	if(act->data().toInt()==4) // the hide option
		return;

	this->dspmode = PROGDSP(act->data().toInt());
	this->update_progdsp(this->prog->value());
	this->jag->setdbdata("toolbar_dspmode", this->dspmode);
}

void toolbar::show_labelmenu(){
	this->lmenu->exec(QPoint(QCursor::pos().x(), QCursor::pos().y()+10));
}

void toolbar::on_progvol_resize() {
}

void toolbar::store_volsize() {
    this->jag->setdbdata("ui_volsize", this->vol->width());
}

void toolbar::menu_show(){
    this->conf->showTearOffMenu();
}