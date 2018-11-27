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
#ifndef PROGSLIDER_HPP
#define PROGSLIDER_HPP

#include <QSlider>
#include <QDateTime>
#include <QToolTip>
#include <QMouseEvent>
#include "toolbar.hpp"

class progslider : public QSlider
{
protected:
    void mousePressEvent(QMouseEvent *event){
        if (event->button() == Qt::LeftButton){
            if (orientation() == Qt::Vertical)
                setValue(minimum() + ((maximum()-minimum()) * (height()-event->y())) / height()) ;
            else
                setValue(minimum() + ((maximum()-minimum()) * event->x()) / width()) ;

            QPoint *p = new QPoint(this->mapToGlobal(this->pos()));
            p->setX(QCursor::pos().rx());
            QToolTip::showText(*p, QDateTime::fromTime_t(unsigned(this->value())).toString("mm : ss"), this);
            p->~QPoint();
            event->accept();
        }
        QSlider::mousePressEvent(event);
    }
public slots:
    void rotate(Qt::Orientation o);
public:
    progslider(Qt::Orientation o, toolbar *bar, QWidget *parent = nullptr);
    ~progslider();
};

#endif // PROGSLIDER_HPP
