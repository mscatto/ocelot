#include "about.hpp"

#include <QDialog>
#include <QGridLayout>
#include <QSplitter>

about::about(QWidget *parent, vars *jag) : QDialog(parent){
    QGridLayout *l = new QGridLayout(this);
    QSplitter *s = new QSplitter(this);
    l->addWidget(s);

    this->setLayout(l);
    this->setWindowTitle("About Ocelot");
    this->setFixedSize(500,300);
    this->setModal(true);
}

about::~about(){

}
