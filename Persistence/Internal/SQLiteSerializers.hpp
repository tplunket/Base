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

#pragma once

#include <SQLiteCpp/SQLiteCpp.h>

#include "Base/Base.hpp"
#include "Persistence/Serializer.hpp"

namespace NxA { namespace Internal {
    class PopulateFromDBSerializer : public Serializer {
    private:
        #pragma mark Internal Members
        SQLite::Statement& statement;
        uinteger32 columnIndex;

    public:
        #pragma mark Constructors
        PopulateFromDBSerializer(SQLite::Statement& s, uinteger32 startIndex = 1);

        #pragma mark Instance Methods
        void serializeField(const character* name, integer32& value);
        void serializeField(const character* name, String::Pointer& value);
    };

    class BindToQuerySerializer : public Serializer {
    private:
        #pragma mark Internal Members
        SQLite::Statement& statement;
        uinteger32 columnIndex;

    public:
        #pragma mark Constructors
        BindToQuerySerializer(SQLite::Statement& s, uinteger32 startIndex = 1);

        #pragma mark Instance Methods
        void serializeField(const character* name, integer32& value);
        void serializeField(const character* name, String::Pointer& value);
    };

    class GetFieldNamesSerializer : public Serializer {
    private:
        #pragma mark Internal Members
        String::Array::Pointer fieldNames;

    public:
        #pragma mark Constructors
        GetFieldNamesSerializer();

        #pragma mark Instance Methods
        void serializeField(const character* name, integer32& value);
        void serializeField(const character* name, String::Pointer& value);
        void saveFieldName(const character* name);
        String::Array::Pointer getFieldNames(void);
    };
} }
