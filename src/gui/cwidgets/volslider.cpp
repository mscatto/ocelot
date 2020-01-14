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

#include "src/gui/cwidgets/volslider.hpp"
#include "src/gui/cwidgets/toolbar.hpp"

#include <QMouseEvent>
#include <QPainter>
#include <QToolTip>

volslider::volslider(vars *jag) : QSlider() {
    this->jag = jag;
    this->setOrientation(Qt::Horizontal);
    this->pctx = jag->pctx;
    this->setObjectName("volume_widget");
    this->setTracking(true);
    this->setMinimumWidth(48);
    this->setRange(0, 10);
    this->setToolTip("Volume");
    this->setInvertedControls(false);
    this->setTickInterval(1);
    this->setTickPosition(TickPosition::TicksBothSides);
    connect(this, &volslider::valueChanged, this, &volslider::start_save);
    connect(&this->savetimer, &QTimer::timeout, this, &volslider::save_vol);
    this->setValue(this->jag->fetchdbdata("general_volume").toInt());
	//this->setStyleSheet(this->HSTYLESHEET);
}

volslider::~volslider() {
}

void volslider::rotate(Qt::Orientation o) {
    this->setOrientation(o);
    if(o == Qt::Vertical) {
        this->setMinimumSize(0, int(MINSIZE));
        this->setInvertedControls(true);
    } else {
        this->setMinimumSize(int(MINSIZE), 0);
        this->setInvertedControls(false);
    }
}

void volslider::start_save(){
    this->savetimer.start(SAVEDELAY);
}

void volslider::save_vol(){
    this->jag->setdbdata("general_volume",this->value());
}

void volslider::paintEvent(QPaintEvent *ev){
    /*QPainter painter(this);
    QPolygon polygon;

    painter.setRenderHint(QPainter::RenderHint::SmoothPixmapTransform);
    painter.setBrush(QBrush(QColor(226,226,226,255), Qt::BrushStyle::SolidPattern));
    painter.setPen(QPen(QColor(96,96,96,200), 0.6, Qt::PenStyle::SolidLine, Qt::PenCapStyle::RoundCap));
    polygon << QPoint(4, this->height()-4)
            << QPoint(this->width()-8, this->height()-4)
            << QPoint(this->width()-8, 3);

    painter.drawPolygon(polygon, Qt::FillRule::WindingFill);*/

    QSlider::paintEvent(ev);
}

void volslider::mousePressEvent(QMouseEvent* event){
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    if(event->button() == Qt::LeftButton && !sr.contains(event->pos())) {
        int val;
        if(orientation() == Qt::Vertical) {
            double hhandle_h = (0.5 * sr.height()) + 0.5;
            int posy = height() - event->y();
            if(posy < hhandle_h)
                posy = int(hhandle_h);
            if(posy > height() - hhandle_h)
                posy = int(height() - hhandle_h);
            double nh = (height() - hhandle_h) - hhandle_h;
            double npos = (posy - hhandle_h) / nh;

            val = int(minimum() + (maximum() - minimum()) * npos);
        } else {
            double hhandle_w = (0.5 * sr.width()) + 0.5;
            int posx = event->x();
            if(posx < hhandle_w)
                posx = int(hhandle_w);
            if(posx > width() - hhandle_w)
                posx = int(width() - hhandle_w);
            double nw = (width() - hhandle_w) - hhandle_w;
            double npos = (posx - hhandle_w) / nw;

            val = int(minimum() + ((maximum() - minimum()) * npos));
        }

        if(val > this->maximum() || val < 0)
            return;

        if(this->invertedAppearance())
			this->setValue(this->maximum() - val);
        else
            this->setValue(val);

        emit this->pctx->setVolume(ushort(val));
        emit this->sliderMoved(ushort(val));
        emit event->accept();
        QSlider::mousePressEvent(event);

    } else {
        emit event->accept();
        QSlider::mousePressEvent(event);
    }
    // emit onClick(this->value());
}
