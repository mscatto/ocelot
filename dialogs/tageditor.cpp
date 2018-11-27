#include "tageditor.hpp"

#include <QGridLayout>
#include <QTableWidget>
#include <QDialogButtonBox>
#include <QButtonGroup>
#include <QHeaderView>
#include <QTableView>
#include <taglib/tag.h>
#include <taglib/fileref.h>

void tageditor::init(QStringList *l){
    foreach(QString path, *l){
        qDebug() << path;
    }
}

tageditor::tageditor(QWidget *parent, vars *jag) : QDialog(parent){
    this->setWindowTitle(QString("Editing tags from ")+QString::number(0)+" items :: OCELOT v"+jag->VERSION);
    this->setFixedSize(400,500);
    this->setModal(false);

    QGridLayout *ml = new QGridLayout();
    QGridLayout *fl = new QGridLayout();
    QFrame *frame = new QFrame();
    frame->setLayout(fl);
    fl->setContentsMargins(0,0,0,0);

    QDialogButtonBox *bb = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Discard);
    QTableWidget *tbl = new QTableWidget();
    tbl->setRowCount(jag->dumpvars().count());
    tbl->setColumnCount(1);
    tbl->setColumnWidth(0,264);
    tbl->setHorizontalHeaderLabels(QStringList()<<"Tag Data");
    tbl->setVerticalHeaderLabels(jag->dumpvars());
    for(int i=0;i<jag->dumpvars().count();i++){
        //qDebug() << tbl->verticalHeaderItem(i)->text();
    }

    tbl->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tbl->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    fl->addWidget(tbl);

    ml->addWidget(frame);
    ml->addWidget(bb);
    this->setLayout(ml);
}

tageditor::~tageditor(){}
