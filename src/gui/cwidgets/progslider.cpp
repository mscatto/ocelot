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
#include <QMouseEvent>
#include <qdebug.h>

progslider::progslider (Qt::Orientation o, player* p) : QSlider (o) {
	this->setMaximum (0);
	// this->setmi
	this->pctx = p;
	this->setObjectName ("prog_widget");
	this->setToolTip ("Progress");
	this->setEnabled (false);
	this->setTracking (true);
	this->setInvertedControls (false);
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

progslider::~progslider() {}

void progslider::mousePressEvent (QMouseEvent* event) {
	QStyleOptionSlider opt;
	initStyleOption (&opt);
	QRect sr = style()->subControlRect (QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
	if (event->button() == Qt::LeftButton && !sr.contains (event->pos())) {
		int val;
		if (orientation() == Qt::Vertical) {
			double hhandle_h = (0.5 * sr.height()) + 0.5;
			int posy = height() - event->y();
			if (posy < hhandle_h)
				posy = int (hhandle_h);
			if (posy > height() - hhandle_h)
				posy = int (height() - hhandle_h);
			double nh = (height() - hhandle_h) - hhandle_h;
			double npos = (posy - hhandle_h) / nh;

			val = int (minimum() + (maximum() - minimum()) * npos);
		} else {
			double hhandle_w = (0.5 * sr.width()) + 0.5;
			int posx = event->x();
			if (posx < hhandle_w)
				posx = int (hhandle_w);
			if (posx > width() - hhandle_w)
				posx = int (width() - hhandle_w);
			double nw = (width() - hhandle_w) - hhandle_w;
			double npos = (posx - hhandle_w) / nw;

			val = int (minimum() + ((maximum() - minimum()) * npos));
		}

		// case it ever goes out of bounds
		if (val > this->maximum() || val < 0)
			return;

		if (this->invertedAppearance())
			this->setValue (this->maximum() - val);
		else
			this->setValue (val);

		emit this->pctx->seek (val);
		emit this->sliderMoved (val);

		emit event->accept();
		QSlider::mousePressEvent (event);

		// spawns a tooltip with playback position
		/*QPoint* p = new QPoint(this->mapToGlobal(this->pos()));
		p->setX(QCursor::pos().rx());
		QToolTip::showText(*p, QDateTime::fromTime_t(unsigned(val)).toString("mm : ss"), this);
		p->~QPoint();*/
	} else {
		emit event->accept();
		QSlider::mousePressEvent (event);
	}
	// emit onClick(this->value());
}

void progslider::rotate (Qt::Orientation o) {
	this->setOrientation (o);
	if (o == Qt::Vertical) {
		this->setMinimumSize (0, int (MINSIZE));
		this->setInvertedControls (true);
	} else {
		this->setMinimumSize (int (MINSIZE), 0);
		this->setInvertedControls (false);
	}
}

// void progslider::mousePressEvent(QMouseEvent *event){
//    this->setValue(event->pos().rx()-8); /* half width of handle */
//    QSlider::mousePressEvent(event);
//}
