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

#include "Persistence/PersistentObjectSchema.hpp"
#include "Base/Base.hpp"
#include "Base/Internal/Object.hpp"

namespace NxA { namespace Internal {
    struct PersistentObjectSchema : public Object {
        NXA_GENERATED_INTERNAL_DECLARATIONS_WITHOUT_CONSTRUCTORS_FOR(NxA, PersistentObjectSchema);

        static NxA::PersistentObjectSchema::Map::Pointer schemas;

        #pragma mark Constructors
        PersistentObjectSchema(uinteger32 classHash,
                               const character* className,
                               NxA::PersistentObject::Pointer obj,
                               NxA::CreateObjectFunc func);

        #pragma mark Static Methods
        static void registerSchema(NxA::PersistentObjectSchema::Pointer schema);
        static NxA::PersistentObjectSchema& getSchemaForType(uinteger32 type);

        #pragma mark Internal Members

        uinteger32 identifier;
        String::Pointer tableName;

        uinteger32 fieldCount;
        String::Pointer createQuery;
        String::Pointer selectQuery;
        String::Pointer insertQuery;
        String::Pointer updateQuery;
        String::Pointer deleteQuery;

        NxA::CreateObjectFunc createMethod;
    };
} }
