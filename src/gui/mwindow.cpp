/*
 * The MIT License (MIT)
 * Copyright (c) 2018 Matheus Scattolin Anselmo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * The Software is provided “as is”, without warranty of any kind, express or
 * implied, including but not limited to the warranties of merchantability,
 * fitness for a particular purpose and noninfringement. In no event shall the
 * authors or copyright holders be liable for any claim, damages or other
 * liability, whether in an action of contract, tort or otherwise, arising from,
 * out of or in connection with the software or the use or other dealings in the
 * Software.
 */

#include "mwindow.hpp"
#include "../library.hpp"
#include "../vars.hpp"

#include "cwidgets/progslider.hpp"
#include "cwidgets/toolbar.hpp"
#include "cwidgets/volslider.hpp"

#include "src/libs/libnotify-qt/Notification.h"

#include "dialogs/about.hpp"
#include "dialogs/settings.hpp"
#include "dialogs/tageditor.hpp"

#include <QDateTime>
#include <QDebug>
#include <QGridLayout>
#include <QMainWindow>
#include <QMenu>
#include <QSizePolicy>
#include <QSplitter>
#include <QSqlRecord>
#include <QStandardPaths>
#include <QStatusBar>
#include <QToolBar>
#include <QToolTip>
#include <QTreeWidget>
#include <taglib/fileref.h>
#include <taglib/flacfile.h>

mwindow::mwindow(vars* jag) : QMainWindow() {
    /* mandatory allocations */
    this->jag = jag;

    this->prog = new progslider(Qt::Horizontal, jag->pctx);
    this->vol = new volslider(Qt::Horizontal, jag->pctx);
    this->bar = new toolbar(this, this->configmenu, this->prog, this->vol, jag);
    this->curtag = new TagLib::FileRef();
    this->sdiag = new settings(this, jag);
    this->adiag = new about(this, jag);
    this->tagdiag = new tageditor(this, jag);
    this->transcdiag = new transcoder(jag, this);
    this->setStatusBar(new QStatusBar());
    this->wb = new workbench(jag, this);

    this->addToolBar(this->bar);
    this->setCentralWidget(this->wb);
    this->statusBar()->addWidget(&this->status);
    this->statusBar()->addPermanentWidget(&this->proglabel);

    this->setWindowIcon(QIcon(":/internals/caticon"));
    this->setMinimumSize(360, 220);
    this->layout()->setContentsMargins(0, 0, 0, 0);
    this->setWindowTitle(QString("OCELOT v") + jag->VERSION);
    this->sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
    this->sizePolicy().setVerticalPolicy(QSizePolicy::Expanding);
    this->status.setText("<b>IDLE</b>");


    this->update_proglabel(0, 0);

    /* connect the required signals */
    connect(this, &mwindow::uilock_flip, this, &mwindow::uilock_respond);

    connect(this, &mwindow::player_play, this->jag->pctx, &player::play);
    connect(this, &mwindow::player_set, this, &mwindow::on_player_set);
    connect(this, &mwindow::player_set, this->jag->pctx, &player::set);
    connect(this, &mwindow::player_stop, this->jag->pctx, &player::stop);
    connect(this, &mwindow::player_pause, this->jag->pctx, &player::pause);
    connect(this, &mwindow::player_seek, this->jag->pctx, &player::seek);
    connect(this, &mwindow::player_setvol, this->jag->pctx, &player::setVolume);

    connect(this->jag->pctx, &player::position_changed, this, &mwindow::progslider_sync);
    connect(this->jag->pctx, &player::length_changed, this, &mwindow::progslider_set);
    connect(this->jag->pctx, &player::EOM, this, &mwindow::on_player_EOM);

    connect(this->prog, &progslider::sliderMoved, this, &mwindow::progslider_moved);

    connect(this->vol, &volslider::sliderMoved, this, &mwindow::volslider_moved);

    /* restore the previous saved UI state */
    this->loadstate();

    /* so it picks up the saved position */
    // this->bar->rotate(this->bar->orientation());

    /* then we're golden. hopefully. */
    qInfo() << "[INFO] Ocelot initialized successfully!\n::";
}

void mwindow::resizeEvent(QResizeEvent* event) {
    // this->savestate();
    event->accept();
}

void mwindow::update_proglabel(uint pos, uint len) {
    // qInfo() << pos << len;
    this->proglabel.setText("<b>" + QDateTime::fromTime_t(pos).toString("mm:ss") + " / "
                            + QDateTime::fromTime_t(len).toString("mm:ss") + "</b>");
}

/*void mwindow::restore_state(){
    QSqlQuery q = this->jag->DB_REF->exec("SELECT val FROM data WHERE var =
'ui_state'"); if(!q.next()) return;

    qInfo() << this->restoreState(q.value(0).toByteArray());
}

void mwindow::restorewinsize(){
    QSqlQuery q = this->jag->DB_REF->exec("SELECT val FROM data WHERE var =
'ui_windowsize'"); if(!q.next()) return;

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
    QString s =
QString::number(this->window()->width())+","+QString::number(this->window()->height());
    QSqlQuery *q = new QSqlQuery(*this->jag->DB_REF);
    q->prepare("UPDATE data SET val = :size WHERE var LIKE 'ui_windowsize'");
    q->bindValue(":size", s);
    q->exec();
    q->~QSqlQuery();

    this->resizetimer->stop();
}*/

void mwindow::closeEvent(QCloseEvent* event) {
    this->savestate();
    event->accept();
}

void mwindow::savestate() {
    qInfo() << "[FIX] mwindow::savestate"; // this->children();
    this->jag->setdbdata("ui_geometry", this->saveGeometry());
    this->jag->setdbdata("ui_state", this->saveState());
    this->jag->setdbdata("ui_maximized", QString::number(this->isMaximized()));
    if(!this->isMaximized()) {
        // this->setdbdata(QString("ui_windowsize"),
        // (QString::number(this->size().width())+","+QString::number(this->size().height())));
        this->jag->setdbdata("ui_windowpos", (QString::number(this->pos().x()) + "," + QString::number(this->pos().y())));
    }
}

void mwindow::loadstate() {
    if(this->jag->fetchdbdata("ui_maximized") == "1")
        this->showMaximized();

    QStringList tmppos = this->jag->fetchdbdata("ui_windowpos").toString().split(",");
    this->move(QPoint(tmppos.first().toInt(), tmppos.last().toInt()));

    this->restoreGeometry(this->jag->fetchdbdata("ui_geometry").toByteArray());
    this->restoreState(this->jag->fetchdbdata("ui_state").toByteArray());

    /*QStringList tmpsize =
    this->jag->fetchdbdata("ui_windowsize").toString().split(",");
    this->resize(QSize(tmpsize.first().toInt(), tmpsize.last().toInt()));*/
    // qInfo() << this->fetchdbdata("ui_maximized");
}

void mwindow::volslider_moved(int x) {
    QPoint p = this->vol->mapToGlobal(this->vol->pos());

    if(this->vol->orientation() == Qt::Horizontal)
        p.setX(QCursor::pos().rx());
    else
        p.setY(QCursor::pos().ry());
    QToolTip::showText(p, QString::number(x), this->vol);

    emit player_setvol(uint(x));
}

void mwindow::play(QTreeWidgetItem* item) {
    qInfo() << "cy";
    this->plappend(item->data(0, Qt::UserRole).toStringList());
    emit player_set(item->data(0, Qt::UserRole).toStringList().first());


    // this->fr->~FileRef();
    // this->fr = new TagLib::FileRef(qPrintable(item->data(0, Qt::UserRole).toStringList().first()));
}

void mwindow::select(QTreeWidgetItem* item) {
    // qDebug() << "lel";
    if(!item->data(0, Qt::UserRole).isValid()) /* case path is empty */
        return;

    QString* front = new QString(item->data(0, Qt::UserRole).toStringList().first());
    this->selectionchanged(*front);
    front->~QString();
}

void mwindow::show() {
    QMainWindow::show();
}

/********
 * SLOTS
 *********/

void mwindow::tageditor_spawn(QStringList* l) {
    // qDebug() << l;
    // this->tagdiag->init(l);
    this->tagdiag->show();
    l->~QStringList();
}

/* will be called every time the player changes position, which will be x */
void mwindow::progslider_sync(QTime pos) {
    this->prog->setValue(QTime(0, 0).secsTo(pos));
    this->update_proglabel(QTime(0, 0).secsTo(pos), this->prog->maximum());
}

void mwindow::progslider_moved(int x) {
    QPoint p = this->prog->mapToGlobal(this->prog->pos());
    if(this->prog->orientation() == Qt::Horizontal)
        p.setX(QCursor::pos().rx());
    else
        p.setY(QCursor::pos().ry());
    QToolTip::showText(p, QDateTime::fromTime_t(unsigned(x)).toString("mm : ss"), this->prog);

    emit this->player_seek(static_cast<short>(x));
}

void mwindow::uilock_respond() {
    if(this->wb->islocked()) {
        this->status.setText("<b>LAYOUT EDITOR ENABLED!</b> :: You can disable "
                             "it on the cogwheel button.");
    } else {
        this->status.setText(QString("<b>OCELOT v") + this->jag->VERSION + "</b");
    }
}

void mwindow::notify(bool playing, QString summary, QString body) {
    Notification notify(summary);
    playing ? notify.init("NOW PLAYING :: OCELOT") : notify.init("OCELOT");

    notify.setBody(body);
    notify.setIconName("caticon");
    notify.setCategory("playback");
    notify.setTimeout(3000);
    notify.setUrgency(NOTIFICATION_URGENCY_LOW);

    notify.show();
}

void mwindow::on_player_set(QString file) {
    this->playing = file;
    this->state = pstate::PLAYING;
    this->curtag->~FileRef();
    this->curtag = new TagLib::FileRef(file.toUtf8());
    this->prog->setEnabled(true);

    this->setWindowTitle(QString("OCELOT v") + this->jag->VERSION);
    QString t = this->curtag->tag()->artist().toCString(true) + QString(": ");
    this->setWindowTitle(t + this->curtag->tag()->title().toCString(true) + " :: " + this->windowTitle());

    this->status.setText("<b>PLAYING:</b> " + QString(this->curtag->tag()->title().toCString(true))
                         + " :: " + QMimeDatabase().mimeTypeForFile(file).name().remove(0, 6).toUpper().append(" ")
                         + QString::number(this->curtag->audioProperties()->bitrate()) + "kb/s @"
                         + QString::number(this->curtag->audioProperties()->sampleRate()) + "Hz");

    QString summary = "$t ($d)\n$a";
    summary.replace("$t", this->curtag->tag()->title().toCString(true));
    summary.replace("$d", QDateTime::fromTime_t(this->curtag->audioProperties()->lengthInSeconds()).toString("mm:ss"));
    summary.replace("$a", this->curtag->tag()->artist().toCString(true));
    qInfo() << summary;
    QString body = "$a ($y)";
    body.replace("$a", this->curtag->tag()->album().toCString(true));
    body.replace("$y", QString::number(this->curtag->tag()->year()));

    this->notify(true, summary, body);
}


void mwindow::on_player_EOM() {

    this->prog->setEnabled(false);
    this->prog->setValue(0);
    this->prog->setRange(0, 0);
    this->status.setText("<b>IDLE</b>");
    this->setWindowTitle(QString("OCELOT v") + jag->VERSION);
    this->update_proglabel(0, 0);
}

void mwindow::progslider_set(QTime length) {
    if(QTime(0, 0).secsTo(length) == this->prog->maximum())
        return;

    this->prog->setEnabled(true);
    this->prog->setRange(0, QTime(0, 0).secsTo(length));
    this->prog->setValue(0);
}

void mwindow::child_resized() {
    this->savestate();
}

void mwindow::toolbar_pause() {
    if(this->state == pstate::PLAYING) {
        emit this->player_pause();
        this->state = pstate::PAUSED;

        this->setWindowTitle(this->windowTitle().prepend("[PAUSED] "));
        this->status.setText(this->status.text().replace("PLAYING", "PAUSED"));
    }
}

void mwindow::toolbar_play() {
    if(this->state == pstate::PAUSED) {
        emit this->player_play();
        this->state = pstate::PLAYING;

        this->setWindowTitle(this->windowTitle().remove("[PAUSED] "));
        this->status.setText(this->status.text().replace("PAUSED", "PLAYING"));
    }
}

void mwindow::toolbar_stop() {
    if((this->state == pstate::PAUSED) || (this->state == pstate::PLAYING)) {
        emit this->player_stop();
        this->state = pstate::IDLE;

        this->on_player_EOM();
    }
}

void mwindow::toolbar_next() {
    this->plnext();
}

void mwindow::toolbar_prev() {
    this->plprev();
}

void mwindow::toolbar_menu() {
    // this->configmenu->show();
}

void mwindow::config_spawn() {
    this->sdiag->show();
}

void mwindow::about_spawn() {
    this->adiag->show();
}

void mwindow::transcoder_spawn(QStringList* l, bool discard) {
    if(discard)
        this->transcdiag->clear();
    this->transcdiag->append(l);
    this->transcdiag->show();
}

void mwindow::player_respond(int status) {
    /*this->playerstatus = status;

    switch(status){
    case QMediaPlayer::MediaStatus::NoMedia: // case stopped
        this->coverchanged(new QPixmap());
        this->setWindowTitle(QString("OCELOT v")+jag->VERSION);
        this->status.setText("<b>IDLE</b>");
        this->clearcover();
        this->prog->setValue(0);
        this->prog->setEnabled(false);
        break;
    case QMediaPlayer::MediaStatus::LoadingMedia: //when a new song is being
    loaded break; //BufferedMedia is sent when it finishes case
    QMediaPlayer::MediaStatus::LoadedMedia:
        this->status.setText(this->status.text().replace("PAUSED", "NOW
    PLAYING")); this->setWindowTitle(this->windowTitle().replace("PAUSED :: ",
    "")); break; case QMediaPlayer::MediaStatus::EndOfMedia: this->plnext();
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
    case QMediaPlayer::MediaStatus::BufferedMedia: // this usually means it
    started playing
        //TagLib::FileRef *ref = new TagLib::FileRef();
        QMimeDatabase *db = new QMimeDatabase;
        QString *fmt = new
    QString(db->mimeTypeForFile(fr->file()->name()).name().remove(0,6).toUpper());

        this->prog->setRange(0, this->fr->audioProperties()->lengthInSeconds());
        this->prog->setEnabled(true);

        this->setWindowTitle(QString(this->fr->tag()->artist().toCString(true))+"
    - "+QString(this->fr->tag()->title().toCString(true))+" :: OCELOT
    v"+jag->VERSION); this->status.setText("<b>NOW PLAYING:</b>
    "+QString(this->fr->tag()->title().toCString(true))+" :: "+*fmt+"
    "+QString::number(this->fr->audioProperties()->bitrate())+"kb/s
    @"+QString::number(this->fr->audioProperties()->sampleRate())+"Hz");

        if(db->mimeTypeForFile(this->fr->file()->name()).name().remove(0,6) ==
    "flac"){ // chops the 'audio/' from mime name TagLib::FLAC::File *x = new
    TagLib::FLAC::File(qPrintable(this->fr->file()->name()));
            if(x->pictureList().front() != nullptr){
                QPixmap *np = new QPixmap();
                np->loadFromData((const
    uchar*)x->pictureList().front()->data().data(),
    x->pictureList().front()->data().size()); np->scaled(200, 200,
    Qt::KeepAspectRatio); this->coverchanged(np); np->~QPixmap(); }else{
                //this->gcover->
                //ui->coverview->setText("<b>Cover not set!</b>");
            }

            x->~File();
        }else
    if(db->mimeTypeForFile(this->fr->file()->name()).name().remove(0,6) ==
    "mpeg"){

        }else{
            //return;
        }
        db->~QMimeDatabase();
        fmt->~QString();
        break;
    }
*/
}
