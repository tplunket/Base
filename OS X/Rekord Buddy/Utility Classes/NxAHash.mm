//
//  Copyright (c) 2013-2015 Next Audio Labs, LLC. All rights reserved.
//
//  This file contains confidential and proprietary information of Next
//  Audio Labs, LLC ("Next"). No use is permitted without express written
//  permission of Next. If you are not a party to a Confidentiality/
//  Non-Disclosure Agreement with Next, please immediately delete this
//  file as well as all copies in your possession. For further information,
//  please email info@nextaudiolabs.com.
//

#import "NxAHash.h"

#include "Base/Blob.hpp"
#include "Base/Types.hpp"

#pragma mark Implementation

@implementation NxAHash

#pragma mark Class Methods

+ (NSString *)hashFor:(NSData *)data
{
    auto hashBlob = NxA::Blob::hashFor(reinterpret_cast<const NxA::byte*>(data.bytes), static_cast<int>(data.length));
    NxA::byte* hash = hashBlob->data();

    return [NSString stringWithFormat:@"%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx",
            hash[0], hash[1], hash[2], hash[3], hash[4], hash[5], hash[6], hash[7],
            hash[8], hash[9], hash[10], hash[11], hash[12], hash[13], hash[14], hash[15]];
}

@end
