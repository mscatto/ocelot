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

#include "src/player.hpp"
#include "src/gui/mwindow.hpp"

#include <QDebug>
#include <QTime>

player::player() : QObject() {
	this->instance = new QMediaPlayer(this, QMediaPlayer::LowLatency);

	connect(this->instance, &QMediaPlayer::mediaStatusChanged, this, &player::status_changed);
	connect(this->instance, &QMediaPlayer::positionChanged, this, &player::position_changed);
	connect(this->instance, &QMediaPlayer::durationChanged, this, &player::length_changed);

	//connect(&this->m_positionTimer, &QTimer::timeout, this, &player::position_proxy);
	// connect(this, &player::position_proxy, this, &player::microdik);
}

void player::track_preload(){
	qInfo()<<"track preloaded!";
}

void player::status_changed(QMediaPlayer::MediaStatus status){
	switch(status){
	case QMediaPlayer::MediaStatus::NoMedia:

		break;
	case QMediaPlayer::MediaStatus::LoadingMedia:
		//emit length_changed(this->length());
		break;
	case QMediaPlayer::MediaStatus::LoadedMedia:

		break;
	case QMediaPlayer::MediaStatus::BufferedMedia:

		break;
	case QMediaPlayer::MediaStatus::EndOfMedia:
		emit EOM();
		break;
	case QMediaPlayer::MediaStatus::InvalidMedia:

		break;
	case QMediaPlayer::MediaStatus::StalledMedia:

		break;
	case QMediaPlayer::MediaStatus::BufferingMedia:

		break;
	case QMediaPlayer::MediaStatus::UnknownMediaStatus:

		break;

	}
	//qInfo()<<status;
}

void player::play() {
	this->instance->play();
}

void player::stop() {
	this->instance->stop();
	this->instance->setMedia(QMediaContent());
}

void player::pause() {
	this->instance->pause();
}

void player::load(const QString& filepath){
	this->instance->setMedia(QUrl::fromLocalFile(filepath));
}

void player::setVolume(ushort v) {
	this->instance->setVolume(v*10);
}

void player::seek(short sec) {
	this->instance->setPosition(sec*1000);
}

/*QTime player::position() const {
	if(m_pipeline) {
		// here we query the pipeline about its position
		// and we request that the result is returned in time format
		QGst::PositionQueryPtr posquery = QGst::PositionQuery::create(QGst::FormatTime);
		m_pipeline->query(posquery);
		return QGst::ClockTime(posquery->position()).toTime();
	} else {
		return QTime(0, 0);
	}
}*/

/*void player::onBusMessage(const QGst::MessagePtr& message) {
	switch(message->type()) {
	case QGst::MessageEos: // End of stream. We reached the end of the file.
		m_positionTimer.stop();
		emit EOM();
		break;
	case QGst::MessageError: // Some error occurred.
		qCritical() << message.staticCast<QGst::ErrorMessage>()->error();
		stop();
		break;
	case QGst::MessageStateChanged: // The element in message->source() has
									// changed state
		if(message->source() == m_pipeline) {
			handlePipelineStateChange(message.staticCast<QGst::StateChangedMessage>());
		}
		break;
	case QGst::MessageInfo:
		qInfo() << "lollery";
	default:
		break;
	}
}*/

/*void player::handlePipelineStateChange(const QGst::StateChangedMessagePtr& scm) {
	switch(scm->newState()) {
	case QGst::StatePlaying:
		// if(scm->oldState() == QGst::StatePaused)
		// emit paused(false);
		// start the timer when the pipeline starts playing
		m_positionTimer.start(100);

		if(this->length() != this->clen) {
			this->clen = this->length();
			emit length_changed(this->clen);
		}
		break;
	case QGst::StatePaused:
		// stop the timer when the pipeline pauses
		if(scm->oldState() == QGst::StatePlaying) {
			emit paused(true);
			m_positionTimer.stop();
		}
		break;
	case QGst::StateNull:
		qInfo() << "statenull";
		break;
	//case QGst::state
	default:
		break;
	}
}*/


