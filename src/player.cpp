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

#include "player.hpp"
#include "mwindow.hpp"
#include <QMediaPlayer>

player::player(QObject *parent) : QObject(parent){

}

void player::init(QMainWindow *win){
    this->mp = new QMediaPlayer();
    this->mp->setVolume(100);
    this->mp->setNotifyInterval(200);

    //connect(this, &player::paused, this, &player::positionChanged);
    connect(qobject_cast<mwindow*>(win), &mwindow::progsliderchanged, this, &player::setPosition);
    connect(this->mp, &QMediaPlayer::mediaStatusChanged, this, &player::mediaStatusChanged);
    connect(this->mp, &QMediaPlayer::positionChanged, this, &player::positionChanged);
    connect(this, &player::paused, qobject_cast<mwindow*>(win), &mwindow::toolbar_pause);
    connect(qobject_cast<mwindow*>(win), &mwindow::player_play, this, &player::playfile);
}

player::~player(){}

void player::playfile(QString *file){
    qDebug() << *file;
    if(file->isEmpty())
        return;

    this->mp->setMedia(QUrl::fromLocalFile(*file));
    this->mp->play();
}

void player::playloaded(){
    qDebug() << "play without file";
}

void player::stop(){

}

void player::pause(){
    this->mp->pause();

    if(this->mp->mediaStatus() == QMediaPlayer::BufferedMedia){
        this->paused(true); /* making sure this condition is satisfied */
    }/* ie. its paused but theres media loaded */
}

void player::load(QString *file)
{

}

void player::setVolume(int volume){
    this->mp->setVolume(volume);
}

void player::setPosition(int x){
    qDebug() << x;
    this->mp->setPosition(x*1000);
}
