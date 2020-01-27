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
#include "src/library.hpp"
#include "src/trackdata.hpp"
#include "src/vars.hpp"

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
	this->vol = new volslider(this->jag);
	this->bar = new toolbar(this, this->configmenu, this->prog, this->vol, jag);
	this->sdiag = new settings(this, jag);
	this->adiag = new about(this, jag);
	//this->tagdiag = new tageditor(this, jag);
	this->transcdiag = new transcoder(jag, this);
	this->setStatusBar(new QStatusBar());
	this->wb = new workbench(jag, this);

	this->proglabel.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	this->proglabel.setFixedWidth(90);

	this->addToolBar(this->bar);
	this->setCentralWidget(this->wb);
	this->statusBar()->addWidget(&this->status);
	this->statusBar()->addPermanentWidget(&this->proglabel);

	this->setWindowIcon(QIcon(":/internals/caticon"));
	this->setMinimumSize(460, 320);
	this->layout()->setContentsMargins(0, 0, 0, 0);
	this->setWindowTitle(QString("OCELOT v") + jag->VERSION);
	this->sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
	this->sizePolicy().setVerticalPolicy(QSizePolicy::Expanding);
	this->status.setText("<b>IDLE</b>");


	this->update_proglabel();

	/* connect the required signals */
	connect(this, &mwindow::uilock_flip, this, &mwindow::uilock_respond);

	connect(this, &mwindow::player_play, this->jag->pctx, &player::play);
	connect(this, &mwindow::player_load, this, &mwindow::on_player_load);
	connect(this, &mwindow::player_load, this->jag->pctx, &player::load);
	connect(this, &mwindow::player_stop, this->jag->pctx, &player::stop);
	connect(this, &mwindow::player_pause, this->jag->pctx, &player::pause);
	connect(this, &mwindow::player_seek, this->jag->pctx, &player::seek);
	connect(this, &mwindow::player_setvol, this->jag->pctx, &player::setVolume);

	connect(this->jag->pctx, &player::position_changed, this, &mwindow::progslider_sync);
	connect(this->jag->pctx, &player::length_changed, this, &mwindow::progslider_set);

	connect(this->prog, &progslider::sliderMoved, this, &mwindow::progslider_moved);

	connect(this->vol, &volslider::sliderMoved, this, &mwindow::volslider_moved);
	connect(this->vol, &volslider::sliderMoved, this, &mwindow::update_proglabel);

	/* restore the previous saved UI state */
	this->loadstate();
	this->wb->refresh_locks(this->wb->root, new QString(this->jag->fetchdbdata("ui_locks").toString()));

	/* we are now golden */
	qInfo() << "[INFO] Ocelot initialized successfully!\n::";
}

void mwindow::update_proglabel() {
	QString s = "<b>Volume:</b> %v";

	(this->vol->sliderPosition()==this->vol->maximum())?
	s.replace("%v", "MAX"):s.replace("%v", QString::number(this->vol->sliderPosition()));

	//s.replace("%c", QDateTime::fromTime_t(pos).toString("mm:ss")); // current position
	//s.replace("%t", QDateTime::fromTime_t(len).toString("mm:ss")); // total duration

	this->proglabel.setText(s);
}

void mwindow::args_act(const QStringList args) {
	if(args.length() <= 1)
		return; // simply discards the message. maybe bring window to the front?
	if(QMimeDatabase().mimeTypeForFile(args.at(1)).name() == "audio/flac") {
		emit this->player_stop();
		emit this->player_load(args.at(1).toUtf8());
		emit this->player_play();
	}
	// check for multiple files and a less shitty format check
}

void mwindow::resizeEvent(QResizeEvent* event) {
	// this->savestate();
	event->accept();
}

void mwindow::closeEvent(QCloseEvent* event) {
	this->savestate();
	event->accept();
}

void mwindow::mouseReleaseEvent(QMouseEvent* event) {
	//qInfo() << "release!";
	event->accept();
}

void mwindow::moveEvent(QMoveEvent* event) {
	//qInfo() << event->type();
	event->accept();
}

void mwindow::savestate() {
	this->jag->setdbdata("ui_geometry", this->saveGeometry());
	this->jag->setdbdata("ui_state", this->saveState());
	this->jag->setdbdata("ui_maximized", QString::number(this->isMaximized()));
	if(!this->isMaximized()) {
		// this->setdbdata(QString("ui_windowsize"),
		// (QString::number(this->size().width())+","+QString::number(this->size().height())));
		this->jag->setdbdata("ui_windowpos", (QString::number(this->pos().x()) + "," + QString::number(this->pos().y())));
	}

	QList<QSplitter*> spl;
	this->wb->dumpsplitters(this->wb->root, &spl);
	QSqlQuery("DELETE FROM splitterstate").exec();

	foreach(QSplitter* s, spl) {
		QByteArray ba = s->saveState();
		QSqlQuery x(*this->jag->DB_REF);
		x.prepare("INSERT INTO splitterstate VALUES(:raw)");
		x.bindValue(":raw", ba);
		x.exec();
	}

	this->jag->setdbdata("ui_tbsplstate", this->bar->splitterstate());
}

void mwindow::loadstate() {
	if(this->jag->fetchdbdata("ui_maximized") == "1")
		this->showMaximized();

	QStringList tmppos = this->jag->fetchdbdata("ui_windowpos").toString().split(",");
	this->move(QPoint(tmppos.first().toInt(), tmppos.last().toInt()));

	this->restoreGeometry(this->jag->fetchdbdata("ui_geometry").toByteArray());
	this->restoreState(this->jag->fetchdbdata("ui_state").toByteArray());

	QList<QSplitter*> spl;
	this->wb->dumpsplitters(this->wb->root, &spl);
	QSqlQuery q("SELECT val FROM splitterstate", *this->jag->DB_REF);
	q.exec();
	q.next();
	if(!q.isValid())
		return;
	foreach(QSplitter* s, spl) {
		s->restoreState(q.value(0).toByteArray());
		q.next();
	}

	this->bar->restore_splitterstate(this->jag->fetchdbdata("ui_tbsplstate").toByteArray());
}

void mwindow::volslider_moved(int x) {
	QPoint p = this->vol->mapToGlobal(this->vol->pos());
	(this->vol->orientation() == Qt::Horizontal) ? p.setX(QCursor::pos().rx()) : p.setY(QCursor::pos().ry());
	QToolTip::showText(p, QString::number(x), this->vol);

	emit player_setvol(uint(x));
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
void mwindow::progslider_sync(qint64 pos) {
	this->prog->setValue(pos/1000);
	//this->update_proglabel(QTime(0, 0).secsTo(pos), this->prog->maximum());
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
	this->wb->lock_flip();
	if(!this->wb->islocked()) {
		this->status.setText("<b>LAYOUT EDITOR ENABLED!</b> :: You may disable "
							 "it inside the cogwheel toolbar menu when you're done.");
		this->setWindowTitle(QString("[LAYOUT EDITOR ENABLED] OCELOT v") + this->jag->VERSION);
	} else {
		this->status.setText("<b>IDLE</b");
		this->setWindowTitle(QString("OCELOT v") + this->jag->VERSION);
	}
}

void mwindow::notify(bool playing, QString summary, QString body) {
	Notification notify(summary);
	playing ? notify.init("OCELOT") : notify.init("OCELOT");

	notify.setBody(body);
	notify.setIconName("caticon");
	notify.setCategory("playback");
	notify.setTimeout(2000);
	notify.setUrgency(NOTIFICATION_URGENCY_LOW);

	notify.show();
}

void mwindow::on_player_load(const QString& filepath){
	if(this->track != nullptr)
		this->track->~trackdata();
	this->track = new trackdata(filepath.toUtf8());
	this->state = pstate::PLAYING;

	this->prog->setEnabled(true);
	this->cover_set(this->track->cover());
	emit this->player_setvol(static_cast<uint>(this->vol->value()));

	this->setWindowTitle(QString("OCELOT v") + this->jag->VERSION);
	this->setWindowTitle(this->track->artist() + ": " + this->track->title() + " :: " + this->windowTitle());
	this->status.setText("<b>PLAYING:</b> " + this->track->title()
						 + " :: " + QMimeDatabase().mimeTypeForFile(filepath).name().remove(0, 6).toUpper().append(" ")
						 + QString::number(this->track->bitrate()) + "kb/s @" + QString::number(this->track->samples()) + "Hz");

	QString summary = "$t ($d)\n$a";
	summary.replace("$t", this->track->title());
	summary.replace("$d", QDateTime::fromTime_t(this->track->length_sec()).toString("mm:ss"));
	summary.replace("$a", this->track->artist());

	QString body = "$a ($y)";
	body.replace("$a", this->track->album());
	body.replace("$y", QString::number(this->track->year()));

	this->notify(true, summary, body);
}

void mwindow::on_playlist_end() {
	this->prog->setEnabled(false);
	this->prog->setValue(0);
	this->prog->setRange(0, 0);
	this->status.setText("<b>IDLE</b>");
	this->setWindowTitle(QString("OCELOT v") + jag->VERSION);
	//this->update_proglabel(0, 0);
	this->cover_set(QPixmap());
}

void mwindow::progslider_set(qint64 length) {
	if(length/1000 == this->prog->maximum())
		return;

	this->prog->setEnabled(true);
	this->prog->setRange(0, length/1000);
	this->prog->setValue(0);
	emit this->prog->valueChanged(0);
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

		this->on_playlist_end();
	}
}

void mwindow::toolbar_next() {
	emit this->playlist_next();
}

void mwindow::toolbar_prev() {
	emit this->playlist_prev();
}

void mwindow::config_spawn() {
	this->sdiag->show();
}

void mwindow::about_spawn() {
	this->adiag->show();
}

void mwindow::playlist_enqueue(const QStringList files){
	if(this->jag->fetchdbdata("general_playlistappend")=="1")
		emit this->playlist_append(files);
	else
		emit this->playlist_replace(files);

}

void mwindow::transcoder_spawn(QStringList* l, bool discard) {
	if(discard)
		this->transcdiag->clear();
	this->transcdiag->append(l);
	this->transcdiag->show();
}
