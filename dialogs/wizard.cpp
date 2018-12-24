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

#include "wizard.h"

#include <QSqlQuery>
#include <QDialogButtonBox>

wizard::wizard(vars *jag) : QDialog(){
    this->jag = jag;
    QGridLayout *l = new QGridLayout(this);
    QTextEdit *msg = new QTextEdit();
    msg->setText("<b>Welcome to Ocelot!</b></br><p>This assistent will help you initialize your libraries "
                 "and main interface. If you wish to skip this process, you may close this window now.</p>"
                 "</br><p><i>You can always change these later through the Settings window.</i></p>");
    l->addWidget(msg,0,0,1,2);
    this->setLayout(l);

    this->setWindowTitle(QString("Quick Start Wizard :: OCELOT v")+jag->VERSION);
    this->setFixedSize(600,350);

    QDialogButtonBox *bb = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(bb->button(QDialogButtonBox::Close), &QPushButton::clicked, this, &QDialog::close);
    l->addWidget(bb,1,0);

}

void wizard::closeEvent(QCloseEvent *event){
    this->jag->DB_REF->exec("UPDATE data SET val = 0 WHERE var LIKE 'general_runwizard'");
    QDialog::closeEvent(event);
}

wizard::~wizard(){

}
