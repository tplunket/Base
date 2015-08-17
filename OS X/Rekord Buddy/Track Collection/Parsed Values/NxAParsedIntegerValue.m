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
#import "NxAParsedIntegerValue.h"
#import "Track Collection/Tags/NxATag.h"

#pragma mark Private Interface

@interface NxAParsedIntegerValue ()

@property (assign, nonatomic) BOOL p_isNil;

@property (assign, nonatomic) NSInteger p_value;

@end

#pragma mark Implementation

@implementation NxAParsedIntegerValue

#pragma mark Factory Methods

+ (instancetype)parsedValueWithNoInteger
{
    NxAParsedIntegerValue *newValue = [NxAParsedIntegerValue parsedValueWithInteger:0];

    newValue.p_isNil = YES;

    return newValue;
}

+ (instancetype)parsedValueWithInteger:(NSInteger)value
{
    NxAParsedIntegerValue *newValue = [[NxAParsedIntegerValue alloc] initPrivate];
    NxAAssertNotNil(newValue);

    newValue.p_value = value;
    newValue.p_isNil = NO;

    return newValue;
}

#pragma mark Overridden NxAParsedValue Accessors

- (NSString *)comparisonValue
{
    return self.p_value ? [NSString stringWithFormat:@"<%@>", [NSString stringWithFormat:@"%ld", self.p_value]] : @"nil";
}

#pragma mark Instance Methods

- (instancetype)initPrivate
{
    return [super initPrivate];
}

#pragma mark Overridden NxAParsedValue Instance Methods

- (NxATag *)tagForClass:(Class)tagClass
           usingContext:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(tagClass);
    NxAAssertNotNil(context);

    return [tagClass tagWithValue:(self.p_isNil ? nil : @(self.p_value)) context:context];
}

- (NxATag *)tagForClass:(Class)tagClass
               withName:(NSString *)name
           usingContext:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(tagClass);
    NxAAssertNotNil(name);
    NxAAssertNotNil(context);

    return [tagClass tagWithName:name value:(self.p_isNil ? nil : @(self.p_value)) context:context];
}

#pragma mark Overridden NSObject Instance Methods

- (instancetype)init
{
    ALog(@"Illegal base class call from '%@'.", self.class);
    return nil;
}

@end
