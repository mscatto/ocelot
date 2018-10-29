#ifndef TAGEDITOR_HPP
#define TAGEDITOR_HPP

#include <QDialog>
#include <QTableView>

#include "src/vars.hpp"

class tageditor : public QDialog
{
private:
    QStringList *queue;
public:
    void init(QStringList *l);
    tageditor(QWidget *parent, vars *jag);
    ~tageditor();
};

#endif // TAGEDITOR_HPP
