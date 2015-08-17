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

#import "NSDate+NxAUtility.h"
#import "NSManagedObjectContext+NxAUtility.h"
#import "NxATaggedObject.h"
#import "NxATagDescription.h"
#import "NxATrack.h"
#import "Track Collection/Crates/NxACrate.h"
#import "NxAParsedValue.h"
#import "Track Collection/Tags/Internal/NxATagEntry.h"
#import "NxAMessageLog.h"

#pragma mark Implementation

@implementation NxATagEntry

#pragma mark Factory Methods

+ (instancetype)entryWithTag:(NxATag *)tag
                dateModified:(NSDate *)dateModified
                     context:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(tag);
    NxAAssertNotNil(dateModified);
    NxAAssertNotNil(context);

    NxATagEntry *newEntry = [context insertNewObjectForEntity:@"TagEntry"];

    newEntry.p_tag = tag;
    newEntry.p_dateModified = [dateModified roundedDownToTheNearestSecond];

    return newEntry;
}

#pragma mark Class Methods

/*!
 * Get the tags for a set of tag entries.
 * \param entries Entries to read the tags from.
 * \return A set of tags for the entries, in the same order.
 */
+ (NSOrderedSet *)p_tagsInEntries:(NSOrderedSet *)entries
{
    NSMutableOrderedSet *resultValues = [NSMutableOrderedSet orderedSetWithCapacity:entries.count];

    for (NxATagEntry *entry in entries) {
        [resultValues addObject:entry.tag];
    }

    return resultValues;
}

+ (NSSet *)parentTracksForEntries:(NSSet *)entries
{
    NSMutableSet *results = [NSMutableSet set];

    for (NxATagEntry *entry in entries) {
        NxATaggedObject *object = entry.p_parent;
        if ([object isKindOfClass:[NxATrack class]]) {
            [results addObject:object];
        }
    }

    return results;
}

+ (NSArray *)parentCratesForTagEntries:(NSSet *)entries
                    ignoreSystemCrates:(BOOL)ignoreSystemCrates
{
    NSMutableArray *results = [NSMutableArray array];

    for (NxATagEntry *entry in entries) {
        NxATaggedObject *object = entry.p_parent;
        if ([object isKindOfClass:[NxACrate class]]) {
            NxACrate *crate = (NxACrate *)object;
            if (!ignoreSystemCrates || !crate.isASystemCrate) {
                [results addObject:object];
            }
        }
    }

    return results;
}

+ (NSUInteger)indexOfTagEntryForTag:(NxATag *)tag
                          inEntries:(NSArray *)entries
{
    NSUInteger index = 0;

    for (NxATagEntry *entry in entries) {
        if (entry.tag == tag) {
            return index;
        }

        ++index;
    }

    return NSNotFound;
}

+ (BOOL)    parsedValues:(NSArray *)values
             forTagClass:(Class)tagClass
 areTheSameAsSavedValues:(NSArray *)savedValues;
{
    if (values.count != savedValues.count) {
        return NO;
    }

    NSUInteger numberOfValues = values.count;
    if (!numberOfValues) {
        return YES;
    }

    for (NSUInteger index = 0; index < numberOfValues; ++index) {
        NxATagDescription *savedValue = savedValues[index];
        NxAParsedValue *value = values[index];

        NSLog(@"%@", savedValue.comparisonValue);
        if (![value.comparisonValue isEqualToString:savedValue.comparisonValue]) {
            return NO;
        }
    }

    return YES;
}

+ (NxATagEntry *)firstEntryWithTagName:(NSString *)name
                               entries:(NSArray *)entries
{
    for (NxATagEntry *entry in entries) {
        NxATag *tag = entry.tag;
        if (tag.isNil || [tag.name isEqualToString:name]) {
            return entry;
        }
    }

    return nil;
}

+ (NSArray *)entriesWithTagName:(NSString *)name
                        entries:(NSArray *)entries
{
    NSMutableArray *entriesWithName = [NSMutableArray arrayWithCapacity:entries.count];

    for (NxATagEntry *entry in entries) {
        NxATag *tag = entry.tag;
        if (tag.isNil) {
            continue;
        }
        
        if ([entry.tag.name isEqualToString:name]) {
            [entriesWithName addObject:entry];
        }
    }

    return entriesWithName;
}

+ (void)markEntries:(NSArray *)entries
         asModified:(NSDate *)dateModified
{
    NSDate *roundedDownDate = [dateModified roundedDownToTheNearestSecond];
    for (NxATagEntry *entry in entries) {
        entry.p_dateModified = roundedDownDate;
    }
}

+ (void)setAllTagEntriesAsModified:(NSDate *)date
                         inContext:(NSManagedObjectContext *)context
            andAddAnyMessagesToLog:(NxAMessageLog *)log
{
    [log addMessage:[NSString stringWithFormat:@"All tag entries set as modified on '%@'.", date]];

    NSArray *entries = [context allObjectsWithEntityType:@"TagEntry" error:nil];
    [NxATagEntry markEntries:entries asModified:date];
}

+ (void)destroyOrphansIn:(NSManagedObjectContext *)context
  andAddAnyMessagesToLog:(NxAMessageLog *)log
{
    NSArray *orphanEntries = [context fetchFromTemplate:@"TagEntriesWithNoParent"];
    if (!orphanEntries.count) {
        return;
    }

    [log addMessage:[NSString stringWithFormat:@"  Deleting %ld orphan entries.", orphanEntries.count]];
    for (NxATagEntry *entry in orphanEntries) {
        [entry destroy];
    }
}

#pragma mark Accessors

// -- Automatically generated for Core Data properties.
@dynamic p_tag;
@dynamic p_dateModified;
@dynamic p_parent;

- (NxATag *)tag
{
    return self.p_tag;
}

#pragma mark Instance Methods

- (void)destroy
{
    NxATag *tag = self.p_tag;

    [self destroyButPreserveTag];

    if (tag.isAnOrphan) {
        [tag destroy];
    }
}

- (void)destroyButPreserveTag
{
    NxATaggedObject *parent = self.p_parent;
    if (parent) {
        [parent removeTagEntry:self];
        self.p_parent = nil;
    }

    NxATag *tag = self.p_tag;
    if (tag) {
        self.p_tag = nil;
    }

    [self.managedObjectContext deleteObject:self];
}

- (NxATagEntryModificationCompareFlag)compareToModificationDate:(NSDate *)date;
{
    NSDate *localDate = self.p_dateModified;

    NSComparisonResult results = [localDate compare:date];
    switch (results) {
        case NSOrderedAscending: {
            return NxATagTypeWasLastModifiedBeforeDate;
        }
        case NSOrderedSame: {
            return NxATagTypeWasLastModifiedOnDate;
        }
        case NSOrderedDescending: {
            return NxATagTypeWasLastModifiedAfterDate;
        }
    }
}

#pragma mark Overridden NSObject Methods

- (NSString *)description
{
    NxATaggedObject *object = self.p_parent;

    if ([object isKindOfClass:[NxATrack class]]) {
        return [NSString stringWithFormat:@"Entry for tag '%@' modified on '%@' for track '%@'",
                self.tag,
                self.p_dateModified,
                object];
    }
    else if ([object isKindOfClass:[NxACrate class]]) {
        return [NSString stringWithFormat:@"Entry for tag '%@' modified on '%@' for crate '%@'",
                self.tag,
                self.p_dateModified,
                object];
    }
    else {
        return [NSString stringWithFormat:@"Entry for tag '%@' modified on '%@' with no parent",
                self.tag,
                self.p_dateModified];
    }
}

@end
