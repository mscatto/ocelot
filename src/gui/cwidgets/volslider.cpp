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

#include "volslider.hpp"
#include "../mwindow.hpp"
#include "toolbar.hpp"
#include <QMouseEvent>
#include <QPainter>
#include <QToolTip>

volslider::volslider(Qt::Orientation o, player* p) : QSlider(o) {
    this->pctx = p;
    this->setObjectName("volume_widget");
    this->setTracking(true);
    this->setMaximum(100);
    this->setMinimumWidth(32);
    this->setRange(0, 100);
    this->setValue(100);
    this->setToolTip("Playback volume");
    this->setInvertedControls(false);
    // connect(bar, &toolbar::orientationChanged, this, &volslider::rotate);
    /*this->setStyleSheet("\
        QSlider::groove:horizontal{\
        position: absolute;\
        border: 1px solid #979797;\
        background: solid #D9D9D9;\
        height: 8px;\
        margin: 6px 0; \
        }QSlider::handle:horizontal{\
        border: 2px solid #777777;\
        background: solid #434343;\
        height: 16px;\
        width: 8px;\
        margin: -8px 0; \
    }");*/
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
    /*if(o == Qt::Horizontal) {
        this->setFixedWidth(128);
        this->setFixedHeight(16);
        this->setStyleSheet("\
            QSlider::groove:horizontal{\
            position: absolute;\
            border: 1px solid #979797;\
            background: solid #D9D9D9;\
            height: 8px;\
            margin: 6px 0; \
            }QSlider::handle:horizontal{\
            border: 2px solid #777777;\
            background: solid #434343;\
            height: 16px;\
            width: 8px;\
            margin: -8px 16px; \
        }");
    } else {
        this->setFixedWidth(16);
        this->setFixedHeight(128);
        this->setStyleSheet("\
            QSlider::groove:vertical{\
            position: absolute;\
            left: 16px;\
            right: 16px;\
            border: 1px solid #979797;\
            background: solid #D9D9D9;\
            margin: 32px -32px; \
            }QSlider::handle:vertical{\
            border: 2px solid #777777;\
            background: solid #434343;\
            height: 12px;\
            width: 32px;\
            margin: 0px -16px; \
        }");
    }*/
    // position: absolute;
}

// void volslider::mousePressEvent(QMouseEvent *event){
// this->setValue(event->localPos().toPoint().rx()-8-1); /* half width of handle */
// QToolTip::showText(QCursor::pos(), QString::number(this->value()), this);
// QSlider::mousePressEvent(event);
//}

// void volslider::paintEvent(QPaintEvent *event){
// QWidget::paintEvent(event);
// create a QPainter and pass a pointer to the device.
// A paint device can be a QWidget, a QPixmap or a QImage
// QPainter painter(this);

// a simple line
// painter.drawLine(1,1,500,100);

/*//create a black pen that has solid line
//and the width is 2.
QPen myPen(Qt::black, 2, Qt::SolidLine);
painter.setPen(myPen);
painter.drawLine(100,100,100,1);

//draw a point
myPen.setColor(Qt::red);
painter.drawPoint(110,110);

//draw a polygon
QPolygon polygon;
polygon << QPoint(130, 140) << QPoint(180, 170)
         << QPoint(180, 140) << QPoint(220, 110)
         << QPoint(140, 100);
 painter.drawPolygon(polygon);

 //draw an ellipse
 //The setRenderHint() call enables antialiasing, telling QPainter to use different
 //color intensities on the edges to reduce the visual distortion that normally
 //occurs when the edges of a shape are converted into pixels
 painter.setRenderHint(QPainter::Antialiasing, true);
 painter.setPen(QPen(Qt::black, 3, Qt::DashDotLine, Qt::RoundCap));
 painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
 painter.drawEllipse(200, 80, 400, 240);*/
//}
