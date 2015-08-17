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

#import "NxAMessageLog.h"

#pragma mark Public Interface

@interface NSXMLElement (NxAUtility)

#pragma mark Class Methods

+ (void)deleteXMLElements:(NSSet *)objects;

#pragma mark Instance Methods

- (instancetype)elementForName:(NSString *)name;

- (void)addAttributeWithName:(NSString *)name
                 stringValue:(NSString *)value;

- (void)removeAllChildren;

- (void)removeChildrenWithName:(NSString *)name;

- (NSArray *)removeAndReturnChildren;

@end
