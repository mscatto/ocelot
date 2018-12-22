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
