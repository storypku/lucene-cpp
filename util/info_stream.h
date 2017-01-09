#ifndef INFO_STREAM_H
#define INFO_STREAM_H

#include "lucene_object.h"
#include <fstream>

namespace Lucene {

/// Utility class to support streaming info messages.
class InfoStream : public LuceneObject {
protected:
    InfoStream();

public:
    virtual ~InfoStream();
    LUCENE_CLASS(InfoStream);

public:
    virtual InfoStream& operator<< (const String& t) = 0;
};

class InfoStreamFile : public InfoStream {
public:
    InfoStreamFile(const String& path);
    virtual ~InfoStreamFile();

    LUCENE_CLASS(InfoStreamFile);

protected:
    std::ofstream m_file;

public:
    virtual InfoStreamFile& operator<< (const String& t);
};

/// Stream override to write messages to a std::out
class InfoStreamOut : public InfoStream {
public:
    virtual ~InfoStreamOut();
    LUCENE_CLASS(InfoStreamOut);

public:
    virtual InfoStreamOut& operator<< (const String& t);
};

/// Null stream override to eat messages.
class InfoStreamNull : public InfoStream {
public:
    virtual ~InfoStreamNull();
    LUCENE_CLASS(InfoStreamNull);

public:
    virtual InfoStreamNull& operator<< (const String& t);
};

} // namespace Lucene

#endif // INFO_STREAM_H
