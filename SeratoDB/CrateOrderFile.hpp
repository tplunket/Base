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

#include <SeratoDB/TrackEntry.hpp>

#include <Base/Base.hpp>

namespace NxA { namespace Serato {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR(CrateOrderFile);

    class Crate;

    class CrateOrderFile : public Object {
        NXA_GENERATED_DECLARATIONS_FOR(NxA::Serato, CrateOrderFile);

    public:
        #pragma mark Factory Methods
        static CrateOrderFile::Pointer fileWithSeratoFolderInRootFolder(const String& seratoFolderPath,
                                                                        const String& rootFolderPath);

        #pragma mark Instance Methods
        Crate& rootCrate(void);

        timestamp modificationDateInSecondsSince1970(void) const;

        void saveIfModified(void) const;
    };
} }
