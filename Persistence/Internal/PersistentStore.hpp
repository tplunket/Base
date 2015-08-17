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

#include "Persistence/PersistentStore.hpp"
#include "Persistence/PersistentObjectSchema.hpp"
#include "Base/Base.hpp"
#include "Base/Internal/Object.hpp"

#include <SQLiteCpp/SQLiteCpp.h>

namespace NxA { namespace Internal {
    struct PersistentStore : public Object {
        NXA_GENERATED_INTERNAL_DECLARATIONS_WITHOUT_CONSTRUCTORS_FOR(NxA, PersistentStore);

        using AutoIncrementMap = Map<uinteger32, uinteger32>;

        #pragma mark Constructors & Destructors
        PersistentStore(const character* filename);

        #pragma mark Instance Variables
        AutoIncrementMap::Pointer autoIncrementMap;
        SQLite::Database db;

        #pragma mark Instance Methods

        void beginTransaction();
        void endTransaction();

        NxA::PersistentObject::Pointer loadObjectWithID(const ObjectID& id);
        NxA::PersistentObject::Pointer createObjectWithType(uinteger32 type);
        void saveObject(NxA::PersistentObject::Pointer obj);
        void deleteObject(NxA::PersistentObject::Pointer obj);

        ObjectID createObjectIDForType(uinteger32 type);
    };
} }
