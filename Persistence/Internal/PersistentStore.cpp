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

#include "Persistence/Internal/PersistentStore.hpp"
#include "Persistence/Internal/SQLiteSerializers.hpp"
#include "Persistence/ObjectID.hpp"

using namespace NxA::Internal;

#pragma mark Constructors & Destructors

PersistentStore::PersistentStore(const character* filename) :
    autoIncrementMap(AutoIncrementMap::map()),
    db(filename, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE)
{
#if NXA_DEBUG_OBJECT_LIFECYCLE
    printf("Construct Internal PersistentStore at 0x%08lx.\n", (long)this);
#endif

    auto schemas = NxA::PersistentObjectSchema::getSchemaMap();
    for (auto it = schemas->begin(); it != schemas->end(); ++it)
    {
        auto schema = it->second;
        SQLite::Statement statement(db, schema->createQuery()->toUTF8());
        statement.exec();
    }
}

#pragma mark Instance Methods

void PersistentStore::beginTransaction()
{
}
void PersistentStore::endTransaction()
{
}

NxA::PersistentObject::Pointer PersistentStore::loadObjectWithID(const NxA::ObjectID& id)
{
    const NxA::PersistentObjectSchema& schema = NxA::PersistentObjectSchema::getSchemaForType(id.type);
    SQLite::Statement statement(db, schema.selectQuery()->toUTF8());

    statement.bind(1, id.index);

    if (statement.executeStep() == false) {
        throw new RecordNotFoundError("Unable to find record in database");
    }

    auto obj = schema.createObject(id);
    PopulateFromDBSerializer s(statement);
    obj->serialize(s);

    return obj;
}

NxA::PersistentObject::Pointer PersistentStore::createObjectWithType(uinteger32 type)
{
    const NxA::PersistentObjectSchema& schema = NxA::PersistentObjectSchema::getSchemaForType(type);
    auto obj = schema.createObject(createObjectIDForType(type));
    return obj;
}

void PersistentStore::saveObject(NxA::PersistentObject::Pointer obj)
{
    if (obj->validate() == false)
    {
        throw PersistentObjectValidationError::exceptionWith("Validation failed for object of type '%s'",
                                                             obj->className());
    }

    const NxA::PersistentObjectSchema& schema = NxA::PersistentObjectSchema::getSchemaForType(obj->objectID().type);

    SQLite::Statement deleteStatement(db, schema.deleteQuery()->toUTF8());
    deleteStatement.bind(1, obj->objectID().index);
    deleteStatement.exec();
    
    SQLite::Statement insertStatement(db, schema.insertQuery()->toUTF8());
    insertStatement.bind(1, obj->objectID().index);
    BindToQuerySerializer s(insertStatement, 2);
    obj->serialize(s);
    insertStatement.exec();
}

void PersistentStore::deleteObject(NxA::PersistentObject::Pointer obj)
{
    const NxA::PersistentObjectSchema& schema = NxA::PersistentObjectSchema::getSchemaForType(obj->objectID().type);
    SQLite::Statement statement(db, schema.deleteQuery()->toUTF8());
    statement.bind(1, obj->objectID().index);
    statement.exec();
}

NxA::ObjectID PersistentStore::createObjectIDForType(uinteger32 type)
{
    uinteger32 index = 1;
    if (autoIncrementMap->containsValueForKey(type))
    {
        index = autoIncrementMap->valueForKey(type);
    }

    autoIncrementMap->setValueForKey(index + 1, type);
    return ObjectID(type, index);
}


