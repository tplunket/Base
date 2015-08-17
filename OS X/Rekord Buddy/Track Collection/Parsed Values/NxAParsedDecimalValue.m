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
#import "NxAParsedDecimalValue.h"
#import "Track Collection/Tags/NxATag.h"

#pragma mark Private Interface

@interface NxAParsedDecimalValue ()

@property (strong, nonatomic) NSDecimalNumber *p_value;

@end

#pragma mark Implementation

@implementation NxAParsedDecimalValue

#pragma mark Factory Methods

+ (instancetype)parsedValueWithNoDecimalNumber
{
    NxAParsedDecimalValue *newValue = [[NxAParsedDecimalValue alloc] initPrivate];

    newValue.p_value = nil;

    return newValue;
}

+ (instancetype)parsedValueWithDecimalNumber:(NSDecimalNumber *)value
{
    NxAAssertNotNil(value);

    NxAParsedDecimalValue *newValue = [[NxAParsedDecimalValue alloc] initPrivate];

    newValue.p_value = value;

    return newValue;
}

#pragma mark Overridden NxAParsedValue Accessors

- (NSString *)comparisonValue
{
    return self.p_value ? [NSString stringWithFormat:@"<%@>", self.p_value.stringValue] : @"nil";
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
