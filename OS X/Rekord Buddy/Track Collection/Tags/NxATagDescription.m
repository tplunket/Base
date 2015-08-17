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

#import "NxATagDescription.h"
#import "NSManagedObjectContext+NxAUtility.h"
#import "NxAParsedValue.h"

#pragma mark Private Interface

@interface NxATagDescription ()

#pragma mark Properties

@property (strong, nonatomic) NSString *p_identifier;

@property (strong, nonatomic) NSString *p_comparisonValue;

@property (strong, nonatomic) NSString *p_parentObject;

@end

#pragma mark Implementation

@implementation NxATagDescription

#pragma mark Factory Methods

+ (instancetype)descriptionForValue:(NxAParsedValue *)value
                      andIdentifier:(NSString *)identifier
                          inContext:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(value);
    NxAAssertNotNil(identifier);
    NxAAssertNotNil(context);

    NxATagDescription *newDescription = [context insertNewObjectForEntity:@"TagDescription"];
    NxAAssertNotNil(newDescription);

    newDescription.p_identifier = identifier;
    newDescription.p_comparisonValue = value.comparisonValue;

    return newDescription;
}

#pragma mark Class Methods

+ (NSString *)identifierForTagType:(NSString *)tagType
                  andVersionPrefix:(NSString *)versionPrefix
{
    return [NxATagDescription identifierForTagType:tagType
                                        andTagName:@""
                                  andVersionPrefix:versionPrefix];
}

+ (NSString *)identifierForTagType:(NSString *)tagType
                        andTagName:(NSString *)tagName
                  andVersionPrefix:(NSString *)versionPrefix
{
    NxAAssertNotNil(tagType);
    NxAAssertNotNil(tagName);
    NxAAssertNotNil(versionPrefix);

    return [NSString stringWithFormat:@"%@|%@|%@", versionPrefix, tagName, tagType];
}

+ (BOOL)  identifier:(NSString *)identifier
  isForVersionPrefix:(NSString *)versionPrefix
{
    return [identifier hasPrefix:[NSString stringWithFormat:@"%@|", versionPrefix]];
}

#pragma mark Accessors

// -- Automatically generated for Core Data properties.
@dynamic p_identifier;
@dynamic p_comparisonValue;
@dynamic p_parentObject;

- (NSString *)identifier
{
    return self.p_identifier;
}

- (NSString *)comparisonValue
{
    return self.p_comparisonValue;
}

#pragma mark Instance Method

- (void)destroy
{
    [self.managedObjectContext deleteObject:self];
}

#pragma mark Overridden NSObject Methods

- (NSString *)description
{
    return [NSString stringWithFormat:@"%@: '%@'", self.p_identifier, self.p_comparisonValue];
}

@end
