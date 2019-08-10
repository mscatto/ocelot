#ifndef TRANSCPRESET_H
#define TRANSCPRESET_H

#include <QWidget>
#include <QDialog>
#include "../../vars.hpp"

class transchelper : public QDialog
{
    Q_OBJECT
public:
    transchelper(QWidget *parent, vars *jag);
    ~transchelper();
private:
    QMap<QString,QWidget*> *helpers;
    QWidget *dummy;
public slots:
private slots:
    void selector_changed(QString sel);

};

#endif // TRANSCPRESET_H
