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
