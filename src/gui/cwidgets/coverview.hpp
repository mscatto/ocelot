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

#ifndef COVERVIEW_HPP
#define COVERVIEW_HPP

#include "src/gui/mwindow.hpp"
#include "src/gui/workbench.hpp"
#include <QLabel>

class coverview : public QLabel {
	Q_OBJECT
  private:
	void init();

	QPixmap cover;
	mwindow* win;
	QPoint* lastpos;
	QMenu* ctx;
	QMenu* layoutctx;
	workbench* wb;
	// void customMenuRequested(QPoint pos);
	void showctx (QPoint pos);

  public:
	coverview (mwindow* win, workbench* wb);
	~coverview();
	// virtual void paintEvent(QPaintEvent *p);
  protected:
	void resizeEvent (QResizeEvent* event);
	// void paintEvent(QPaintEvent *event);
  public slots:
	void set (QPixmap cover);
	void clear();
  signals:
	void setlastpos (QPoint pos);
};

#endif	// COVERVIEW_HPP
