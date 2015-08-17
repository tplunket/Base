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
#include "Persistence/Persistence.hpp"

namespace NxA {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(Artist);

    class Artist final : public PersistentObject {
        NXA_GENERATED_OPERATOR_EQUAL_DECLARATION_IN_NAMESPACE_FOR_CLASS(NxA, Artist);
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(NxA, Artist);

    public:
        #pragma mark Factory Methods
        static Artist::Pointer artistWithNameInDb(String& name,
                                                  void* db);

        static Artist::Pointer existingArtistWithNameInDb(String& name,
                                                          void* db);

        #pragma mark Instance Methods
        NxA::Pointer<NxA::String> description(void) const;

        const String& name(void) const;
    };
}
