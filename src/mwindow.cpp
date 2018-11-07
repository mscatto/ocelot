/*
 * Ocelot Music Manager: music player and library manager built using Qt
 * Copyright (C) 2018 Matheus Scattolin Anselmo
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "mwindow.hpp"
#include "library.hpp"
#include "vars.hpp"

#include "cwidgets/volslider.hpp"
#include "cwidgets/progslider.hpp"
#include "cwidgets/toolbar.hpp"

#include "dialogs/settings.hpp"
#include "dialogs/about.hpp"
#include "dialogs/tageditor.hpp"

#include <QToolBar>
#include <QStatusBar>
#include <QGridLayout>
#include <QTreeWidget>
#include <QSplitter>
#include <QDebug>
#include <QSizePolicy>
#include <QSqlRecord>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QDateTime>
#include <QToolTip>
#include <QMenu>
#include <QStandardPaths>
#include <taglib/fileref.h>
#include <taglib/flacfile.h>

mwindow::mwindow(vars *jag) : QMainWindow(nullptr){
    this->jag = jag;
    this->bar = new toolbar(this, this->configmenu);
    this->player = new QMediaPlayer();
    this->prog = new progslider(Qt::Horizontal, this->bar, this);
    this->vol = new volslider(Qt::Horizontal, this->bar, this);

    this->sdiag = new settings(this, jag);
    this->adiag = new about(this, jag);
    this->tagdiag = new tageditor(this, jag);

    player->setVolume(100);
    player->setNotifyInterval(200);
    this->media_status(player->mediaStatus());
    this->setWindowIcon(QIcon(":/internals/icon"));
    this->setMinimumSize(896, 504);
    this->layout()->setContentsMargins(0,0,0,0);
    this->setWindowTitle(QString("OCELOT v")+jag->VERSION);
    this->sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
    this->sizePolicy().setVerticalPolicy(QSizePolicy::Expanding);
    this->setStatusBar(new QStatusBar());
    this->statusBar()->addWidget(&this->status);
    this->statusBar()->addPermanentWidget(&this->proglabel);


    connect(this->player, &QMediaPlayer::positionChanged, this, &mwindow::progslider_sync);
    connect(this->prog, &QSlider::sliderMoved, this, &mwindow::progslider_moved);
    connect(this->prog, &QSlider::sliderReleased, this, &mwindow::progslider_released);
    connect(this->prog, &QSlider::actionTriggered, this, &mwindow::progslider_clicked);
    connect(this->vol, &QSlider::valueChanged, this->player, &QMediaPlayer::setVolume);
    connect(this->vol, &QSlider::sliderMoved, this, &mwindow::volslider_moved);
    connect(this->player, &QMediaPlayer::mediaStatusChanged, this, &mwindow::media_status);
    connect(this->player, &QMediaPlayer::mediaStatusChanged, this, &mwindow::mediastatus);


    this->bar->addWidget(this->vol);
    this->bar->addSeparator();
    this->bar->addWidget(this->prog);
    ///this->bar->set

    status.setText("<b>IDLE</b>");
    proglabel.setText("<b>"+QDateTime::fromTime_t(0).toString("mm:ss")+" / "+QDateTime::fromTime_t(0).toString("mm:ss")+"</b>");

    /*QStringList *data = new QStringList();
    QSqlQuery d = this->lib->query("SELECT path FROM songs");
    while(d.next())
        data->append(d.record().value(0).toString());
    this->lib->scan("/storage/music", data);
    data->~QStringList();*/

    this->wb = new workbench(jag, this);
    this->setCentralWidget(wb);
    this->addToolBar(this->bar);

    qInfo() << "[INFO] Ocelot initialized successfully!\n\n";
}

mwindow::~mwindow(){

}

void mwindow::toolbar_pause(){
    this->player->pause();

    if(this->player->mediaStatus() == QMediaPlayer::BufferedMedia){
        this->status.setText(this->status.text().replace("NOW PLAYING", "PAUSED"));
        this->setWindowTitle("PAUSED :: "+this->windowTitle());
    }
}

void mwindow::toolbar_play(){
    this->player->play();

    if(this->player->mediaStatus()==QMediaPlayer::LoadedMedia)
        this->media_status(this->player->mediaStatus());
    else{
        this->status.setText(this->status.text().replace("PAUSED", "NOW PLAYING"));
        this->setWindowTitle(this->windowTitle().replace("PAUSED :: ", ""));
    }
}

void mwindow::toolbar_stop(){
    this->player->stop();
    this->player->setMedia(QMediaContent());
    this->prog->setValue(0);
    this->prog->setRange(0,0);
    this->status.setText("<b>IDLE</b>");
    this->setWindowTitle(QString("OCELOT v")+jag->VERSION);
    this->proglabel.setText("<b>"+QDateTime::fromTime_t(0).toString("mm:ss")+" / "+QDateTime::fromTime_t(0).toString("mm:ss")+"</b>");
}

void mwindow::toolbar_next(){
    this->plnext();
}

void mwindow::toolbar_prev(){
    this->plprev();
}

void mwindow::toolbar_menu(){
    //this->configmenu->show();
}

void mwindow::config_spawn(){
    this->sdiag->show();
}

void mwindow::about_spawn(){
    this->adiag->show();
}

void mwindow::transcoder_spawn(){
    this->transcdiag->show();
}

void mwindow::tageditor_spawn(QStringList *l){
    //qDebug() << l;
    //this->tagdiag->init(l);
    this->tagdiag->show();
}

/* will be called every time the player changes position, which will be x */
void mwindow::progslider_sync(qint64 x){
    if (!this->prog->isSliderDown()){
        this->prog->setValue((int)x/1000);
        this->proglabel.setText("<b>"+QDateTime::fromTime_t(unsigned(x)/1000).toString("mm:ss")+" / "+QDateTime::fromTime_t(player->duration()/1000).toString("mm:ss")+"</b>");
    }
}

void mwindow::progslider_moved(int x){
    QPoint *p = new QPoint(this->vol->mapToGlobal(this->vol->pos()));
    p->setX(QCursor::pos().rx());
    QToolTip::showText(*p, QDateTime::fromTime_t(unsigned(x)).toString("mm : ss"), this->prog);
    p->~QPoint();
}

void mwindow::progslider_clicked(int action){
    //QPoint *p = new QPoint(this->prog->mapToGlobal(this->prog->pos()));
    //this->player->setPosition(this->prog->value()+1);
    //this->progslider_moved(this->prog->value());
    //this
}

void mwindow::volslider_moved(int x){
    QPoint *p = new QPoint(this->vol->mapToGlobal(this->vol->pos()));
    p->setX(QCursor::pos().rx());
    QToolTip::showText((*p), QString::number(x), this->vol);
    p->~QPoint();
}

void mwindow::progslider_released(){
    this->player->setPosition(this->prog->value()*1000);
}

void mwindow::media_status(QMediaPlayer::MediaStatus status){
    //qDebug() << status;
    switch(status){
    case QMediaPlayer::MediaStatus::LoadedMedia:
        /* to start playing right away */
    case QMediaPlayer::MediaStatus::EndOfMedia:
        this->plnext();
        break;
    case QMediaPlayer::MediaStatus::NoMedia:
        this->coverchanged(new QPixmap());
        this->setWindowTitle(QString("OCELOT v")+jag->VERSION);
        this->status.setText("<b>IDLE</b>");
        this->clearcover();
        this->prog->setValue(0);
        this->prog->setEnabled(false);

        break;
    case QMediaPlayer::MediaStatus::InvalidMedia:
        this->setWindowTitle(QString("OCELOT v")+jag->VERSION);
        this->status.setText("<b>INVALID MEDIA!</b>");
        break;
    case QMediaPlayer::MediaStatus::UnknownMediaStatus:
        this->setWindowTitle(QString("OCELOT v")+jag->VERSION);
        this->status.setText("<b>NO MEDIA</b>");
        break;
    case QMediaPlayer::MediaStatus::StalledMedia:
    case QMediaPlayer::MediaStatus::BufferingMedia:
        break;
    case QMediaPlayer::MediaStatus::LoadingMedia: /* when new song is loaded */
        break; /* BufferedMedia is triggered when it finishes, so it is of no use */
    case QMediaPlayer::MediaStatus::BufferedMedia: /* this usually means it started playing */
        TagLib::FileRef *ref = new TagLib::FileRef(qPrintable(player->media().canonicalResource().url().path()));
        this->prog->setRange(0, ref->audioProperties()->lengthInSeconds());
        this->prog->setEnabled(true);

        QMimeDatabase *mdb = new QMimeDatabase();
        QString *fmt = new QString(mdb->mimeTypeForFile(player->media().canonicalResource().url().path()).name().remove(0,6).toUpper());
        this->setWindowTitle(QString(ref->tag()->artist().toCString(true))+" - "+QString(ref->tag()->title().toCString(true))+" :: OCELOT v"+jag->VERSION);
        this->status.setText("<b>NOW PLAYING:</b> "+QString(ref->tag()->title().toCString(true))+" :: "+*fmt+" "+QString::number(ref->audioProperties()->bitrate())+"kb/s @"+QString::number(ref->audioProperties()->sampleRate())+"Hz");

        if(mdb->mimeTypeForFile(player->media().canonicalResource().url().path()).name().remove(0,6) == "flac"){ /* chops the 'audio/' from mime name */
            TagLib::FLAC::File *x = new TagLib::FLAC::File(qPrintable(player->media().canonicalResource().url().path()));
            if(x->pictureList().front() != nullptr){
                QPixmap *np = new QPixmap();
                np->loadFromData((const uchar*)x->pictureList().front()->data().data(), x->pictureList().front()->data().size());
                np->scaled(200, 200, Qt::KeepAspectRatio);
                this->coverchanged(np);
                np->~QPixmap();
            }else{
                //this->gcover->
                //ui->coverview->setText("<b>Cover not set!</b>");
            }

            x->~File();
        }else if(mdb->mimeTypeForFile(player->media().canonicalResource().url().path()).name().remove(0,6) == "mpeg"){

        }else{
            //return;
        }
        ref->~FileRef();
        mdb->~QMimeDatabase();
        fmt->~QString();
        break;
    }
}

void mwindow::play(QTreeWidgetItem *item){
    if(!item->data(0, Qt::UserRole).isValid()) /* case path is empty */
        return;

    this->player->setMedia(QUrl::fromLocalFile(item->data(0, Qt::UserRole).toStringList().first()));
    this->plappend(item->data(0, Qt::UserRole).toStringList());
    this->player->play();
}

void mwindow::select(QTreeWidgetItem *item){
    //qDebug() << "lel";
    if(!item->data(0, Qt::UserRole).isValid()) /* case path is empty */
        return;

    QString *front = new QString(item->data(0, Qt::UserRole).toStringList().first());
    this->selectionchanged(*front);
    front->~QString();
}
