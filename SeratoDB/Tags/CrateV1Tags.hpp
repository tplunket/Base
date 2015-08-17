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

#include <Base/Base.hpp>

#pragma mark Constants

namespace NxA { namespace Serato {
    // -- Known crate tag identifiers.
    constexpr uinteger32 crateVersionTagIdentifier                      = 'vrsn';
    constexpr uinteger32 trackEntryTagIdentifier                        = 'otrk';
    constexpr uinteger32 trackEntryPathTagIdentifier                    = 'ptrk';
    constexpr uinteger32 crateRemoveTrackActionTagIdentifier            = 'ortk';
    constexpr uinteger32 crateColumnSortingTagIdentifier                = 'osrt';
    constexpr uinteger32 crateColumnNameTag                             = 'tvcn';
    constexpr uinteger32 crateColumnSortDirectionTagIdentifier          = 'brev';
    constexpr uinteger32 cateColumnVisibilityTagIdentifier              = 'ovct';
    constexpr uinteger32 crateColumnWidthTagIdentifier                  = 'tvcw';
    constexpr uinteger32 crateRemoveColumnVisibilityActionTagIdentifier = 'orvc';
} }
