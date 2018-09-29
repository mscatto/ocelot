#include "transcoder.hpp"

#include <QDialog>
#include <QGridLayout>
#include <QSplitter>

#include "src/mwindow.hpp"


transcoder::transcoder(vars *jag, QWidget *win) : QDialog(win){
    QGridLayout *l = new QGridLayout(this);
    QSplitter *s = new QSplitter(this);
    l->addWidget(s);

    this->setLayout(l);
    this->setWindowTitle(QString("Transcoder :: OCELOT v")+jag->VERSION);
    this->setFixedSize(900,560);
    this->setModal(true);

    connect(qobject_cast<mwindow*>(win), &mwindow::convhandler, this, &transcoder::handle);
}



transcoder::~transcoder(){

}

void transcoder::handle(QStringList files){
    qDebug() << files;
}


