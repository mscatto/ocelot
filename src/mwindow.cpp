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
    this->prog = new progslider(Qt::Horizontal, this->bar, this);
    this->vol = new volslider(Qt::Horizontal, this->bar, this);
    this->fr = new TagLib::FileRef();
    this->jag->mp->init(this);
    QThread *pt = new QThread(this);
    this->jag->mp->moveToThread(pt);
    pt->start();

    this->sdiag = new settings(this, jag);
    this->adiag = new about(this, jag);
    this->tagdiag = new tageditor(this, jag);

    //connect(jag->mp, &player::mediaStatusChanged, this, &mwindow::player_respond);
    //this->media_status(player->mediaStatus());
    this->setWindowIcon(QIcon(":/internals/icon"));
    this->setMinimumSize(896, 504);
    this->layout()->setContentsMargins(0,0,0,0);
    this->setWindowTitle(QString("OCELOT v")+jag->VERSION);
    this->sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
    this->sizePolicy().setVerticalPolicy(QSizePolicy::Expanding);
    this->setStatusBar(new QStatusBar());
    this->statusBar()->addWidget(&this->status);
    this->statusBar()->addPermanentWidget(&this->proglabel);


    connect(this->jag->mp, &player::positionChanged, this, &mwindow::progslider_sync);
    connect(this->prog, &QSlider::sliderMoved, this, &mwindow::progslider_moved);
    connect(this->prog, &QSlider::valueChanged, this, &mwindow::progslider_changed);
    //connect(this->prog, &QSlider::actionTriggered, this, &mwindow::progslider_clicked);
    connect(this->vol, &QSlider::valueChanged, this->jag->mp, &player::setVolume);
    connect(this->vol, &QSlider::sliderMoved, this, &mwindow::volslider_moved);
    //connect(this->jag->mp, &player::mediaStatusChanged, this, &mwindow::media_status);
    connect(this->jag->mp, &player::mediaStatusChanged, this, &mwindow::player_respond);


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

void mwindow::toolbar_pause(bool res){
    if(res){/* isn't this ugly? */
        this->status.setText(this->status.text().replace("NOW PLAYING", "PAUSED"));
        this->setWindowTitle("PAUSED :: "+this->windowTitle());
        return;
    }

    //this->jag->mp->pause(); /* if the player actually changes to a pause pause state */
    /* this function will be called again, but packing a true */
}

void mwindow::toolbar_play(){
    //this->jag->mp->play();
}

void mwindow::toolbar_stop(){
    //this->jag->mp->stop();
    //this->player->setMedia(QMediaContent());
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
void mwindow::progslider_sync(qint64 pos){
    if (!this->prog->isSliderDown()){
        this->prog->setValue((int)pos/1000);
        this->proglabel.setText("<b>"+QDateTime::fromTime_t(unsigned(pos)/1000).toString("mm:ss")+" / "+QDateTime::fromTime_t(fr->audioProperties()->lengthInSeconds()).toString("mm:ss")+"</b>");
    }
}

void mwindow::progslider_moved(int x){
    QPoint *p = new QPoint(this->vol->mapToGlobal(this->vol->pos()));
    p->setX(QCursor::pos().rx());
    QToolTip::showText(*p, QDateTime::fromTime_t(unsigned(x)).toString("mm : ss"), this->prog);
    p->~QPoint();
    this->progsliderchanged(x);
}

void mwindow::progslider_clicked(int action){
    //QPoint *p = new QPoint(this->prog->mapToGlobal(this->prog->pos()));
    qDebug() << action;
    this->progsliderchanged(this->prog->value()+1);
    //this->player->setPosition(this->prog->value()+1);
    //this->progslider_moved(this->prog->value());
    //this
}

void mwindow::progslider_changed(int x){
    qDebug() << x;
}

void mwindow::volslider_moved(int x){
    QPoint *p = new QPoint(this->vol->mapToGlobal(this->vol->pos()));
    p->setX(QCursor::pos().rx());
    QToolTip::showText((*p), QString::number(x), this->vol);
    p->~QPoint();
}

void mwindow::player_respond(QMediaPlayer::MediaStatus status){
    switch(status){
    case QMediaPlayer::MediaStatus::NoMedia: /* case stopped */
        this->coverchanged(new QPixmap());
        this->setWindowTitle(QString("OCELOT v")+jag->VERSION);
        this->status.setText("<b>IDLE</b>");
        this->clearcover();
        this->prog->setValue(0);
        this->prog->setEnabled(false);
        break;
    case QMediaPlayer::MediaStatus::LoadingMedia: /* when a new song is being loaded */
        break; /* BufferedMedia is sent when it finishes */
    case QMediaPlayer::MediaStatus::LoadedMedia:
        this->status.setText(this->status.text().replace("PAUSED", "NOW PLAYING"));
        this->setWindowTitle(this->windowTitle().replace("PAUSED :: ", ""));
        break;
    case QMediaPlayer::MediaStatus::EndOfMedia:
        this->plnext();
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
    case QMediaPlayer::MediaStatus::BufferedMedia: /* this usually means it started playing */
        //TagLib::FileRef *ref = new TagLib::FileRef();
        QMimeDatabase *db = new QMimeDatabase;
        QString *fmt = new QString(db->mimeTypeForFile(fr->file()->name()).name().remove(0,6).toUpper());

        this->prog->setRange(0, this->fr->audioProperties()->lengthInSeconds());
        this->prog->setEnabled(true);

        this->setWindowTitle(QString(this->fr->tag()->artist().toCString(true))+" - "+QString(this->fr->tag()->title().toCString(true))+" :: OCELOT v"+jag->VERSION);
        this->status.setText("<b>NOW PLAYING:</b> "+QString(this->fr->tag()->title().toCString(true))+" :: "+*fmt+" "+QString::number(this->fr->audioProperties()->bitrate())+"kb/s @"+QString::number(this->fr->audioProperties()->sampleRate())+"Hz");

        if(db->mimeTypeForFile(this->fr->file()->name()).name().remove(0,6) == "flac"){ /* chops the 'audio/' from mime name */
            TagLib::FLAC::File *x = new TagLib::FLAC::File(qPrintable(this->fr->file()->name()));
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
        }else if(db->mimeTypeForFile(this->fr->file()->name()).name().remove(0,6) == "mpeg"){

        }else{
            //return;
        }
        db->~QMimeDatabase();
        fmt->~QString();
        break;
    }

}


/*void mwindow::media_status(QMediaPlayer::MediaStatus status){
    //qDebug() << status;
    switch(status){
    case QMediaPlayer::MediaStatus::LoadedMedia:
        // to start playing right away
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
    case QMediaPlayer::MediaStatus::LoadingMedia: // when new song is loaded
        break; // BufferedMedia is triggered when it finishes, so it is of no use
    case QMediaPlayer::MediaStatus::BufferedMedia: // this usually means it started playing
        TagLib::FileRef *ref = new TagLib::FileRef(qPrintable(player->media().canonicalResource().url().path()));
        this->prog->setRange(0, ref->audioProperties()->lengthInSeconds());
        this->prog->setEnabled(true);

        QMimeDatabase *mdb = new QMimeDatabase();
        QString *fmt = new QString(mdb->mimeTypeForFile(player->media().canonicalResource().url().path()).name().remove(0,6).toUpper());
        this->setWindowTitle(QString(ref->tag()->artist().toCString(true))+" - "+QString(ref->tag()->title().toCString(true))+" :: OCELOT v"+jag->VERSION);
        this->status.setText("<b>NOW PLAYING:</b> "+QString(ref->tag()->title().toCString(true))+" :: "+*fmt+" "+QString::number(ref->audioProperties()->bitrate())+"kb/s @"+QString::number(ref->audioProperties()->sampleRate())+"Hz");

        if(mdb->mimeTypeForFile(player->media().canonicalResource().url().path()).name().remove(0,6) == "flac"){ //chops the 'audio/' from mime name
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
}*/

void mwindow::play(QTreeWidgetItem *item){
    this->plappend(item->data(0, Qt::UserRole).toStringList());
    this->player_play(new QString(item->data(0, Qt::UserRole).toStringList().first()));

    this->fr->~FileRef();
    this->fr = new TagLib::FileRef(qPrintable(item->data(0, Qt::UserRole).toStringList().first()));
}

void mwindow::select(QTreeWidgetItem *item){
    //qDebug() << "lel";
    if(!item->data(0, Qt::UserRole).isValid()) /* case path is empty */
        return;

    QString *front = new QString(item->data(0, Qt::UserRole).toStringList().first());
    this->selectionchanged(*front);
    front->~QString();
}
