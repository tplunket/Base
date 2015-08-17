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

#include "Persistence/Internal/PersistentObjectSchema.hpp"
#include "Persistence/Internal/SQLiteSerializers.hpp"

using namespace NxA::Internal;

#pragma mark Static Variables

NxA::PersistentObjectSchema::Map::Pointer PersistentObjectSchema::schemas = NxA::PersistentObjectSchema::Map::map();

#pragma mark Static Methods

void PersistentObjectSchema::registerSchema(NxA::PersistentObjectSchema::Pointer schema)
{
    schemas->setValueForKey(schema, schema->identifier());
}

NxA::PersistentObjectSchema& PersistentObjectSchema::getSchemaForType(uinteger32 type)
{
    return schemas->valueForKey(type);
}

#pragma mark Utility methods

static NxA::String::Pointer concatenateParams(NxA::String::Array::Pointer array,
                                              const NxA::character* prefix,
                                              const NxA::character* suffix,
                                              const NxA::character* separator)
{
    NxA::String::Pointer str = NxA::String::string();
    for (int i = 0; i < array->length(); ++i)
    {
        str->append(prefix);
        str->append((*array)[i]);
        str->append(suffix);

        if (i != array->length() - 1)
            str->append(separator);
    }

    return str;
}

static NxA::String::Pointer createParams(NxA::String::Array::Pointer array)
{
    NxA::String::Pointer result = NxA::String::stringWith("`id` PRIMARY KEY");
    if (array->length() > 0)
    {
        result->append(", ");
        result->append(concatenateParams(array, "`", "`", ", "));
    }
    return result;
}

static NxA::String::Pointer updateParams(NxA::String::Array::Pointer array)
{
    return concatenateParams(array, "`", "`=?", ", ");
}

static NxA::String::Pointer insertFields(NxA::String::Array::Pointer array)
{
    NxA::String::Pointer result = NxA::String::stringWith("`id`");
    if (array->length() > 0)
    {
        result->append(", ");
        result->append(concatenateParams(array, "`", "`", ", "));
    }
    return result;
}

static NxA::String::Pointer insertArguments(NxA::String::Array::Pointer array)
{
    NxA::String::Pointer result = NxA::String::stringWith("?");
    for (int i = 0; i < array->length(); ++i)
    {
        result->append(", ?");
    }
    return result;
}

#pragma mark Constructors

PersistentObjectSchema::PersistentObjectSchema(uinteger32 classHash,
                                               const NxA::character* className,
                                               NxA::PersistentObject::Pointer obj,
                                               CreateObjectFunc func) :
    identifier(classHash),
    tableName(String::stringWith(className)),
    createQuery(String::string()),
    selectQuery(String::string()),
    insertQuery(String::string()),
    updateQuery(String::string()),
    deleteQuery(String::string()),
    createMethod(func)
{
#if NXA_DEBUG_OBJECT_LIFECYCLE
    printf("Construct Internal PersistentObjectSchema at 0x%08lx.\n", (long)this);
#endif

    GetFieldNamesSerializer fieldNameSerializer;
    obj->serialize(fieldNameSerializer);
    fieldCount = (NxA::uinteger32)fieldNameSerializer.getFieldNames()->length();

    NxA::String::Array::Pointer fieldNames = fieldNameSerializer.getFieldNames();

    createQuery = NxA::String::stringWithFormat("CREATE TABLE IF NOT EXISTS `%s` (%s);",
                                                tableName->toUTF8(), 
                                                createParams(fieldNames)->toUTF8());

    selectQuery = NxA::String::stringWithFormat("SELECT * FROM `%s` WHERE id=?;", tableName->toUTF8());
    deleteQuery = NxA::String::stringWithFormat("DELETE FROM `%s` WHERE id=?;", tableName->toUTF8());

    insertQuery = NxA::String::stringWithFormat("INSERT INTO `%s` (%s) VALUES (%s);",
                                                tableName->toUTF8(),
                                                insertFields(fieldNames)->toUTF8(),
                                                insertArguments(fieldNames)->toUTF8());


    updateQuery = NxA::String::stringWithFormat("UPDATE `%s` SET %s WHERE `id`=?;",
                                                tableName->toUTF8(), 
                                                updateParams(fieldNames)->toUTF8());

}

