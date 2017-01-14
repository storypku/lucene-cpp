#include "lucene_exception.h"

namespace Lucene {

LuceneException::LuceneException(const String& error, ExceptionType type) throw()
    : m_type(type) , m_error(error) {
}

LuceneException::~LuceneException() throw() {
}

LuceneException::ExceptionType LuceneException::get_type() const {
    return m_type;
}

String LuceneException::get_error() const {
    return m_error;
}

bool LuceneException::is_null() const {
    return (m_type == Null);
}

void LuceneException::throw_exception() {
    switch (m_type) {
    case LuceneException::AlreadyClosed:
        throw AlreadyClosedException(m_error, m_type);

    case LuceneException::Compression:
        throw CompressionException(m_error, m_type);

    case LuceneException::CorruptIndex:
        throw CorruptIndexException(m_error, m_type);

    case LuceneException::FieldReader:
        throw FieldReaderException(m_error, m_type);

    case LuceneException::FileNotFound:
        throw FileNotFoundException(m_error, m_type);

    case LuceneException::IllegalArgument:
        throw IllegalArgumentException(m_error, m_type);

    case LuceneException::IllegalState:
        throw IllegalStateException(m_error, m_type);

    case LuceneException::IndexOutOfBounds:
        throw IndexOutOfBoundsException(m_error, m_type);

    case LuceneException::IO:
        throw IOException(m_error, m_type);

    case LuceneException::LockObtainFailed:
        throw LockObtainFailedException(m_error, m_type);

    case LuceneException::LockReleaseFailed:
        throw LockReleaseFailedException(m_error, m_type);

    case LuceneException::Lookahead:
        throw LookaheadSuccess(m_error, m_type);

    case LuceneException::MergeAborted:
        throw MergeAbortedException(m_error, m_type);

    case LuceneException::Merge:
        throw MergeException(m_error, m_type);

    case LuceneException::NoSuchDirectory:
        throw NoSuchDirectoryException(m_error, m_type);

    case LuceneException::NullPointer:
        throw NullPointerException(m_error, m_type);

    case LuceneException::NumberFormat:
        throw NumberFormatException(m_error, m_type);

    case LuceneException::OutOfMemory:
        throw OutOfMemoryError(m_error, m_type);

    case LuceneException::Parse:
        throw ParseException(m_error, m_type);

    case LuceneException::QueryParser:
        throw QueryParserError(m_error, m_type);

    case LuceneException::Runtime:
        throw RuntimeException(m_error, m_type);

    case LuceneException::StaleReader:
        throw StaleReaderException(m_error, m_type);

    case LuceneException::StopFillCache:
        throw StopFillCacheException(m_error, m_type);

    case LuceneException::Temporary:
        throw TemporaryException(m_error, m_type);

    case LuceneException::TimeExceeded:
        throw TimeExceededException(m_error, m_type);

    case LuceneException::TooManyClauses:
        throw TooManyClausesException(m_error, m_type);

    case LuceneException::UnsupportedOperation:
        throw UnsupportedOperationException(m_error, m_type);

    case LuceneException::Null:
        // silence static analyzer
        break;
    }
}

} // namespace Lucene
