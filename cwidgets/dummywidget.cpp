#include "dummywidget.h"
#include <QGridLayout>
#include <QLabel>

dummywidget::dummywidget(const QString text) : QLabel(){
    this->setFrameShadow(QFrame::Raised);
    this->setFrameShape(QFrame::Shape::StyledPanel);
    this->setText(text);
    this->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

dummywidget::~dummywidget(){

}
