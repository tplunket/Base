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

#include "Persistence/PersistentObject.hpp"
#include "Persistence/Serializer.hpp"
#include "Persistence/ObjectID.hpp"
#include "Base/Base.hpp"
#include "Base/Internal/Object.hpp"

namespace NxA { namespace Internal {

    struct PersistentObject : public Object {
        NXA_GENERATED_INTERNAL_DECLARATIONS_FOR(NxA, PersistentObject);

        #pragma mark Instance Variables
        ObjectID objectID;
        boolean isDeleted;

        #pragma mark Instance Methods
        virtual boolean validate(void) const;
        virtual void serialize(NxA::Serializer& s);
    };
} }

