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
/*
#include "mainwindow.hpp"
//#include "ui_mainwindow.h"
#include "database.hpp"
#include "configs.hpp"
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/tfile.h>
#include <flacfile.h>
#include <QLabel>
#include <QString>
#include <QList>
#include <QSqlRecord>
#include <QFileInfo>
#include <QToolTip>
#include <QDateTime>
#include <QMenu>
#include <QPushButton>
#include <QMediaPlayer>
#include <QMimeDatabase>
#include <QAction>
#include <taglib/flacpicture.h>
#include <taglib/tbytevector.h>
#include <taglib/attachedpictureframe.h>
#include <tpropertymap.h>

#include "cwidgets/core_coverview.hpp"

MainWindow::MainWindow(state *nst, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    st = nst;
    ui->setupUi(this);

    /* init player */
    MainWindow::player = new QMediaPlayer;
    player->setVolume(100);
    player->setNotifyInterval(200);

    /* init sliders */
    progslider = new QSlider(Qt::Horizontal, ui->mainToolBar);
    volslider = new QSlider(Qt::Horizontal, ui->mainToolBar);
    progslider->setTracking(true);
    progslider->setToolTip("Playback progress");
    volslider->setTracking(true);
    volslider->setMaximumWidth(120);
    volslider->setRange(0, 100);
    volslider->setValue(100);
    volslider->setToolTip("Playback volume");
    //volslider->setStyleSheet("height: 18");

    /* init labels */
    statusBar()->insertWidget(0, &infolabel, 1);
    statusBar()->insertWidget(1, &poslabel, 2);
    poslabel.setText("::");
    poslabel.setTextFormat(Qt::PlainText);
    poslabel.setFrameStyle(QFrame::Panel | QFrame::StyledPanel);
    poslabel.setAlignment(Qt::AlignBottom | Qt::AlignRight);

    infolabel.setTextFormat(Qt::RichText);
    infolabel.setFrameStyle(QFrame::Panel | QFrame::StyledPanel);
    infolabel.setAlignment(Qt::AlignBottom | Qt::AlignLeft);

    on_player_state(QMediaPlayer::MediaStatus::NoMedia);

    /* init toolbar and connect items */
    //ui->mainToolBar->
    ui->mainToolBar->addAction(new QAction(QIcon(":/internals/stop"), ""));
    ui->mainToolBar->addAction(new QAction(QIcon(":/internals/play"), ""));
    ui->mainToolBar->addAction(new QAction(QIcon(":/internals/pause"), ""));
    ui->mainToolBar->addAction(new QAction(QIcon(":/internals/prev"), ""));
    ui->mainToolBar->addAction(new QAction(QIcon(":/internals/next"), ""));
    ui->mainToolBar->addAction(new QAction(QIcon(":/internals/gear"), ""));

    ui->mainToolBar->actions().first()->setToolTip("Stop media playback");
    connect(ui->mainToolBar->actions().first(), SIGNAL(triggered()), this, SLOT(toolbar_stop()));

    ui->mainToolBar->actions().at(1)->setToolTip("Play selected media");
    connect(ui->mainToolBar->actions().at(1), SIGNAL(triggered()), this, SLOT(toolbar_play()));

    ui->mainToolBar->actions().at(2)->setToolTip("Pause media playback");
    connect(ui->mainToolBar->actions().at(2), SIGNAL(triggered()), this, SLOT(toolbar_pause()));

    ui->mainToolBar->actions().at(3)->setToolTip("Previous queued media");
    connect(ui->mainToolBar->actions().at(3), SIGNAL(triggered()), this, SLOT(toolbar_prev()));

    ui->mainToolBar->actions().at(4)->setToolTip("Next queued media");
    connect(ui->mainToolBar->actions().at(4), SIGNAL(triggered()), this, SLOT(toolbar_next()));

    ui->mainToolBar->actions().at(5)->setToolTip("Configuration");
    connect(ui->mainToolBar->actions().at(5), SIGNAL(triggered()), this, SLOT(toolbar_config()));

    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(volslider);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(progslider);


    /* init playlist view */
    ui->playlist->setColumnWidth(0,36);
    ui->playlist->setColumnWidth(1,64);
    ui->playlist->setColumnWidth(2,260);

    /* initialize properties view */
    ui->properties->setColumnCount(2);
    ui->properties->setColumnWidth(0, 200);
    ui->properties->setHeaderHidden(true);
    ui->properties->setRootIsDecorated(false);
    ui->properties->setAlternatingRowColors(true);

    QStringList x;
    x << "Your tags will be shown here.." << "whenever you select or play a track";

    ui->properties->insertTopLevelItem(0, new QTreeWidgetItem(ui->properties, x));

    /* then connect respective signals */
    //connect(ui->playlist, SIGNAL(ct)), this, SLOT(on_playlist_customContextMenuRequested(QPoint)));
    //connect(volslider, SIGNAL(sliderRelustomContextMenuRequested(QPoineased())), this, SLOT(on_volslider_released()));
    volslider->connect(volslider, SIGNAL(sliderMoved(int)), this, SLOT(on_volslider_moved(int)));

    player->connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(handlepos(qint64)));
    player->connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(on_player_state(QMediaPlayer::MediaStatus)));
    progslider->connect(progslider, SIGNAL(sliderMoved(int)), this, SLOT(on_progslider_moved(int)));
    progslider->connect(progslider, SIGNAL(sliderReleased()), this, SLOT(on_progslider_released()));
    progslider->connect(progslider, SIGNAL(actionTriggered(int)), this, SLOT(on_progslider_action(int x)));

}

void MainWindow::setlib(library *nlib){
    MainWindow::lib = nlib;
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* extract every word whitin # from a string
 *
 * returns stringlist with words
 */
QStringList *MainWindow::extract(QString vars){
    QStringList *out = new QStringList();
    QRegularExpressionMatchIterator reg = QRegularExpression("#(\\w+)#").globalMatch(vars);
    while(reg.hasNext())
        out->append(reg.next().captured(1));

    return out;
}

/* take one and pass it ahead */
/* i'm so uglyyyyyyy */
void MainWindow::recurtree(QTreeWidgetItem *parent, QStringList levels, QString conditions){
    QString root = levels.first();
    levels.removeFirst();
    QStringList* sroot = MainWindow::extract(root);
    QString vars = sroot->join(", ");

    QSqlQuery rq("SELECT DISTINCT "+vars+" FROM songs WHERE "+conditions+" ORDER BY "+sroot->first());

    while(rq.next()){
        QString name = root;
        sroot = MainWindow::extract(root);

        for(int i=sroot->length()-1; i>=0; i--){
            name.replace(sroot->at(i), rq.record().value(i).toString());
            sroot->replace(i, sroot->at(i)+"='"+rq.record().value(i).toString()+"'");
        }

        name.remove("#");

        parent->addChild(new QTreeWidgetItem(parent,QStringList(name)));

        QSqlQuery qpath("SELECT DISTINCT path FROM songs WHERE "+sroot->join(" AND "));
        QStringList act;
        while(qpath.next()){
            act.append(qpath.record().value(0).toString());
        }
        parent->child(parent->childCount()-1)->setData(1, Qt::EditRole, act);

        if(levels.length()>=1){
            MainWindow::recurtree(parent->child(parent->childCount()-1), levels, conditions+" AND "+sroot->join(" AND "));
            parent->child(parent->childCount()-1)->setText(0, parent->child(parent->childCount()-1)->text(0)+" ("+QString::number(parent->child(parent->childCount()-1)->childCount())+")");

        }
    }
}

void MainWindow::handlepos(qint64 x){
    if (!progslider->isSliderDown()){
           progslider->setValue(x/1000);
           poslabel.setText(QDateTime::fromTime_t(x/1000).toString("mm:ss")+" / "+QDateTime::fromTime_t(player->duration()/1000).toString("mm:ss"));

    }
           //progslider->ti
    //progslider->update();
    //qDebug() << x/1000 << progslider->value();
}


void MainWindow::showlabel(QString pos){
    //MainWindow::ui->stats->children().empty();
}

void MainWindow::pop_tree(database *db){
    QStringList order = QString("#artist#/[#year#] #album#/#track#. #title#").split("/");

    QSqlQuery q("SELECT DISTINCT "+MainWindow::extract(order.first())->join(", ")+" FROM songs ORDER BY "+MainWindow::extract(order.first())->join(", "));
    QString root = order.first();
    order.removeFirst();
    QList<QTreeWidgetItem*> items;
    while(q.next()){
        QStringList *caseq = MainWindow::extract(root);
        QString name = root;
        for(int i=0; i<caseq->length(); i++){
            name.replace(caseq->at(i), q.record().value(i).toString());
            caseq->replace(i, caseq->at(i) + "='"+q.record().value(i).toString()+"'");
        }
        name.remove("#");

        items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(name)));
        QSqlQuery qpath("SELECT DISTINCT path FROM songs WHERE "+caseq->join(" AND "));
        QStringList act;
        while(qpath.next()){
            act.append(qpath.record().value(0).toString());
        }

        items.last()->setData(1, Qt::EditRole, act);
        MainWindow::recurtree(items.last(), order, caseq->join(" AND "));
        caseq->~QStringList();
        items.last()->setText(0, items.last()->text(0)+" ("+QString::number(items.last()->childCount())+")");
    }

    ui->treelib->insertTopLevelItem(0, new QTreeWidgetItem((QTreeWidget*)0, QStringList("")));
    ui->treelib->topLevelItem(0)->addChildren(items);
    ui->treelib->topLevelItem(0)->setText(0, "All Media ("+QString::number(ui->treelib->topLevelItem(0)->childCount())+")");
    ui->treelib->expandItem(ui->treelib->topLevelItem(0));
}

void MainWindow::toolbar_stop(){
    player->stop();
    poslabel.setText(":1:");
}

void MainWindow::toolbar_play(){
    if(player->state() == QMediaPlayer::PausedState){
        infolabel.setText(infolabel.text().replace("PAUSED", "NOW PLAYING"));
        this->setWindowTitle(this->windowTitle().remove("[PAUSED] :: "));
    }

    player->play();
}

void MainWindow::toolbar_pause(){
    if(player->state() == QMediaPlayer::PlayingState){
        player->pause();
        infolabel.setText(infolabel.text().replace("NOW PLAYING", "PAUSED"));
        this->setWindowTitle(this->windowTitle().insert(0,"[PAUSED] :: "));
    }
}

void MainWindow::toolbar_next(){
//todo
}

void MainWindow::toolbar_prev(){
//todo
}

void MainWindow::toolbar_config(){
    qDebug() << "hammertime!";
}

void MainWindow::on_progslider_moved(int value){
    QToolTip::showText(QCursor::pos(), QDateTime::fromTime_t(value).toString("mm : ss"), progslider);
    //progslider->
}

void MainWindow::on_progslider_released(){
    player->setPosition(progslider->value()*1000);
}

void MainWindow::on_player_state(QMediaPlayer::MediaStatus x){
    switch(x){
        case QMediaPlayer::MediaStatus::EndOfMedia:
        case QMediaPlayer::MediaStatus::LoadedMedia:
        case QMediaPlayer::MediaStatus::NoMedia:
        case QMediaPlayer::MediaStatus::InvalidMedia:
            this->setWindowTitle("OCELOT v0.1");
            infolabel.setText("<b>IDLE</b>");
            ui->coverview->clear();
            ui->coverview->setText("<b>Nothing to show</b>");
            progslider->setValue(0);
            progslider->setEnabled(false);
        break;

        case QMediaPlayer::MediaStatus::StalledMedia:
        case QMediaPlayer::MediaStatus::BufferingMedia:
        case QMediaPlayer::MediaStatus::BufferedMedia:
            TagLib::FileRef *fr = new TagLib::FileRef(qPrintable(player->media().canonicalResource().url().path()));
            if(fr->isNull()){
                qWarning() << "QMediaPlayer::MediaStatus reports that a invalid media has been loaded!";
                qWarning() << ">source: "+ player->media().canonicalResource().url().path();
                return;
            }
            progslider->setEnabled(true);
            QMimeDatabase *mdb = new QMimeDatabase();
            QString *fmt = new QString(mdb->mimeTypeForFile(player->media().canonicalResource().url().path()).name().remove(0,6).toUpper());
            this->setWindowTitle(QString(fr->tag()->artist().toCString(true))+" - "+QString(fr->tag()->title().toCString(true))+" :: OCELOT v0.1");
            infolabel.setText("<b>NOW PLAYING:</b> "+QString(fr->tag()->title().toCString(true))+" :: "+*fmt+" "+QString::number(fr->audioProperties()->bitrate())+"kb/s @"+QString::number(fr->audioProperties()->sampleRate())+"Hz");

            fr->~FileRef();
            mdb->~QMimeDatabase();
            fmt->~QString();
        break;
    }
}


void MainWindow::play(QString fpath){
    qInfo() << "Playing file "+fpath;
    player->stop();
    player->setMedia(QUrl::fromLocalFile(fpath));
    player->play();
    populate_tags(new TagLib::FileRef(qPrintable(fpath)));
    QMimeDatabase db;
    if(db.mimeTypeForFile(fpath).name().remove(0,6) == "flac"){ /* chops the 'audio/' from mime name */
        TagLib::FLAC::File *x = new TagLib::FLAC::File(qPrintable(fpath));
        if(x->pictureList().front() != nullptr){
            QPixmap *a = new QPixmap();
            a->loadFromData((const uchar*) x->pictureList().front()->data().data(), x->pictureList().front()->data().size());
            ui->coverview->setPixmap(a->scaled(ui->coverview->width(), ui->coverview->height(), Qt::KeepAspectRatio));
            a->~QPixmap();
        }else{
            ui->coverview->clear();
            ui->coverview->setText("<b>Cover not set!</b>");
        }

        progslider->setRange(0, x->audioProperties()->lengthInSeconds());

        x->~File();
    }else if(db.mimeTypeForFile(fpath).name().remove(0,6) == "mpeg"){

    }else{
        return;
    }

}

void MainWindow::on_treelib_itemClicked(QTreeWidgetItem *item, int column){

}

void MainWindow::on_treelib_itemDoubleClicked(QTreeWidgetItem *item, int column){
    QStringList data = item->data(1,Qt::EditRole).toStringList();

    play(data.first());

    ui->playlist->clear(); // should be optional
    QList<QTreeWidgetItem*> *pl = new QList<QTreeWidgetItem*>;
    foreach(QString t, data){
        TagLib::FileRef *tag = new TagLib::FileRef(qPrintable(t));
        //tag->file()->
        pl->append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString::number(pl->length()+1))));
        pl->last()->setData(10, Qt::EditRole, data.first());
        pl->last()->setData(1, Qt::DisplayRole, QString::number(tag->tag()->track()));
        pl->last()->setData(2, Qt::DisplayRole, QString(tag->tag()->title().toCString(true)));
        pl->last()->setData(3, Qt::DisplayRole, QString(tag->tag()->album().toCString(true)));
        pl->last()->setData(4, Qt::DisplayRole, QString(tag->tag()->artist().toCString(true)));
        tag->~FileRef();
    }
    ui->playlist->addTopLevelItems(*pl);
}

void MainWindow::on_playlist_customContextMenuRequested(const QPoint &pos){
    // Handle global position
        //QPoint globalPos = ui->tabWidget->mapToGlobal(pos);

        // Create menu and insert some actions
        QMenu ctx;
        ctx.addAction("Insert", this, SLOT(addItem()));
        ctx.addAction("Erase",  this, SLOT(eraseItem()));

        // Show context menu at handling position
        //ctx.exec(globalPos);
}

void MainWindow::on_progslider_action(int x){
    switch(x){
    case QAbstractSlider::SliderSingleStepAdd:
        /*progslider->set
    case :
    case :
    case :
    case :
    case :*/

    break;
    }
}

void MainWindow::populate_tags(TagLib::FileRef *t){
    TagLib::PropertyMap x = t->tag()->properties();
    QList<QTreeWidgetItem*> info;
    //x.
    /*for(int i=0;i<x.size();i++){

    }*/
    t->~FileRef();
}

void MainWindow::on_volslider_released(){
    player->setVolume(volslider->value());
}
void MainWindow::on_volslider_moved(int value){
    QToolTip::showText(QCursor::pos(), QString::number(value), volslider);
    player->setVolume(volslider->value());
}
*/
