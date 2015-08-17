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

#import "Track Collection/Parsed Values/Internal/NxAParsedValue.h"
#import "NxAParsedStringValue.h"
#import "Track Collection/Tags/NxATag.h"

#pragma mark Private Interface

@interface NxAParsedStringValue ()

@property (strong, nonatomic) NSString *p_value;

@end

#pragma mark Implementation

@implementation NxAParsedStringValue

#pragma mark Factory Methods

+ (instancetype)parsedValueWithNoString
{
    NxAParsedStringValue *newValue = [[NxAParsedStringValue alloc] initPrivate];

    newValue.p_value = nil;

    return newValue;
}

+ (instancetype)parsedValueWithString:(NSString *)value
{
    NxAAssertNotNil(value);

    NxAParsedStringValue *newValue = [[NxAParsedStringValue alloc] initPrivate];

    newValue.p_value = value;

    return newValue;
}

#pragma mark Overridden NxAParsedValue Accessors

- (NSString *)comparisonValue
{
    return self.p_value ? [NSString stringWithFormat:@"<%@>", self.p_value] : @"nil";
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
    NxAAssertNotNil(tagClass);
    NxAAssertNotNil(context);

    return [tagClass tagWithValue:self.p_value context:context];
}

- (NxATag *)tagForClass:(Class)tagClass
               withName:(NSString *)name
           usingContext:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(tagClass);
    NxAAssertNotNil(name);
    NxAAssertNotNil(context);

    return [tagClass tagWithName:name value:self.p_value context:context];
}

#pragma mark Overridden NSObject Instance Methods

- (instancetype)init
{
    ALog(@"Illegal base class call from '%@'.", self.class);
    return nil;
}

@end
