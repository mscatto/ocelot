#ifndef CONFIGS_HPP
#define CONFIGS_HPP

#include <QDialog>
#include "library.hpp"

namespace Ui {
    class configs;
    }

class configs : public QDialog
{
    Q_OBJECT

public:
    explicit configs(QWidget *parent = 0);
    void refresh(QStringList *vars);
    ~configs();

private slots:
    void on_tabWidget_tabBarClicked(int index);
    void on_listView_doubleClicked(const QModelIndex &index);

private:
    Ui::configs *ui;
};

#endif // CONFIGS_HPP
