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

#include "Collection/Artist.hpp"
#include "Persistence/Internal/PersistentObject.hpp"
#include "Base/Base.hpp"

namespace NxA { namespace Internal {
    struct Artist : public PersistentObject {
        NXA_GENERATED_INTERNAL_DECLARATIONS_FOR(NxA, Artist);

        #pragma mark Instance Variables
        NxA::String::Pointer name;
        
        virtual void serialize(NxA::Serializer& s);
    };
} }
