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
#include "Persistence/ObjectID.hpp"
#include "Persistence/Serializer.hpp"

namespace NxA {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(PersistentObject);

    class PersistentObject : public Object {
        NXA_GENERATED_OPERATOR_EQUAL_DECLARATION_IN_NAMESPACE_FOR_CLASS(NxA, PersistentObject);
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(NxA, PersistentObject);

    protected:
        void setObjectID(const ObjectID& id);

    public:
        #pragma mark Factory Methods
        static PersistentObject::Pointer persistentObject(const ObjectID& id);

        #pragma mark Instance Methods
        virtual String::Pointer description(void) const;

        const ObjectID& objectID(void) const;
        boolean validate(void) const;
        boolean isDeleted(void) const;

        void serialize(Serializer& s);

        void deleteFromContext(void);
    };
}
