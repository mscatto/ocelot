#include "tageditor.hpp"

#include <QGridLayout>
#include <QButtonGroup>
#include <QTableView>
#include <taglib/tag.h>
#include <taglib/fileref.h>

tageditor::tageditor(QWidget *parent, vars *jag) : QDialog(parent){
    this->queue = new QStringList();
    QGridLayout *l = new QGridLayout(this);

    this->setLayout(l);
    this->setWindowTitle(QString("Editing ")+QString::number(this->queue->size())+" items :: OCELOT v"+jag->VERSION);
    this->setFixedSize(500,300);
    this->setModal(true);
}

tageditor::~tageditor(){}
