#include "splitter.hpp"

splitter::splitter (Qt::Orientation o, mwindow* win, workbench* wb) : QSplitter (o) {
	(o == Qt::Horizontal) ? this->setObjectName ("hsplitter") : this->setObjectName ("vsplitter");
	this->setChildrenCollapsible (false);
	this->winptr = win;
	this->wb = wb;
}

QSplitterHandle* splitter::createHandle() {
	return new class handle (orientation(), this, this->winptr);
}

handle::handle (Qt::Orientation o, QSplitter* parent, mwindow* win) : QSplitterHandle (o, parent) {
	connect (this, &handle::savestate, static_cast<mwindow*> (win), &mwindow::savestate);
}

void handle::mouseReleaseEvent (QMouseEvent* e) {
	emit this->savestate();
	e->accept();
}

void splitter::showctx (QPoint pos) {
	if (!this->wb->islocked()) {
		this->wb->setlastctx (this);
		this->wb->ctx_req (this->mapTo (this->wb, pos));
	}
}
