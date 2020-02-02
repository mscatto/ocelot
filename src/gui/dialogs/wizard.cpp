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

#include "wizard.hpp"

#include <QSqlQuery>
#include <QDialogButtonBox>

wizard::wizard (mwindow *win, vars *jag) : QDialog() {
	this->jag = jag;
	this->win = win;
	this->setWindowTitle (QString ("First Time Wizard :: OCELOT v") + jag->VERSION);
	this->setModal (true);
	this->setFixedSize (520, 300);
	this->l = new QVBoxLayout;
	this->l->setContentsMargins (0, 0, 0, 0);
	this->current = this->gen_start();
	this->l->addWidget (current);

	this->setLayout (this->l);
}

QWidget *wizard::gen_start() {
	QWidget *start = new QWidget();
	QGridLayout *l = new QGridLayout (start);

	QLabel *msg = new QLabel();
	msg->setWordWrap (true);
	msg->setFrameShape (QFrame::Shape::StyledPanel);
	msg->setFrameShadow (QFrame::Shadow::Sunken);
	msg->setFrameStyle (QFrame::Shape::StyledPanel);
	msg->setAlignment (Qt::AlignTop | Qt::AlignLeading);
	msg->setText ("<h1>Welcome to Ocelot!</h1><p>This wizard will help you initialize your libraries "
				  "and main interface.<br>If you wish to skip it, you may close this window.</p>"
				  "<br><br><br><br><br><br><br><i>You can always change these later through the Settings window.</i>");

	QPushButton *next = new QPushButton ("Next");
	next->setIcon (QIcon::fromTheme ("go-next-view"));

	QPushButton *skip = new QPushButton ("Skip");
	skip->setIcon (QIcon::fromTheme ("dialog-close"));

	connect (skip, &QPushButton::clicked, this, &QDialog::close);
	connect (next, &QPushButton::clicked, this, &wizard::switch_libs);

	l->addWidget (msg, 0, 0, 1, 2);
	l->addWidget (skip, 1, 0, 1, 1);
	l->addWidget (next, 1, 1, 1, 1);

	start->setLayout (l);

	return start;
}

QWidget *wizard::gen_libs() {
	QWidget *libs = new QWidget();
	QGridLayout *x = new QGridLayout (libs);
	QLabel *label =
		new QLabel ("<b>Managed Directories</b><br><i>You can also add them later through the settings dialog.</i>");

	QPushButton *next = new QPushButton ("Next");
	next->setIcon (QIcon::fromTheme ("go-next-view"));
	QWidget *w = this->win->sdiag->spawn_dirtab (this->win);  // tapping the settings pane
	static_cast<QGridLayout *> (w->layout())->addWidget (next, 4, 3, 1, 1);
	static_cast<QGridLayout *> (w->layout())->setContentsMargins (0, 0, 0, 0);

	x->addWidget (label, 0, 0, 1, 4);
	x->addWidget (w, 1, 0, 1, 4);

	connect (next, &QPushButton::clicked, this, &wizard::switch_ui);

	libs->setLayout (x);
	return libs;
}

QWidget *wizard::gen_ui() {
	QWidget *ui = new QWidget();
	QGridLayout *x = new QGridLayout (ui);
	QLabel *label = new QLabel (
		"<b>UI Layout</b><br><i>You can edit it later by enabling the layout editor and right-clicking.</i>");
	label->setAlignment (Qt::AlignTop | Qt::AlignLeading);
	QGroupBox *bg = new QGroupBox();

	QRadioButton *l1 = new QRadioButton ("Layout 1");
	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget (l1);
	bg->setLayout (vbox);

	QPushButton *next = new QPushButton ("Finish");
	next->setIcon (QIcon::fromTheme ("dialog-ok-apply"));

	x->addWidget (label, 0, 0, 1, 4);
	x->addWidget (bg, 1, 0, 5, 4);
	x->addWidget (next, 6, 3, 1, 1);

	connect (next, &QPushButton::clicked, this, &QDialog::close);

	ui->setLayout (x);
	this->show_mwindow();
	return ui;
}

void wizard::switch_start() {
	this->current->~QWidget();
	this->current = this->gen_start();
	this->l->addWidget (this->current);
}

void wizard::switch_libs() {
	this->current->~QWidget();
	this->current = this->gen_libs();
	this->l->addWidget (this->current);
}

void wizard::switch_ui() {
	this->current->~QWidget();
	this->current = this->gen_ui();
	this->l->addWidget (this->current);
}

void wizard::closeEvent (QCloseEvent *event) {
	this->jag->DB_REF->exec ("UPDATE data SET val = 0 WHERE var LIKE 'general_runwizard'");
	this->show_mwindow();
	QDialog::closeEvent (event);
}

wizard::~wizard() {}
