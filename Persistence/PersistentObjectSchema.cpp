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

#include "Persistence/PersistentObjectSchema.hpp"
#include "Persistence/Internal/PersistentObjectSchema.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA, PersistentObjectSchema, Object);

using namespace NxA;

#pragma mark Factory Methods

PersistentObjectSchema::Pointer PersistentObjectSchema::schema(uinteger32 classHash,
                                                               const character* className,
                                                               PersistentObject::Pointer obj,
                                                               CreateObjectFunc func)
{
    auto internal = Internal::PersistentObjectSchema::Pointer(
        std::make_shared<NxA::Internal::PersistentObjectSchema>(classHash, className, obj, func));
    auto internalPtr = NxA::Internal::Object::Pointer::dynamicCastFrom(internal);
    return PersistentObjectSchema::makeSharedWithInternal(internalPtr);
}

#pragma mark Static Methods

void PersistentObjectSchema::registerSchema(NxA::PersistentObjectSchema::Pointer schema)
{
    Internal::PersistentObjectSchema::registerSchema(schema);
}

PersistentObjectSchema& PersistentObjectSchema::getSchemaForType(uinteger32 type)
{
    return Internal::PersistentObjectSchema::getSchemaForType(type);
}

PersistentObjectSchema::Map::Pointer PersistentObjectSchema::getSchemaMap()
{
    return Internal::PersistentObjectSchema::schemas;
}

#pragma mark Instance Methods

uinteger32 PersistentObjectSchema::identifier(void) const
{
    return internal->identifier;
}
String::Pointer PersistentObjectSchema::tableName(void) const
{
    return internal->tableName;
}
String::Pointer PersistentObjectSchema::createQuery(void) const
{
    return internal->createQuery;
}
String::Pointer PersistentObjectSchema::selectQuery(void) const
{
    return internal->selectQuery;
}
String::Pointer PersistentObjectSchema::insertQuery(void) const
{
    return internal->insertQuery;
}
String::Pointer PersistentObjectSchema::updateQuery(void) const
{
    return internal->updateQuery;
}
String::Pointer PersistentObjectSchema::deleteQuery(void) const
{
    return internal->deleteQuery;
}
uinteger32 PersistentObjectSchema::fieldCount(void) const
{
    return internal->fieldCount;
}
PersistentObject::Pointer PersistentObjectSchema::createObject(const ObjectID& id) const
{
    return internal->createMethod(id);
}

