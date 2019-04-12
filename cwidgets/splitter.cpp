#include "splitter.hpp"

splitter::splitter(Qt::Orientation o, QWidget *parent) : QSplitter(o, parent){
    (o==Qt::Horizontal) ? this->setObjectName("hsplitter") : this->setObjectName("vsplitter");
    this->setChildrenCollapsible(false);
}

splitter::~splitter(){

}
