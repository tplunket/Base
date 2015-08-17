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

#include "Persistence/Internal/SQLiteSerializers.hpp"

using namespace NxA::Internal;
using namespace NxA;

#pragma mark PopulateFromDBSerializer

PopulateFromDBSerializer::PopulateFromDBSerializer(SQLite::Statement& s, uinteger32 startIndex) :
    statement(s), columnIndex(startIndex)
{
}
void PopulateFromDBSerializer::serializeField(const character* name, integer32& value)
{
    value = statement.getColumn(columnIndex);
    ++columnIndex;
}
void PopulateFromDBSerializer::serializeField(const character* name, String::Pointer& value)
{
    value = String::stringWith(statement.getColumn(columnIndex));
    ++columnIndex;
}

#pragma mark BindToQuerySerializer

BindToQuerySerializer::BindToQuerySerializer(SQLite::Statement& s, uinteger32 startIndex) :
    statement(s), columnIndex(startIndex)
{
}
void BindToQuerySerializer::serializeField(const character* name, integer32& value)
{
    statement.bind(columnIndex, value);
    ++columnIndex;
}
void BindToQuerySerializer::serializeField(const character* name, String::Pointer& value)
{
    statement.bind(columnIndex, value->toUTF8());
    ++columnIndex;
}

#pragma mark GetFieldNamesSerializer

GetFieldNamesSerializer::GetFieldNamesSerializer() : fieldNames(String::Array::array())
{
}
void GetFieldNamesSerializer::serializeField(const character* name, integer32& value)
{
    saveFieldName(name);
}
void GetFieldNamesSerializer::serializeField(const character* name, String::Pointer& value)
{
    saveFieldName(name);
}
void GetFieldNamesSerializer::saveFieldName(const character* name)
{
    fieldNames->append(String::stringWith(name));
}
String::Array::Pointer GetFieldNamesSerializer::getFieldNames(void)
{
    return fieldNames;
}

