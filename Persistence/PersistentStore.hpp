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

#include "Base/Base.hpp"
#include "Persistence/PersistentObject.hpp"
#include "Persistence/PersistentObjectSchema.hpp"

namespace NxA {
    NXA_EXCEPTION_NAMED_WITH_PARENT(RecordNotFoundError, NxA::Exception);
    NXA_EXCEPTION_NAMED_WITH_PARENT(PersistentObjectValidationError, NxA::Exception);

    NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(PersistentStore);
    
    class PersistentStore : public Object {
        NXA_GENERATED_OPERATOR_EQUAL_DECLARATION_IN_NAMESPACE_FOR_CLASS(NxA, PersistentStore);
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(NxA, PersistentStore);

    public:
        
        #pragma mark Factory Methods
        static PersistentStore::Pointer persistentStore(const character* filename);

        #pragma mark Instance Methods
        void beginTransaction();
        void endTransaction();

        PersistentObject::Pointer loadObjectWithID(const ObjectID& id);
        PersistentObject::Pointer createObjectWithType(uinteger32 type);
        void saveObject(PersistentObject::Pointer obj);
        void deleteObject(PersistentObject::Pointer obj);
   };
}
