//
//  NxASeratoCrateTrack.cpp
//  SeratoDB
//
//  Created by Didier Malenfant on 2/5/15.
//  Copyright (c) 2015 Next Audio Labs, LLC. All rights reserved.
//
//  This file contains confidential and proprietary information of Serato
//  Inc. LLP ("Serato"). No use is permitted without express written
//  permission of Serato. If you are not a party to a Confidentiality/
//  Non-Disclosure Agreement with Serato, please immediately delete this
//  file as well as all copies in your possession. For further information,
//  please refer to the modified MIT license provided with this library,
//  or email licensing@serato.com.
//

#include "SeratoDB/NxASeratoCrateTrack.h"
#include "SeratoDB/NxASeratoCrateV1Tags.h"

using namespace NxA;

#pragma mark Instance Methods

bool SeratoCrateTrack::p_containsAValidTag(void) const
{
    return this->p_trackTag.get() != NULL;
}

StringAutoPtr SeratoCrateTrack::trackFilePath(void) const
{
    if (this->p_containsAValidTag()) {
        const SeratoTag* tag = this->p_trackTag->subTagWithIdentifierOrNilIfDoesNotExist(NxASeratoCrateTrackPathTag);
        if (tag != NULL) {
            return tag->dataAsPath();
        }
    }

    return StringAutoPtr(new std::string(""));
}
