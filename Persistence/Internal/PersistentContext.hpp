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

#include "Persistence/PersistentContext.hpp"
#include "Persistence/PersistentStore.hpp"
#include "Base/Base.hpp"
#include "Base/Internal/Object.hpp"
#include "Base/Map.hpp"

namespace NxA { namespace Internal {
    struct PersistentContext : public Object {
        NXA_GENERATED_INTERNAL_DECLARATIONS_WITHOUT_CONSTRUCTORS_FOR(NxA, PersistentContext);

        using PersistentObjectMap = Map<ObjectID, NxA::PersistentObject>;

        #pragma mark Constructors & Destructors
        PersistentContext(NxA::PersistentStore::Pointer store);

        #pragma mark Instance Variables
        PersistentObjectMap::Pointer loadedObjects;
        NxA::PersistentStore::Pointer store;

        #pragma mark Instance Methods
        NxA::PersistentObject::Pointer loadObjectWithID(ObjectID id);
        NxA::PersistentObject::Pointer createObjectWithType(uinteger32 type);
        void deleteObject(NxA::PersistentObject::Pointer obj);
        void saveContext(void);
    };
} }
