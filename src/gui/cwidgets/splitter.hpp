#ifndef SPLITTER_HPP
#define SPLITTER_HPP

#include "src/gui/mwindow.hpp"
#include "src/gui/workbench.hpp"
#include <QSplitter>
#include <QWidget>

class splitter : public QSplitter {
    Q_OBJECT
private:
    mwindow* winptr;
	workbench* wb;
	void showctx(QPoint pos);
public:
	splitter(Qt::Orientation o, mwindow* win, workbench* wb);
    ~splitter() override {
    }

protected:
    QSplitterHandle* createHandle() override;
};

class handle : public QSplitterHandle {
    Q_OBJECT
public:
	handle(Qt::Orientation o, QSplitter* parent, mwindow* win);
    ~handle() override {
    }
signals:
    void savestate();

protected:
    void mouseReleaseEvent(QMouseEvent* e) override;
};

#endif // SPLITTER_HPP
