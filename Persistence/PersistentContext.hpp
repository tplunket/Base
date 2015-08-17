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
#include "Persistence/PersistentStore.hpp"

namespace NxA {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(PersistentContext);
    
    class PersistentContext : public Object {
        NXA_GENERATED_OPERATOR_EQUAL_DECLARATION_IN_NAMESPACE_FOR_CLASS(NxA, PersistentContext);
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(NxA, PersistentContext);

    protected:

        PersistentObject::Pointer createObjectWithType(uinteger32 type);

    public:

        #pragma mark Factory Methods
        static PersistentContext::Pointer persistentContext(PersistentStore::Pointer store);

        #pragma mark Instance Methods
        PersistentObject::Pointer loadObjectWithID(const ObjectID& id);
        void saveContext(void);
        PersistentStore::Pointer store(void);

        template<typename T> typename T::Pointer createObject(void) {
            auto obj = createObjectWithType(T::hashOfClassName());
            return T::Pointer::dynamicCastFrom(obj);
        }
    };
}
