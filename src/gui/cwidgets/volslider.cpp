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
    this->setMinimumWidth(32);
    this->setRange(0, 10);
    this->setToolTip("Volume");
    this->setInvertedControls(false);
    connect(this, &volslider::valueChanged, this, &volslider::start_save);
    connect(&this->savetimer, &QTimer::timeout, this, &volslider::save_vol);

    this->setValue(this->jag->fetchdbdata("general_volume").toInt());
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
