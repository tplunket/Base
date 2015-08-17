//
//  Copyright (c) 2015 Next Audio Labs, LLC. All rights reserved.
//
//  This file contains confidential and proprietary information of Next
//  Audio Labs, LLC ("Next"). No use is permitted without express written
//  permission of Next. If you are not a party to a Confidentiality/
//  Non-Disclosure Agreement with Next, please immediately delete this
//  file as well as all copies in your possession. For further information,
//  please email info@nextaudiolabs.com.
//

#include "Persistence/Internal/PersistentContext.hpp"

using namespace NxA::Internal;

#pragma mark Constructors & Destructors

PersistentContext::PersistentContext(NxA::PersistentStore::Pointer s) :
    loadedObjects(PersistentObjectMap::map()), store(s)
{
#if NXA_DEBUG_OBJECT_LIFECYCLE
    printf("Construct Internal PersistentContext at 0x%08lx.\n", (long)this);
#endif
}

#pragma mark Instance Methods

NxA::PersistentObject::Pointer PersistentContext::createObjectWithType(uinteger32 type)
{
    auto result = store->createObjectWithType(type);
    loadedObjects->setValueForKey(result, result->objectID());
    return result;
}

NxA::PersistentObject::Pointer PersistentContext::loadObjectWithID(ObjectID id)
{
    if (loadedObjects->containsValueForKey(id))
    {
        return loadedObjects->valueForKey(id).pointer();
    }
    else
    {
        NxA::PersistentObject::Pointer result = store->loadObjectWithID(id);
        loadedObjects->setValueForKey(result, id);
        return result;
    }
}

void PersistentContext::saveContext()
{
    auto iter = loadedObjects->begin();
    while (iter != loadedObjects->end())
    {
        NxA::PersistentObject::Pointer obj = iter->second;

        if (obj->isDeleted())
        {
            store->deleteObject(obj);
            auto toDelete = iter;
            ++iter;
            loadedObjects->erase(toDelete);
        }
        else
        {
            store->saveObject(obj);
            ++iter;
        }
    }
}
