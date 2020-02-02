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

#ifndef TABBUTTON_HPP
#define TABBUTTON_HPP

#include <QPushButton>

class tabbutton : public QPushButton {
	Q_OBJECT
  private:
	QString mapkey;

  public:
	tabbutton (const QIcon &icon, QString key);
	~tabbutton(){};
	void set_key (QString nkey);
  public slots:
	void slot_clicked (bool checked = false);
  signals:
	void idclicked (QString key);
};

#endif	// TABBUTTON_HPP
