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

#include "Collection/Artist.hpp"
#include "Collection/Internal/Artist.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA, Artist, PersistentObject);

using namespace NxA;

#pragma mark Factory Methods

Artist::Pointer Artist::artistWithNameInDb(String& name, void* db)
{
    auto newArtist = Artist::makeShared();
    newArtist->internal->name = name.pointer();

    return newArtist;
}

Artist::Pointer Artist::existingArtistWithNameInDb(String& name, void* db)
{
    auto newArtist = Artist::makeShared();
    newArtist->internal->name = name.pointer();

    return newArtist;
}

#pragma mark Instance Methods

String::Pointer Artist::description(void) const
{
    return String::stringWithFormat("Artist with name '%s' id %d", this->name().toUTF8(), internal->objectID);
}

const String& Artist::name(void) const
{
    return internal->name;
}
