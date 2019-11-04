#include "splitter.hpp"
#include "src/gui/mwindow.hpp"
#include <QDebug>
splitter::splitter(Qt::Orientation o, mwindow* win) : QSplitter(o) {
    (o == Qt::Horizontal) ? this->setObjectName("hsplitter") : this->setObjectName("vsplitter");
    this->setChildrenCollapsible(false);
    this->winptr = win;
}

QSplitterHandle* splitter::createHandle() {
    return new class handle(orientation(), this, this->winptr);
}

handle::handle(Qt::Orientation o, QSplitter* parent, mwindow* win) : QSplitterHandle(o, parent) {
    connect(this, &handle::savestate, static_cast<mwindow*>(win), &mwindow::savestate);
}

void handle::mouseReleaseEvent(QMouseEvent* e) {
    emit this->savestate();
}
