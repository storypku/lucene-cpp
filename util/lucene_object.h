#ifndef LUCENE_OBJECT_H
#define LUCENE_OBJECT_H

#include <memory>
#include <string>
#include <fstream>

#include "lucene_sync.h"

#define LUCENE_INTERFACE(Name) \
    static String _get_class_name() { return #Name; } \
    virtual String get_class_name() { return #Name; }

#define LUCENE_CLASS(Name) \
    LUCENE_INTERFACE(Name); \
    std::shared_ptr<Name> shared_from_this() { return std::static_pointer_cast<Name>(LuceneObject::shared_from_this()); } \

namespace Lucene {

typedef std::string String;
const String EmptyString;

typedef std::shared_ptr<std::ifstream> IfstreamPtr;

} // namespace Lucene

namespace Lucene {

/// Base class for all Lucene classes
class LuceneObject : public LuceneSync, public std::enable_shared_from_this<LuceneObject> {
public:
    virtual ~LuceneObject();

protected:
    LuceneObject();

public:
    /// Called directly after instantiation to create objects that depend on this object being
    /// fully constructed.
    virtual void initialize();

    /// Return clone of this object
    /// @param other clone reference - null when called initially, then set in top virtual override.
    virtual LuceneObjectPtr clone(const LuceneObjectPtr& other = LuceneObjectPtr());

    /// Return hash code for this object.
    virtual int32_t hash_code();

    /// Return whether two objects are equal
    virtual bool equals(const LuceneObjectPtr& other);

    /// Compare two objects
    virtual int32_t compare_to(const LuceneObjectPtr& other);

    /// Returns a string representation of the object
    virtual String to_string();
};

} // namespace Lucene

#endif // LUCENE_OBJECT_H
