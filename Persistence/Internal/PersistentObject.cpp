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

#include "Persistence/Internal/PersistentObject.hpp"

using namespace NxA::Internal;

#pragma mark Constructors & Destructors

PersistentObject::PersistentObject() : isDeleted(false)
{
#if NXA_DEBUG_OBJECT_LIFECYCLE
    printf("Construct Internal PersistentObject at 0x%08lx.\n", (long)this);
#endif
}

#pragma mark Instance Methods

NxA::boolean PersistentObject::validate(void) const
{
    return true;
}

void PersistentObject::serialize(NxA::Serializer& s)
{
}
