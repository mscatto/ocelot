#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <QObject>
#include <QMediaPlayer>
#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QThread>

class player : public QObject
{
    Q_OBJECT
private:
    QMediaPlayer *mp;
public:
    explicit player(QObject *parent = nullptr);
    void init(QMainWindow *win);
    ~player();
public slots:
    void playfile(QString *file);
    void playloaded();
    void stop();
    void pause();
    void load(QString *file);
    void setVolume(int volume);
    void setPosition(int x);
signals:
    void EOM();
    void paused(bool res);
    //void mediastatus(QMediaPlayer::MediaStatus status);
    void mediaStatusChanged(QMediaPlayer::MediaStatus status);
    void positionChanged(qint64 pos);
};
#endif // PLAYER_HPP
