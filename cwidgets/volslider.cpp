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
#include "volslider.hpp"
#include "src/mwindow.hpp"
#include "toolbar.hpp"
#include <QPainter>
#include <QToolTip>
#include <QMouseEvent>

volslider::volslider(Qt::Orientation o, toolbar *bar, QWidget *parent) : QSlider(o, parent){
    this->setTracking(true);
    this->setMaximumWidth(100);
    this->setRange(0, 100);
    this->setValue(100);
    this->setToolTip("Playback volume");
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

volslider::~volslider(){

}

void volslider::rotate(Qt::Orientation o){
    this->setOrientation(o);
}

//void volslider::mousePressEvent(QMouseEvent *event){
    //this->setValue(event->localPos().toPoint().rx()-8-1); /* half width of handle */
    //QToolTip::showText(QCursor::pos(), QString::number(this->value()), this);
    //QSlider::mousePressEvent(event);
//}

//void volslider::paintEvent(QPaintEvent *event){
    //QWidget::paintEvent(event);
    //create a QPainter and pass a pointer to the device.
    //A paint device can be a QWidget, a QPixmap or a QImage
    //QPainter painter(this);

    //a simple line
    //painter.drawLine(1,1,500,100);

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
