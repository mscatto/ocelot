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

wizard::wizard(vars *jag) : QDialog(){
    this->jag = jag;
    this->setWindowTitle(QString("First Time Wizard :: OCELOT v")+jag->VERSION);
    this->setModal(true);
    this->setFixedSize(260,380);
    this->l = new QVBoxLayout;
    this->l->setContentsMargins(0,0,0,0);
    this->current = this->gen_start();
    this->l->addWidget(current);

    this->setLayout(this->l);
}

QWidget *wizard::gen_start(){
    QWidget *start = new QWidget();
    QGridLayout *l = new QGridLayout(start);

    QLabel *msg = new QLabel();
    msg->setWordWrap(true);
    msg->setFrameShape(QFrame::Shape::StyledPanel);
    msg->setFrameShadow(QFrame::Shadow::Sunken);
    msg->setFrameStyle(QFrame::Shape::StyledPanel);
    msg->setText("<b>Welcome to Ocelot!</b></br><p>This wizard will help you initialize your libraries "
                 "and main interface. If you wish to skip it, you may close this window.</p></br></br>"
                 "</br><p><i>You can always change these later through the Settings window.</i></p>");

    QPushButton *next = new QPushButton("Next");
    next->setIcon(QIcon::fromTheme("go-next-view"));

    QPushButton *skip = new QPushButton("Skip");
    skip->setIcon(QIcon::fromTheme("dialog-close"));

    connect(skip, &QPushButton::clicked, this, &QDialog::close);
    connect(next, &QPushButton::clicked, this, &wizard::switch_libs);

    l->addWidget(msg,0,0,1,2);
    l->addWidget(skip,1,0,1,1);
    l->addWidget(next,1,1,1,1);

    start->setLayout(l);

    return start;
}

QWidget *wizard::gen_libs(){
    QWidget *libs = new QWidget();
    QGridLayout *x = new QGridLayout(libs);

    QPushButton *next = new QPushButton("Next");
    next->setIcon(QIcon::fromTheme("go-next-view"));

    x->addWidget(new QTreeWidget,0,0,1,2);
    x->addWidget(next,1,0,1,2);

    connect(next, &QPushButton::clicked, this, &wizard::switch_ui);

    libs->setLayout(x);
    return libs;
}

QWidget *wizard::gen_ui(){
    QWidget *ui = new QWidget();
    QGridLayout *y = new QGridLayout(ui);

    QPushButton *next = new QPushButton("Finish");
    next->setIcon(QIcon::fromTheme("dialog-ok-apply"));

    y->addWidget(new QLabel("UI"),0,0,1,2);
    y->addWidget(next,1,0,1,2);

    connect(next, &QPushButton::clicked, this, &QDialog::close);

    ui->setLayout(y);
    this->show_mwindow();
    return ui;
}

void wizard::switch_start(){
    this->current->~QWidget();
    this->current = this->gen_start();
    this->l->addWidget(this->current);
}

void wizard::switch_libs(){
    this->current->~QWidget();
    this->current = this->gen_libs();
    this->l->addWidget(this->current);
}

void wizard::switch_ui(){
    this->current->~QWidget();
    this->current = this->gen_ui();
    this->l->addWidget(this->current);
}

void wizard::closeEvent(QCloseEvent *event){
    this->jag->DB_REF->exec("UPDATE data SET val = 0 WHERE var LIKE 'general_runwizard'");
    this->show_mwindow();
    QDialog::closeEvent(event);
}

wizard::~wizard(){

}


