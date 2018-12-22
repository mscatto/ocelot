#include "tagextractor.h"

tagextractor::tagextractor(const QString &url,
                           const QString &mimetype,
                           const KFileMetaData::ExtractionResult::Flags &flags)
                           : KFileMetaData::ExtractionResult (url,mimetype,flags)
{

}

tagextractor::~tagextractor(){

}
