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

#import "Track Collection/Tags/Properties/NxAStringProperty.h"
#import "NSManagedObjectContext+NxAUtility.h"
#import "NSCharacterSet+NxAUtility.h"
#import "Track Collection/Tags/Properties/Internal/NxAStringProperty.h"

#pragma mark Implementation

@implementation NxAStringProperty

#pragma mark Overridden NxATag Class Methods

+ (NSString *)p_managedEntityName
{
    return @"StringProperty";
}

+ (Class)p_valueClass
{
    return [NSString class];
}

#pragma mark Accessors

// -- Automatically generated for Core Data properties.
@dynamic p_value;

#pragma mark Overridden NxATag Accessors

- (NSString *)stringValue
{
    return self.p_value;
}

@end
