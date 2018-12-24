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
