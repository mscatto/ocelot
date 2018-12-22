#ifndef TAGEXTRACTOR_H
#define TAGEXTRACTOR_H

#include <QObject>
#include <KFileMetaData/ExtractionResult>

class tagextractor : public KFileMetaData::ExtractionResult
{
public:
    tagextractor(const QString &url, const QString &mimetype = QString(), const Flags &flags = ExtractEverything);
    ~tagextractor();
};

#endif // TAGEXTRACTOR_H
