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
    QMenu *emptyctx;
    ushort playing_index=0;
    QTreeWidgetItem *playing;
    QTreeWidgetItem *selected;
    QStringList pl;
    QTabWidget *view;

    const char* playchar = "►";
    bool contains(QString path);
    void dump(QString *path);
public:
    playlist(QString *order, QMenu *headerctx, vars *jag, mwindow *win, QWidget *parent);
    ~playlist();
private slots:
    void show_headerctx(const QPoint & pos);
    void show_bodyctx(const QPoint &pos);
    void clearchildren();
public slots:
    void rebuild_columns();
    void append(QStringList f);
    void next();
    void prev();
    void doubleclick(QTreeWidgetItem*item);
    void exportpl();
signals:
    void changesel(TagLib::FileRef *f);
    void play(QTreeWidgetItem*);
    void EOP();
};

#endif // PLAYLIST_HPP
