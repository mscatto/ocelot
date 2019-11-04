#ifndef TRACK_HPP
#define TRACK_HPP
#include <QMap>
#include <QPixmap>
#include <QVariant>
#include <taglib/fileref.h>
#include <taglib/flacfile.h>

class trackdata {
private:
    const char* filepath;
    TagLib::FileRef* fref;

    enum filetype { MP3, OGG, WAV, FLAC, UNKNOWN };
    filetype ft = filetype::UNKNOWN;

public:
    trackdata(const char* filepath);
    ~trackdata();
    QString title();
    QString artist();
    QString album();
    uint year();
    int bitrate();
    int samples();
    uint length_sec();
    QPixmap cover();
};

#endif // TRACK_HPP
