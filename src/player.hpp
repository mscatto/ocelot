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

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <QMainWindow>
#include <QObject>
#include <QThread>
#include <QTimer>
#include <QTreeWidgetItem>

#include <Qt5GStreamer/QGst/Init>
#include <Qt5GStreamer/QGst/Pipeline>
#include <Qt5GStreamer/QGst/TagList>

class player : public QObject {
    Q_OBJECT
private:
    void onBusMessage(const QGst::MessagePtr& message);
    void handlePipelineStateChange(const QGst::StateChangedMessagePtr& scm);

    QGst::PipelinePtr m_pipeline;
    QTimer m_positionTimer;

    QTime position() const;
    QTime length() const;
    QTime clen;

public:
    player();
    ~player();
private slots:
    void position_proxy();
public slots:
    void play();
    void stop();
    void pause();
    void seek(short sec);
    void set(QString file);
    void setVolume(ushort v);
signals:
    void position_changed(QTime pos);
    void length_changed(QTime len);
    void paused(bool paused);
    void EOM();
    // void length_changed(QTime len);
};
#endif // PLAYER_HPP
