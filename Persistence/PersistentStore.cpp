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

#include "Persistence/PersistentStore.hpp"
#include "Persistence/Internal/PersistentStore.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA, PersistentStore, Object);

using namespace NxA;

#pragma mark Factory Methods

PersistentStore::Pointer PersistentStore::persistentStore(const character* filename)
{
    auto internal = Internal::PersistentStore::Pointer(std::make_shared<NxA::Internal::PersistentStore>(filename));
    auto internalPtr = NxA::Internal::Object::Pointer::dynamicCastFrom(internal);
    auto context = PersistentStore::makeSharedWithInternal(internalPtr);
    return context;
}

#pragma mark Instance Methods

void PersistentStore::beginTransaction()
{
    internal->beginTransaction();
}
void PersistentStore::endTransaction()
{
    internal->endTransaction();
}
PersistentObject::Pointer PersistentStore::loadObjectWithID(const ObjectID& id)
{
    return internal->loadObjectWithID(id);
}
PersistentObject::Pointer PersistentStore::createObjectWithType(uinteger32 type)
{
    return internal->createObjectWithType(type);
}
void PersistentStore::saveObject(PersistentObject::Pointer obj)
{
    internal->saveObject(obj);
}
void PersistentStore::deleteObject(PersistentObject::Pointer obj)
{
    internal->deleteObject(obj);
}


