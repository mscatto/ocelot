#ifndef TOOLBAR_HPP
#define TOOLBAR_HPP

#include<QToolBar>

class toolbar : public QToolBar{
private:
    QMenu *conf;
    QList<QWidget*> docked;
public:
    toolbar(QWidget *win);
    ~toolbar();
    void pop();
    void addWidget(QWidget *widget);
private slots:
    void orient(Qt::Orientation o);
};

#endif // TOOLBAR_HPP
