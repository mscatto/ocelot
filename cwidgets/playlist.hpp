#ifndef PLAYLIST_HPP
#define PLAYLIST_HPP

#include<QTreeWidget>
#include"src/mwindow.hpp"

class playlist : public QTreeWidget
{
private:
    QString *order;
    vars *jag;
    QMenu *headerctx;
    QMenu *bodyctx;
    ushort playing_index;
    QTreeWidgetItem *playing;
    QTreeWidgetItem *selected;
    QStringList pl;

    const char* playchar = "►";
    bool contains(QString path);
public:
    playlist(QString *order, QMenu *headerctx, vars *jag, mwindow *win);
    ~playlist();
private slots:
    void show_headerctx(const QPoint & pos);
    void show_bodyctx(const QPoint &pos);
    void clear();
public slots:
    void rebuild_columns();
    void append(QStringList f);
};

#endif // PLAYLIST_HPP
