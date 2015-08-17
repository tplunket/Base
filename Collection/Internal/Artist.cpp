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

#include "Collection/Internal/Artist.hpp"

using namespace NxA::Internal;

#pragma mark Constructors & Destructors

Artist::Artist() : name(NxA::String::string())
{
#if NXA_DEBUG_OBJECT_LIFECYCLE
    printf("Construct Internal Artist at 0x%08lx.\n", (long)this);
#endif
}

void Artist::serialize(NxA::Serializer& s) {
    s.serialize(NXA_SERIALIZED_MEMBER(name));

    PersistentObject::serialize(s);
}
