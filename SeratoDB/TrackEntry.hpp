//
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

#pragma once

#include <Tags/ObjectTag.hpp>

#include <Base/Base.hpp>

namespace NxA { namespace Serato {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(TrackEntry);

    class Tag;

    class TrackEntry : public Object {
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(NxA::Serato, TrackEntry);

    public:
        #pragma mark Factory Methods
        static TrackEntry::Pointer entryWithTagOnVolume(const ObjectTag& tag, const String& volumePath);
        static TrackEntry::Pointer entryWithTrackFileAtOnVolume(const String& path, const String& volumePath);

        #pragma mark Instance Methods
        String::Pointer trackFilePath(void) const;

        const Tag& tagForEntry(void) const;

        void destroy(void);
    };
} }
