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

#import "NSManagedObjectContext+NxAUtility.h"
#import "NxATrack.h"
#import "Track Collection/Tags/NxATag.h"
#import "Track Collection/Internal/NxASortingCriteria.h"

#pragma mark Implementation

@implementation NxASortingCriteria

#pragma mark Factory Methods

+ (instancetype)criteriaWithTagClass:(Class)tagClass
                         description:(NSString *)description
                             context:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(description);
    NxAAssertNotNil(context);

    // -- Create a new object.
    NxASortingCriteria *newCriteria = [context insertNewObjectForEntity:@"SortingCriteria"];

    newCriteria.p_sortTagClassName = NSStringFromClass(tagClass);
    newCriteria.p_sortTagDescription = description;

    return newCriteria;
}

#pragma mark Class Methods

+ (BOOL)isCriteriaForTagDescription:(NSString *)description
                            foundIn:(NSOrderedSet *)set
{
    for (NxASortingCriteria *test in set) {
        if ([test.p_sortTagDescription isEqualToString:description]) {
            return YES;
        }
    }

    return NO;
}

+ (BOOL)isCriteriaForTag:(NxATag *)tag
                 foundIn:(NSOrderedSet *)set
{
    return [tag isSortingCriteriaFoundIn:set];
}

#pragma mark Accessors

// -- Automatically generated for Core Data properties.
@dynamic p_sortTagClassName;
@dynamic p_sortTagDescription;
@dynamic p_ascending;
@dynamic p_parentCrate;

- (NSSortDescriptor *)sortDescriptor
{
    // TODO: Descriptor should be for this one tag only.
    NSString *keyName = @"tags";
    NSSortDescriptor *sortDesc = [[NSSortDescriptor alloc] initWithKey:keyName
                                                             ascending:self.p_ascending.boolValue
                                                              selector:@selector(localizedStandardCompare:)];

    return sortDesc;
}

- (NSString *)sortingPropertyDescription
{
    return self.p_sortTagDescription;
}

#pragma mark Instance Methods

- (void)destroy
{
    [self.p_parentCrate removeSortingCriteria:self];

    [self.managedObjectContext deleteObject:self];
}

- (void)flipSortingOrder
{
    self.p_ascending = @(!self.p_ascending.boolValue);
}

- (NSArray *)sortTracks:(NSArray *)tracks
{
    return [NxATrack sortTracks:tracks
                     byTagClass:NSClassFromString(self.p_sortTagClassName)
             withTagDescription:self.p_sortTagDescription
                   fromCriteria:self
                  inParentCrate:self.p_parentCrate];
}

#pragma mark Overridden NSObject Methods

- (NSString *)description
{
    return [NSString stringWithFormat:@"Sorting Criteria for '%@' of name '%@'.", self.p_sortTagClassName, self.p_sortTagDescription];
}

#pragma mark NSCopying protocol

- (id)copyWithZone:(NSZone *)zone
{
    NxAAssertNotNil(self.managedObjectContext);

    // -- Create a new object.
    NxASortingCriteria *newCriteria = [self.managedObjectContext insertNewObjectForEntity:@"SortingCriteria"];

    NSArray *properties = self.entity.properties;
    for (NSAttributeDescription *attribute in properties) {
        // -- We want only actual attributes, not relationships or fetched properties.
        if([attribute isKindOfClass:[NSAttributeDescription class]]) {
            [newCriteria setValue:[self valueForKey:attribute.name] forKey:attribute.name];
        }
    }

    return newCriteria;
}

@end
