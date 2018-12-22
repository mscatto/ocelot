#ifndef DUMMYWIDGET_H
#define DUMMYWIDGET_H

#include<QLabel>

class dummywidget : public QLabel
{
    Q_OBJECT
public:
    dummywidget(const QString text);
    ~dummywidget();
};

#endif // DUMMYWIDGET_H
