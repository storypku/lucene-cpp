#ifndef RAM_DIRECTORY_H
#define RAM_DIRECTORY_H

namespace Lucene {

class RAMDirectory : public LuceneObject {
public:

protected:
    int64_t m_sizeInBytes;
    friend class RAMFile;
};

} // namespace Lucene

#endif // RAM_DIRECTORY_H
