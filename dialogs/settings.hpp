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
    const uint APPEND=0;
    const uint REPLACE=1;
    const uint NOTHING=2;

    const uint PLAY=0;
    const uint IFIDLE=1;
    const uint DONTPLAY=2;

    vars *jag;
    QWidget *win;
    QTreeWidget *libstree;
    QWidget* spawn_maintab(QWidget *win);
    QWidget* spawn_libtab(QWidget *win);

private slots:
    void lib_dirpicker();
    void lib_treerefresh();
    void lib_remdir();

    void gen_mdclick(int index);
    void gen_mmclick(int index);
    void gen_mappend(int index);
public:
    settings(QWidget *parent, vars *jag);
    ~settings();
};

#endif // SETTINGS_HPP
