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

#import "NxAParsedCrateEntryValueTests.h"
#import "Track Collection/Parsed Values/Internal/NxAParsedCrateEntryValue.h"
#import "Track Collection/Tags/Crate Properties/NxACrateProperty.h"
#import "Track Collection/Crates/NxACrate.h"

#pragma mark Implementation

@implementation NxAParsedCrateEntryValueTests

#pragma mark Class Methods

+ (NxAParsedCrateEntryValue *)p_mockCrateEntryValueForCrate:(NxACrate *)crate
{
    NxAParsedCrateEntryValue *mockValue = mock([NxAParsedCrateEntryValue class]);

    [given([mockValue comparisonValue]) willReturn:[NxAParsedCrateEntryValue p_comparisonValueForCrateWithName:crate.name]];

    return mockValue;
}

+ (void)saveCrateEntryValuesForCrates:(NSArray *)crates
                              inCrate:(NxACrate *)crate
                   usingVersionPrefix:(NSString *)versionPrefix
{
    NSArray *parsedValues = nil;

    if (crates.count) {
        NSMutableArray *newValues = [NSMutableArray arrayWithCapacity:crates.count];
        for (NxACrate *childrenCrate in crates) {
            NxAParsedCrateEntryValue *mockValue = [NxAParsedCrateEntryValueTests p_mockCrateEntryValueForCrate:childrenCrate];
            [newValues addObject:mockValue];
        }

        parsedValues = newValues;
    }
    else {
        parsedValues = @[ [NxAParsedCrateEntryValue parsedValueWithNoCrateEntry] ];
    }


    [crate saveValues:parsedValues forTagClass:[NxACrateProperty class] andVersion:versionPrefix];
}

@end
