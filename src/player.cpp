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
#include <Qt5GStreamer/QGlib/Connect>
#include <Qt5GStreamer/QGlib/Error>
#include <Qt5GStreamer/QGst/Bus>
#include <Qt5GStreamer/QGst/ClockTime>
#include <Qt5GStreamer/QGst/ElementFactory>
#include <Qt5GStreamer/QGst/Event>
#include <Qt5GStreamer/QGst/Init>
#include <Qt5GStreamer/QGst/Message>
#include <Qt5GStreamer/QGst/Pipeline>
#include <Qt5GStreamer/QGst/Query>
#include <Qt5GStreamer/QGst/StreamVolume>

player::player() : QObject() {
    QGst::init();

    if(!m_pipeline) {
        m_pipeline = QGst::ElementFactory::make("playbin").dynamicCast<QGst::Pipeline>();
        if(m_pipeline) {
            QGst::BusPtr bus = m_pipeline->bus();
            bus->addSignalWatch();
            QGlib::connect(bus, "message", this, &player::onBusMessage);
        } else {
            qFatal("[FATAL] GStreamer: Failed to create pipeline");
        }
    }

    connect(&this->m_positionTimer, &QTimer::timeout, this, &player::position_proxy);
    // connect(this, &player::position_proxy, this, &player::microdik);
}

void player::position_proxy() {
    // if(QTime(0, 0).secsTo(this->length()) > 0)
    emit this->position_changed(this->position());
}


player::~player() {
    if(m_pipeline) {
        m_pipeline->setState(QGst::StateNull);
    }
}

QTime player::position() const {
    if(m_pipeline) {
        // here we query the pipeline about its position
        // and we request that the result is returned in time format
        QGst::PositionQueryPtr posquery = QGst::PositionQuery::create(QGst::FormatTime);
        m_pipeline->query(posquery);
        return QGst::ClockTime(posquery->position()).toTime();
    } else {
        return QTime(0, 0);
    }
}

QTime player::length() const {
    QGst::DurationQueryPtr query = QGst::DurationQuery::create(QGst::FormatTime);
    m_pipeline->query(query);

    return QGst::ClockTime(query->duration()).toTime();
}

void player::onBusMessage(const QGst::MessagePtr& message) {
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
}

void player::handlePipelineStateChange(const QGst::StateChangedMessagePtr& scm) {
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
    default:
        break;
    }
}

void player::play() {
    if(m_pipeline) {
        m_pipeline->setState(QGst::StatePlaying);
    }
}

void player::stop() {
    if(m_pipeline) {
        m_pipeline->setState(QGst::StateNull);
    }
}

void player::pause() {
    if(m_pipeline) {
        m_pipeline->setState(QGst::StatePaused);
    }
}

void player::set(QString file) {
    QString url;
    // if uri is not a real uri, assume it is a file path
    if(file.indexOf("://") < 0) {
        url = QUrl::fromLocalFile(file).toEncoded();
    }

    if(!m_pipeline) {
        qInfo() << "pipeline missing";
        m_pipeline = QGst::ElementFactory::make("playbin").dynamicCast<QGst::Pipeline>();
        if(m_pipeline) {
            // watch the bus for messages
            QGst::BusPtr bus = m_pipeline->bus();
            bus->addSignalWatch();
            QGlib::connect(bus, "message", this, &player::onBusMessage);
        } else {
            qCritical() << "Failed to create the pipeline";
        }
    }

    if(m_pipeline) {
        m_pipeline->setProperty("uri", url);
    }
}

void player::setVolume(ushort v) {
    if(m_pipeline) {
        QGst::StreamVolumePtr svp = m_pipeline.dynamicCast<QGst::StreamVolume>();

        if(svp) {
            svp->setVolume(v/8.192, QGst::StreamVolumeFormatLinear);
        }
    }
}

void player::seek(short sec) {
    uint length = -this->length().secsTo(QTime(0, 0));
    if(length != 0 && sec > 0) {
        QTime pos(0, 0);
        pos = pos.addSecs(sec);
        QGst::SeekEventPtr evt
        = QGst::SeekEvent::create(1.0, QGst::FormatTime, QGst::SeekFlagFlush, QGst::SeekTypeSet,
                                  QGst::ClockTime::fromTime(pos), QGst::SeekTypeNone, QGst::ClockTime::None);
        m_pipeline->sendEvent(evt);
    }
}
