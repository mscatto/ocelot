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
    QTimer savetimer;
    const uint MINSIZE = 64;
    const int SAVEDELAY = 1000;
    const QString VSTYLESHEET = "";
    const QString HSTYLESHEET = "\
        QSlider::handle:horizontal{\
            border: 2px solid #8c8c8c;\
            background: solid #3c3c3c;\
            height: 16px;\
            width: 8px;\
            margin: -8px 8px;\
            }";
private slots:
    void start_save();
    void save_vol();
protected:
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent* event);
};
#endif // volslider_HPP
