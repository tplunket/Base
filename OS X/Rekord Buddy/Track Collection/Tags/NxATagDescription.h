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

#import "Track Collection/Tags/NxATag.h"

@class NxATaggedObject;
@class NxAParsedValue;

#pragma mark Public Interface

@interface NxATagDescription : NSManagedObject

#pragma mark Factory Methods

+ (instancetype)descriptionForValue:(NxAParsedValue *)value
                      andIdentifier:(NSString *)identifier
                          inContext:(NSManagedObjectContext *)context;

#pragma mark Class Methods

+ (NSString *)identifierForTagType:(NSString *)tagType
                  andVersionPrefix:(NSString *)versionPrefix;

+ (NSString *)identifierForTagType:(NSString *)tagType
                        andTagName:(NSString *)tagName
                  andVersionPrefix:(NSString *)versionPrefix;

+ (BOOL) identifier:(NSString *)identifier
 isForVersionPrefix:(NSString *)versionPrefix;

#pragma mark Properties

@property (readonly, nonatomic) NSString *identifier;

@property (readonly, nonatomic) NSString *comparisonValue;

#pragma mark Instance Method

- (void)destroy;

@end
