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

mwindow::mwindow(vars *jag) : QMainWindow(){
    /* mandatory allocations */
    this->jag = jag;
    this->bar = new toolbar(this, this->configmenu);
    this->prog = new progslider(Qt::Horizontal, this->bar, this);
    this->vol = new volslider(Qt::Horizontal, this->bar, this);
    this->fr = new TagLib::FileRef();
    this->sdiag = new settings(this, jag);
    this->adiag = new about(this, jag);
    this->tagdiag = new tageditor(this, jag);
    this->transcdiag = new transcoder(jag, this);
    this->setStatusBar(new QStatusBar());
    this->wb = new workbench(jag, this);

    /* initialize the QMediaPlayer subclass and then move it to a separate thread */
    this->playert = new QThread(this);
    this->jag->mp->init(this);
    this->jag->mp->moveToThread(this->playert);
    this->playert->start();

    /* setting stuff up */
    this->bar->addWidget(this->vol);
    this->bar->addSeparator();
    this->bar->addWidget(this->prog);
    this->addToolBar(this->bar);
    this->setCentralWidget(this->wb);
    this->setWindowIcon(QIcon(":/internals/caticon"));
    this->setMinimumSize(360, 220);
    this->layout()->setContentsMargins(0,0,0,0);
    this->setWindowTitle(QString("OCELOT v")+jag->VERSION);
    this->sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
    this->sizePolicy().setVerticalPolicy(QSizePolicy::Expanding);
    this->statusBar()->addWidget(&this->status);
    this->statusBar()->addPermanentWidget(&this->proglabel);
    this->status.setText("<b>IDLE</b>");
    this->proglabel.setText("<b>"+QDateTime::fromTime_t(0).toString("mm:ss")+" / "+QDateTime::fromTime_t(0).toString("mm:ss")+"</b>");

    /*connect(jag->mp, &player::mediaStatusChanged, this, &mwindow::player_respond);
    this->media_status(player->mediaStatus());*/

    /* connect the required signals */
    connect(this, &mwindow::uilock_flip, this, &mwindow::uilock_respond);
    connect(this->jag->mp, &player::positionChanged, this, &mwindow::progslider_sync);
    connect(this->prog, &QSlider::sliderMoved, this, &mwindow::progslider_moved);
    connect(this->prog, &QSlider::valueChanged, this, &mwindow::progslider_changed);
    connect(this->vol, &QSlider::valueChanged, this->jag->mp, &player::setVolume);
    connect(this->vol, &QSlider::sliderMoved, this, &mwindow::volslider_moved);
    connect(this->jag->mp, &player::mediaStatusChanged, this, &mwindow::player_respond);
    //connect(this->jag->mp, &player::mediaStatusChanged, this, &mwindow::media_status);
    //connect(this->prog, &QSlider::actionTriggered, this, &mwindow::progslider_clicked);

    /* restore the previous saved UI state */
    this->loadstate();

    /* then we're golden. hopefully. */
    qInfo() << "[INFO] Ocelot initialized successfully!\n::";
}

mwindow::~mwindow(){

}


void mwindow::resizeEvent(QResizeEvent* event){
    this->savestate();
    event->accept();
}

/*void mwindow::restore_state(){
    QSqlQuery q = this->jag->DB_REF->exec("SELECT val FROM data WHERE var = 'ui_state'");
    if(!q.next())
        return;

    qInfo() << this->restoreState(q.value(0).toByteArray());
}

void mwindow::restorewinsize(){
    QSqlQuery q = this->jag->DB_REF->exec("SELECT val FROM data WHERE var = 'ui_windowsize'");
    if(!q.next())
        return;

    int sw = QGuiApplication::primaryScreen()->geometry().width();
    int sh = QGuiApplication::primaryScreen()->geometry().height();
    int nw = q.value(0).toString().split(",").first().toInt();
    int nh = q.value(0).toString().split(",").last().toInt();
    if(nw>=sw||nh>=sh){
        this->setWindowState(Qt::WindowMaximized);
        return;
    }

    this->resize(nw, nh);
}*/

/*void mwindow::dumpwinsize(){
    qInfo() << "mwindow::dumpwinsize";
    QString s = QString::number(this->window()->width())+","+QString::number(this->window()->height());
    QSqlQuery *q = new QSqlQuery(*this->jag->DB_REF);
    q->prepare("UPDATE data SET val = :size WHERE var LIKE 'ui_windowsize'");
    q->bindValue(":size", s);
    q->exec();
    q->~QSqlQuery();

    this->resizetimer->stop();
}*/

void mwindow::closeEvent(QCloseEvent *event){
    this->savestate();
    event->accept();
}

void mwindow::savestate(){
    qInfo() << this->children();
    this->jag->setdbdata("ui_geometry", this->saveGeometry());
    this->jag->setdbdata("ui_state", this->saveState());
    this->jag->setdbdata("ui_maximized", QString::number(this->isMaximized()));

    if(!this->isMaximized()){
        //this->setdbdata(QString("ui_windowsize"), (QString::number(this->size().width())+","+QString::number(this->size().height())));
        this->jag->setdbdata("ui_windowpos", (QString::number(this->pos().x())+","+QString::number(this->pos().y())));
    }
}

void mwindow::loadstate(){
    if(this->jag->fetchdbdata("ui_maximized")=="1")
        this->showMaximized();

    QStringList tmppos = this->jag->fetchdbdata("ui_windowpos").toString().split(",");
    this->move(QPoint(tmppos.first().toInt(), tmppos.last().toInt()));

    this->restoreGeometry(this->jag->fetchdbdata("ui_geometry").toByteArray());
    this->restoreState(this->jag->fetchdbdata("ui_state").toByteArray());

    /*QStringList tmpsize = this->jag->fetchdbdata("ui_windowsize").toString().split(",");
    this->resize(QSize(tmpsize.first().toInt(), tmpsize.last().toInt()));*/
    //qInfo() << this->fetchdbdata("ui_maximized");

}


void mwindow::volslider_moved(int x){
    QPoint *p = new QPoint(this->vol->mapToGlobal(this->vol->pos()));
    p->setX(QCursor::pos().rx());
    QToolTip::showText((*p), QString::number(x), this->vol);
    p->~QPoint();
}

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

void mwindow::show(){
    QMainWindow::show();
}



/********
 * SLOTS
*********/

void mwindow::tageditor_spawn(QStringList *l){
    //qDebug() << l;
    //this->tagdiag->init(l);
    this->tagdiag->show();
    l->~QStringList();
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
    /////////////////////////////////////////////////////////////////////////////////////qDebug() << x;
}

void mwindow::uilock_respond(){
    if(this->wb->islocked()){
        this->status.setText("<b>LAYOUT EDITOR ENABLED!</b> :: You can disable it on the cogwheel button.");
    }else{
        this->status.setText(QString("<b>Ocelot v")+this->jag->VERSION+"</b");
    }
}

void mwindow::child_resized(){
    this->savestate();
}

void mwindow::toolbar_pause(){
    if(this->playerstatus != QMediaPlayer::BufferedMedia)
        return;
    if(!paused){
        this->setWindowTitle(QString("[PAUSED] :: ")+this->windowTitle());
        paused = !paused;
    }

    this->jag->mp->pause();
}

void mwindow::toolbar_play(){
    if(paused){
        paused=!paused;
        this->setWindowTitle(this->windowTitle().remove("[PAUSED] :: ").toUtf8());
    }
    this->jag->mp->playloaded();
}

void mwindow::toolbar_stop(){
    this->jag->mp->stop();
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

void mwindow::transcoder_spawn(QStringList *l, bool discard){
    if(discard)
        this->transcdiag->clear();
    this->transcdiag->append(l);
    this->transcdiag->show();
}

void mwindow::player_respond(QMediaPlayer::MediaStatus status){
    this->playerstatus = status;

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
