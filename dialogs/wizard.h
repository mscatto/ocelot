#ifndef WIZARD_H
#define WIZARD_H

#include <QDialog>
#include <src/vars.hpp>

class wizard : public QDialog
{
private:
    vars *jag;
    void closeEvent(QCloseEvent *event);
public:
    wizard(vars *jag);
    ~wizard();
private slots:
    void on_close();
};

#endif // WIZARD_H
