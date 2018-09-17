#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QLabel>
#include <QSqlQueryModel>
#include <QGraphicsScene>
#include "configs.hpp"
#include "database.hpp"
#include "library.hpp"
#include "state.hpp"
#include <taglib.h>
#include <fileref.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(state* nst, QWidget *parent = nullptr);
    library *lib;
    void setlib(library *nlib);
    ~MainWindow();
    void showlabel(QString pos);
    void pop_tree(database* db);
private slots:
    void toolbar_stop();
    void toolbar_play();
    void toolbar_pause();
    void toolbar_next();
    void toolbar_prev();
    void toolbar_config();

    void on_treelib_itemClicked(QTreeWidgetItem *item, int column);
    void handlepos(qint64 x);
    void on_treelib_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_progslider_moved(int value);
    void on_progslider_released();
    void on_player_state(QMediaPlayer::MediaStatus);
    //void on_playlist_itemClicked(QTreeWidgetItem *item, int column);
    void on_volslider_released();
    void on_volslider_moved(int value);
    void on_playlist_customContextMenuRequested(const QPoint &pos);
    void on_progslider_action(int x);
private:
    void populate_tags(TagLib::FileRef *t);
    QGraphicsScene *scn;
    QList<QAction*> *actions;
    state *st;
    configs diag;
    QSlider *progslider;
    QSlider *volslider;
    QLabel poslabel;
    QLabel infolabel;
    QMediaPlayer *player;

    QStringList *extract(QString vars);
    Ui::MainWindow *ui;
    void recurtree(QTreeWidgetItem *parent, QStringList levels, QString conditions);
    void refsources(QStringList sources);
    void play(QString fpath);
    QLabel* status;
};

#endif // MAINWINDOW_H
