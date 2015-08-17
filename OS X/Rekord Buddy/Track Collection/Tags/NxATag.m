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
#import "NSString+NxAUtility.h"
#import "NxATrack.h"
#import "NxATaggedObject.h"
#import "Track Collection/NxASortingCriteria.h"
#import "NxATagEntry.h"
#import "Track Collection/Tags/Internal/NxATag.h"

#pragma mark Implementation

@implementation NxATag

#pragma mark Factory Methods

+ (id)p_existingTagWithName:(NSString *)name
                      value:(id)tagValue
                    context:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(context);
    
    NSString *entityName = self.p_managedEntityName;
    
    NSString *fetchTemplateName = nil;
    NSDictionary *substitutionVariables = nil;
    
    NSArray *results = nil;
    
    if (tagValue) {
        NxAAssertTrue([tagValue isKindOfClass:[self p_valueClass]]);
        
        if (name) {
            fetchTemplateName = [NSString stringWithFormat:@"%@WithNameAndValue", entityName];
            substitutionVariables = @{ @"NAME" : name, @"VALUE" : tagValue };
        }
        else {
            fetchTemplateName = [NSString stringWithFormat:@"%@WithValue", entityName];
            substitutionVariables = @{ @"VALUE" : tagValue };
        }
        
        NSFetchRequest *request = [context fetchRequestFromTemplate:fetchTemplateName
                                              substitutionVariables:substitutionVariables];
        if (!request) {
            if ([self.class sharesTags]) {
                ALog(@"Can't find a fetch request for class '%@'.", [self class]);
            }
        }
        else {
            NSError *systemError = nil;
            
            results = [context executeFetchRequest:request error:&systemError];
            if (systemError) {
                NSLog(@"%@", systemError);
                
                return nil;
            }
        }
    }
    else {
        results = [context fetchForEntityName:entityName withPredicate:@"p_value == nil"];
    }
    
    NxAAssertTrue(results.count < 2);

    return results.count ? results.firstObject : nil;
}

+ (id)p_tagWithName:(NSString *)name
              value:(id)value
            context:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(context);

    if ([self.class sharesTags]) {
        id tagFound = [self p_existingTagWithName:name value:value context:context];
        if (tagFound) {
            return tagFound;
        }
    }
    
    // -- If we didn't find any results then we don't have already an existing property by that name and value.
    NxATag *newTag = [context insertNewObjectForEntity:self.p_managedEntityName];
    NxAAssertNotNil(newTag);
    
    if (name) {
        newTag.p_tagName = name;
    }
    
    if (value) {
        // -- We're doing this on a instance variable that's only found in our subclasses,
        // -- so we have to access it via KVC
        [newTag setValue:value forKey:@"p_value"];
    }
    
    return newTag;
}

+ (instancetype)tagWithName:(NSString *)name
                      value:(id)value
                    context:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(name);

    return [self p_tagWithName:name
                         value:value
                       context:context];
}

+ (instancetype)tagWithValue:(id)value
                     context:(NSManagedObjectContext *)context
{
    return [self p_tagWithName:nil
                         value:value
                       context:context];
}

+ (instancetype)existingTagWithValue:(id)parsedValue
                             context:(NSManagedObjectContext *)context
{
    return [self p_existingTagWithName:nil value:parsedValue context:context];
}

+ (instancetype)existingTagWithName:(NSString *)name
                              value:(id)value
                            context:(NSManagedObjectContext *)context
{
    return [self p_existingTagWithName:name value:value context:context];
}

#pragma mark Class Methods

+ (NSString *)p_managedEntityName
{
    ALog(@"Invalid base class method call.");
    return nil;
}

+ (Class)p_valueClass
{
    ALog(@"Invalid base class method call.");
    return nil;
}

+ (NSSet *)tracksWithTagMatchingPredicate:(NSString *)partialPredicateFormat
                                  context:(NSManagedObjectContext *)context
{
    NSArray *tags = [NSManagedObjectContext entitiesMatchingPredicate:partialPredicateFormat
                                                           entityName:[self p_managedEntityName]
                                                              keyPath:@"p_value"
                                                              context:context];
    if (!tags.count) {
        return nil;
    }

    NSMutableSet *results = [NSMutableSet set];
    for (NxATag *tag in tags) {
        NSSet *tracks = [NxATagEntry parentTracksForEntries:tag.p_parentTagEntries];
        [results unionSet:tracks];
    }

    return results;
}

+ (NSArray *)tagsIn:(NxATaggedObject *)object
{
    NSArray *tags = [object nonNilTagsWithType:[self tagType]];
    if ([self isASingleValueTagType]) {
        NxAAssertTrue(tags.count <= 1);
    }

    return tags;
}

+ (NSArray *)tagsIn:(NxATaggedObject *)object
           withName:(NSString *)name
{
    NSArray *tags = [object nonNilTagsWithType:[self tagType] andName:name];
    if ([self isASingleValueTagType]) {
        NxAAssertTrue(tags.count == 1);
    }

    return tags;
}

+ (NSArray *)tagsInTags:(NSArray *)tags
               withName:(NSString *)name
{
    NSMutableArray *tagsFound = [NSMutableArray arrayWithCapacity:tags.count];

    for (NxATag *tag in tags) {
        if (!tag.isNil && [tag.p_tagName isEqualToString:name]) {
            [tagsFound addObject:tag];
        }
    }

    if (!tagsFound.count) {
        return nil;
    }

    return tagsFound;
}

+ (NSArray *)tagsInTags:(NSArray *)tags
            withoutName:(NSString *)name
{
    NSMutableArray *tagsFound = [NSMutableArray arrayWithCapacity:tags.count];

    for (NxATag *tag in tags) {
        if (!tag.isNil && ![tag.p_tagName isEqualToString:name]) {
            [tagsFound addObject:tag];
        }
    }

    if (!tagsFound.count) {
        return nil;
    }

    return tagsFound;
}

+ (NSArray *)filterTags:(NSArray *)tags
            description:(NSString *)description
{
    NSMutableArray *results = [NSMutableArray array];

    for (NxATag *tag in tags) {
        NSString *nameToTestAgainst = tag.propertyDescription;
        if ([nameToTestAgainst isEqualToString:description]) {
            [results addObject:tag];
        }
    }

    return results.count ? results : nil;
}

+ (NSString *)tagType
{
    return NSStringFromClass(self);
}

+ (BOOL)sharesTags
{
    // -- By default, all tags are shared between different tagged objects.
    return YES;
}

+ (BOOL)isABooleanTagType
{
    // -- By default, all tags are not booleans.
    return NO;
}

+ (BOOL)isASingleValueTagType
{
    // -- By default, all tags can be added multiple times to a tagged object.
    return NO;
}

+ (BOOL)isATagTypeSortedByPropertyName
{
    return NO;
}

+ (BOOL)valuesAreUserModifiable
{
    return YES;
}

#pragma mark Accessors

// -- Automatically generated for Core Data properties.
@dynamic p_tagName;
@dynamic p_parentTagEntries;

- (NSString *)propertyDescription
{
    // -- This should be overloaded by any derived class
    ALog(@"Illegal base class call from '%@'.", self.class);

    return nil;
}

- (BOOL)hasName
{
    return self.p_tagName != nil;
}

- (NSString *)name
{
    NxAAssertNotNil(self.p_tagName);
    return self.p_tagName;
}

- (NSString *)stringValue
{
    // -- This should be overloaded by any derived class
    ALog(@"Illegal base class call from '%@'.", self.class);

    return nil;
}

- (NSString *)stringValueWithNameIfAny
{
    NSString *value = self.stringValue;

    return self.hasName ? [NSString stringWithFormat:@"%@:%@", self.name, value] : value;
}

- (BOOL)isNil
{
    return [self valueForKey:@"p_value"] == nil;
}

- (BOOL)isAnOrphan
{
    return self.p_parentTagEntries.count == 0;
}

#pragma mark Instance Methods

- (void)destroy
{
    if (self.p_parentTagEntries.count) {
        NSOrderedSet *parentEntriesCopy = self.p_parentTagEntries.copy;
        for (NxATagEntry *entry in parentEntriesCopy) {
            [entry destroy];
        }
    }

    [self.managedObjectContext deleteObject:self];
}

- (NxASortingCriteria *)sortingCriteriaInContext:(NSManagedObjectContext *)context
{
    return [NxASortingCriteria criteriaWithTagClass:self.class
                                        description:self.propertyDescription
                                            context:context];
}

- (BOOL)isSortingCriteriaFoundIn:(NSOrderedSet *)set
{
    return [NxASortingCriteria isCriteriaForTagDescription:self.propertyDescription
                                                   foundIn:set];

}

#pragma mark Overridden NSObject Methods

- (NSString *)description
{
    if (self.p_tagName) {
        return [NSString stringWithFormat:@"%@:%@:%@", self.class.tagType, self.p_tagName, self.stringValue];
    }
    else {
        return [NSString stringWithFormat:@"%@:%@", self.class.tagType, self.stringValue];
    }
}

@end
