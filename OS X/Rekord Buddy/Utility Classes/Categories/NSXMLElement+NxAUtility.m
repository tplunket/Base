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

#import "NSXMLElement+NxAUtility.h"

#pragma mark Implementation

@implementation NSXMLElement (NxAUtility)

#pragma mark Class Methods

+ (void)deleteXMLElements:(NSSet *)objects
{
    for (id sourceObject in objects) {
        NxAAssertTrue([sourceObject isKindOfClass:[NSXMLElement class]]);
        NSXMLElement *sourceElement = (NSXMLElement *)sourceObject;

        [sourceElement detach];
        [sourceElement removeAllChildren];
    }
}

#pragma mark Instance Methods

- (instancetype)elementForName:(NSString *)name
{
    NSArray *childelements = [self elementsForName:name];
    return (childelements.count == 1) ? childelements.firstObject : nil;
}

- (void)addAttributeWithName:(NSString *)name
                 stringValue:(NSString *)value
{
    NSXMLNode *attribute = [NSXMLNode attributeWithName:name stringValue:value];
    [self addAttribute:attribute];
}

- (void)removeAllChildren
{
    NSArray *childrens = self.children;
    for (NSXMLElement *children in childrens) {
        [children detach];
    }
}

- (void)removeChildrenWithName:(NSString *)name
{
    NSArray *elementsToRemove = [self elementsForName:name];
    for (NSXMLElement *element in elementsToRemove) {
        [element detach];
    }
}

- (NSArray *)removeAndReturnChildren
{
    NSArray *children = self.children;
    [self removeAllChildren];
    return children;
}

@end
