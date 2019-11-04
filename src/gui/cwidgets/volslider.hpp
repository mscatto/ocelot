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

#ifndef volslider_HPP
#define volslider_HPP

#include "src/vars.hpp"
#include "src/player.hpp"

#include <QDateTime>
#include <QDebug>
#include <QMouseEvent>
#include <QSlider>
#include <QStyleOptionSlider>
#include <QToolTip>

class volslider : public QSlider {
    // Q_OBJECT
public:
    volslider(vars *jag);
    ~volslider();
public slots:
    void rotate(Qt::Orientation o);
private:
    vars* jag;
    player* pctx;
    const uint MINSIZE = 64;
    const int SAVEDELAY = 1000;
    QTimer savetimer;
private slots:
    void start_save();
    void save_vol();
protected:
    void mousePressEvent(QMouseEvent* event) {
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

            // spawns a tooltip with volume number
            /*QPoint* p = new QPoint(this->mapToGlobal(this->pos()));

            if(this->orientation() == Qt::Horizontal)
                p->setY(QCursor::pos().ry());
            else
                p->setX(QCursor::pos().rx() * 2);
            QToolTip::showText(*p, QString::number(this->value()), this);
            p->~QPoint();*/
        } else {
            emit event->accept();
            QSlider::mousePressEvent(event);
        }
        // emit onClick(this->value());
    }
};

#endif // volslider_HPP
