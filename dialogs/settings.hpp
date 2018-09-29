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
    QWidget *win;
    QTreeWidget *libstree;
    QWidget* spawn_maintab(QWidget *win);
    QWidget* spawn_libtab(QWidget *win);

private slots:
    void lib_dirpicker();
    void lib_treerefresh();
    void lib_remdir();
public:
    settings(QWidget *parent, vars *jag);
    ~settings();
};

#endif // SETTINGS_HPP
