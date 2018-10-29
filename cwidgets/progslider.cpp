/*
 * Ocelot Music Manager: music player and library manager built using Qt
 * Copyright (C) 2018 Matheus Scattolin Anselmo
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
