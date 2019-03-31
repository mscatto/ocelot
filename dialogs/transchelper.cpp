#include "transchelper.hpp"
#include "helpers/flac.hpp"
#include "helpers/intro.hpp"

#include <QDialogButtonBox>
#include <QComboBox>

transchelper::transchelper(QWidget *parent, vars *jag) : QDialog (parent){
    /* allocations */
    QDialogButtonBox *b = new QDialogButtonBox(QDialogButtonBox::Discard | QDialogButtonBox::Save);
    QVBoxLayout *l = new QVBoxLayout;
    QComboBox *selector = new QComboBox;

    this->setLayout(l);
    this->setFixedSize(420, 460);
    this->setWindowTitle(QString("New Preset :: OCELOT v")+jag->VERSION);
    this->setModal(false);

    this->helpers = new QMap<QString, QWidget*>;
    //this->helpers->
    this->helpers->insert("Default", new intro);
    this->helpers->insert("Custom", new QWidget);
    this->helpers->insert("FLAC", new flac);
    this->helpers->insert("AAC", new QWidget);
    this->helpers->insert("MP3", new QWidget);
    this->helpers->insert("OGG", new QWidget);
    this->helpers->insert("WAV", new QWidget);

    this->dummy = this->helpers->value("Default");

    selector->addItems(QStringList()<<"Default"<<"Custom"<<"FLAC"<<"AAC"<<"MP3"<<"OGG"<<"WAV");
    selector->setCurrentIndex(0);

    b->setEnabled(false);

    l->addWidget(selector);
    l->addWidget(dummy);
    l->addWidget(b);

    connect(selector, SIGNAL(activated(QString)), this, SLOT(selector_changed(QString)));
}

transchelper::~transchelper(){

}

void transchelper::selector_changed(QString sel){
    int i = qobject_cast<QVBoxLayout*>(this->layout())->indexOf(this->dummy);
    this->dummy->setParent(nullptr);
    qobject_cast<QVBoxLayout*>(this->layout())->removeWidget(this->dummy);
    this->dummy = this->helpers->value(sel);
    qobject_cast<QVBoxLayout*>(this->layout())->insertWidget(i, this->dummy);
}

