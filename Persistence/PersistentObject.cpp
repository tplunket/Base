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

#include "Persistence/PersistentObject.hpp"
#include "Persistence/Internal/PersistentObject.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA, PersistentObject, Object);

using namespace NxA;

#pragma mark Factory Methods

PersistentObject::Pointer PersistentObject::persistentObject(const ObjectID& id)
{
    auto obj = PersistentObject::makeShared();
    obj->internal->objectID = id;
    return obj;
}

#pragma mark Protected Methods

void PersistentObject::setObjectID(const ObjectID& id)
{
    internal->objectID = id;
}

#pragma mark Instance Methods

String::Pointer PersistentObject::description(void) const
{
    return String::stringWithFormat("PersistentObject with ID %s",
                                    internal->objectID.description()->toUTF8());
}

const ObjectID& PersistentObject::objectID(void) const
{
    return internal->objectID;
}

boolean PersistentObject::validate(void) const
{
    if (isDeleted())
    {
        return false;
    }

    return internal->validate();
}

boolean PersistentObject::isDeleted(void) const
{
    return internal->isDeleted;
}

void PersistentObject::deleteFromContext(void)
{
    internal->isDeleted = true;
}

void PersistentObject::serialize(Serializer& s)
{
    internal->serialize(s);
}

