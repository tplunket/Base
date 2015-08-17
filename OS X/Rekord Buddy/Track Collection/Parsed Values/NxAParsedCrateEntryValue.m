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

#import "Track Collection/Parsed Values/Internal/NxAParsedCrateEntryValue.h"
#import "Track Collection/Tags/Crate Properties/NxACrateProperty.h"
#import "Track Collection/Crates/NxACrate.h"
#import "NSDate+NxAUtility.h"

#pragma mark Implementation

@implementation NxAParsedCrateEntryValue

#pragma mark Factory Methods

+ (instancetype)parsedValueWithNoCrateEntry
{
    NxAParsedCrateEntryValue *newValue = [[NxAParsedCrateEntryValue alloc] initPrivate];

    newValue.p_source = nil;

    return newValue;
}

+ (instancetype)parsedValueWithSourceCrateEntry:(id)source
                             whichWasModifiedOn:(NSDate *)dateModified
                                      hasParent:(NxACrate *)parentCrate
                             andCanBeParsedWith:(NxAParsingOperation *)operation

{
    NxAAssertNotNil(source);
    NxAAssertNotNil(operation);
    NxAAssertNotNil(dateModified);
    NxAAssertNotNil(parentCrate);

    NxAParsedCrateEntryValue *newValue = [[NxAParsedCrateEntryValue alloc] initPrivate];

    newValue.p_source = source;
    newValue.p_operation = operation;
    newValue.p_dateModified = [dateModified roundedDownToTheNearestSecond];
    newValue.p_parentCrate = parentCrate;

    return newValue;
}

#pragma mark Class Methods

+ (NSString *)p_comparisonValueForCrateWithName:(NSString *)name
{
    return name ? [NSString stringWithFormat:@"<%@>", name] : @"nil";
}

#pragma mark Overridden NxAParsedValue Accessors

- (NSString *)comparisonValue
{
    NSString *name = [self.p_operation.implementation nameOfFolderEntrySource:self.p_source];
    return [NxAParsedCrateEntryValue p_comparisonValueForCrateWithName:name];
}

#pragma mark Instance Methods

- (instancetype)initPrivate
{
    return [super initPrivate];
}

#pragma mark Overridden NxAParsedValue Instance Methods

- (NxATag *)tagForClass:(Class)tagClass
           usingContext:(NSManagedObjectContext *)context;
{
    NxAAssertTrue(tagClass == [NxACrateProperty class]);
    NxAAssertNotNil(context);

    if (self.p_source) {
        if (![self.p_operation hasAlreadyParsedSource:self.p_source]) {
            [self.p_operation parseSourceFolderEntry:self.p_source
                                      forParentCrate:self.p_parentCrate
                andReplaceContentIfDifferentThanPreviouslyParsedAndOlderThan:self.p_dateModified];
        }

        NxATaggedObject *object = [self.p_operation objectForSource:self.p_source];
        NxAAssertTrue([object isKindOfClass:[NxACrate class]]);

        return [tagClass tagWithValue:object context:context];
    }
    else {
        return [tagClass tagWithValue:nil context:context];
    }
}

#pragma mark Overridden NSObject Instance Methods

- (instancetype)init
{
    ALog(@"Illegal base class call from '%@'.", self.class);
    return nil;
}

@end
