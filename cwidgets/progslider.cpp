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

#include "progslider.hpp"
#include <qdebug.h>
#include <QMouseEvent>

progslider::progslider(Qt::Orientation o, toolbar *bar, QWidget *parent) : QSlider(o, parent){
    this->setRange(0, 100);

    this->setToolTip("Playback progress");
    this->setEnabled(false);
    this->setTracking(true);
    connect(bar, &QToolBar::orientationChanged, this, &QSlider::setOrientation);
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
        margin: -8px 0; \
    }");
}

progslider::~progslider(){

}

//void progslider::mousePressEvent(QMouseEvent *event){
//    this->setValue(event->pos().rx()-8); /* half width of handle */
//    QSlider::mousePressEvent(event);
//}
