#ifndef SINGLE_INSTANCE_LOCK_FACTORY_H
#define SINGLE_INSTANCE_LOCK_FACTORY_H

#include "lock_factory.h"

namespace Lucene {

class SingleInstanceLockFactory : public LockFactory {
public:
    SingleInstanceLockFactory() = default;
    virtual ~SingleInstanceLockFactory() = default;

};

} // namespace Lucene

#endif // SINGLE_INSTANCE_LOCK_FACTORY_H
