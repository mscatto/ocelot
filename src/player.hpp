/*
 * The MIT License (MIT)
 * Copyright (c) 2018 Matheus Scattolin Anselmo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * The Software is provided “as is”, without warranty of any kind, express or implied,
 * including but not limited to the warranties of merchantability, fitness for a
 * particular purpose and noninfringement. In no event shall the authors or copyright
 * holders be liable for any claim, damages or other liability, whether in an action
 * of contract, tort or otherwise, arising from, out of or in connection with the
 * software or the use or other dealings in the Software.
 */

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
