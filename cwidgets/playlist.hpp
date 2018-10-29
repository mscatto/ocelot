#ifndef PLAYLIST_HPP
#define PLAYLIST_HPP

#include<QTreeWidget>
#include"src/mwindow.hpp"

class playlist : public QTreeWidget
{
    Q_OBJECT
private:
    QString *order;
    vars *jag;
    QMenu *headerctx;
    QMenu *bodyctx;
    ushort playing_index=0;
    QTreeWidgetItem *playing;
    QTreeWidgetItem *selected;
    QStringList pl;

    const char* playchar = "►";
    bool contains(QString path);
public:
    playlist(QString *order, QMenu *headerctx, vars *jag, mwindow *win, QWidget *parent);
    ~playlist();
private slots:
    void show_headerctx(const QPoint & pos);
    void show_bodyctx(const QPoint &pos);
    void clear();
public slots:
    void rebuild_columns();
    void append(QStringList f);
    void next();
    void prev();
    void doubleclick(QTreeWidgetItem*item);
signals:
    void play(QTreeWidgetItem*);
    void EOP();
};

#endif // PLAYLIST_HPP
