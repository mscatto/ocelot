#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <QObject>
#include <QWidget>
#include <QDialog>

#include "src/vars.hpp"
#include "src/libmgr.hpp"

class settings : public QDialog
{
private:
    vars *jag;
    QTreeWidget *libstree;
    QWidget* spawn_maintab(QWidget *win);
    QWidget* spawn_libtab(QWidget *win, QSqlDatabase *db);

private slots:
    void lib_dirpicker();
    void lib_treerefresh();
public:
    settings(QWidget *parent, vars *jag);
    ~settings();
};

#endif // SETTINGS_HPP
