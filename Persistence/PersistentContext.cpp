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

#include "Persistence/PersistentContext.hpp"
#include "Persistence/Internal/PersistentContext.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA, PersistentContext, Object);

using namespace NxA;

#pragma mark Factory Methods

PersistentContext::Pointer PersistentContext::persistentContext(PersistentStore::Pointer store)
{
    auto internal = Internal::PersistentContext::Pointer(std::make_shared<NxA::Internal::PersistentContext>(store));
    auto internalPtr = NxA::Internal::Object::Pointer::dynamicCastFrom(internal);
    auto context = PersistentContext::makeSharedWithInternal(internalPtr);
    return context;
}

#pragma mark Instance Methods

PersistentObject::Pointer PersistentContext::createObjectWithType(uinteger32 type)
{
    return internal->createObjectWithType(type);
}
PersistentObject::Pointer PersistentContext::loadObjectWithID(const ObjectID& id)
{
    return internal->loadObjectWithID(id);
}
void PersistentContext::saveContext(void)
{
    internal->saveContext();
}
PersistentStore::Pointer PersistentContext::store(void)
{
    return internal->store;
}

