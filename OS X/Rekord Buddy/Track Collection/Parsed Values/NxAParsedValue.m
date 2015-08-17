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

#import "NxAParsedValue.h"

#pragma mark Implementation

@implementation NxAParsedValue

#pragma mark Accessors

- (NSString *)comparisonValue
{
    // -- This should be overloaded by any derived class
    ALog(@"Illegal base class call from '%@'.", self.class);

    return nil;
}

#pragma mark Instance Methods

- (instancetype)initPrivate
{
    return [super init];
}

- (NxATag *)tagForClass:(Class)tagClass
           usingContext:(NSManagedObjectContext *)context;
{
    // -- This should be overloaded by any derived class
    ALog(@"Illegal base class call from '%@'.", self.class);

    return nil;
}

- (NxATag *)tagForClass:(Class)tagClass
               withName:(NSString *)name
           usingContext:(NSManagedObjectContext *)context
{
    // -- This should be overloaded by any derived class
    ALog(@"Illegal base class call from '%@'.", self.class);

    return nil;
}

#pragma mark Overridden NSObject Instance Methods

- (instancetype)init
{
    ALog(@"Illegal base class call from '%@'.", self.class);
    return nil;
}

@end
