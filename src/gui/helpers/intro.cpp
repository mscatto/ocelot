#include "intro.hpp"

#include <QGridLayout>
#include <QLabel>

intro::intro(QWidget *parent) : QWidget(parent){
    QGridLayout *ml = new QGridLayout;
    this->setLayout(ml);
    ml->setContentsMargins(0,0,0,0);
    QLabel *h = new QLabel("<h2>Welcome to the Ocelot Preset Manager</h2>");
    h->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ml->addWidget(h);
    QLabel *b = new QLabel("Pick any format or select 'Custom' to tailor your own");
    b->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ml->addWidget(b);
}

intro::~intro(){

}
