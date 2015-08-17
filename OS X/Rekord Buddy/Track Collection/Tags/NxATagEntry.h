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

#include "Track Collection/Tags/NxATag.h"
#include "NxAParsedValue.h"

@class NxATaggedObject;
@class NxAMessageLog;

#pragma mark Constants

/*!
 *  enum NxATagEntryModificationCompareFlag
 *  When tag entries were modified in comparaison to a given date.
 */
typedef NS_ENUM(NSInteger, NxATagEntryModificationCompareFlag) {
    NxATagTypeWasLastModifiedBeforeDate,  /*!< The tag entries were modified before the given date. */
    NxATagTypeWasLastModifiedOnDate,      /*!< The tag entries were modified on the given date. */
    NxATagTypeWasLastModifiedAfterDate,   /*!< The tag entries were modified after the given date. */
};

#pragma mark - Public Interface

/*! Tag entry for a property associated with a track. */
@interface NxATagEntry : NSManagedObject

#pragma mark Factory Methods

+ (instancetype)entryWithTag:(NxATag *)tag
                dateModified:(NSDate *)dateModified
                     context:(NSManagedObjectContext *)context;

#pragma mark Class Methods

/*!
 * Get the parent tracks which contain some tag entries.
 * \param entries Entries to look into.
 * \return A set of the parent tracks for these entries.
 */
+ (NSSet *)parentTracksForEntries:(NSSet *)entries;

/*!
 * Get the parent crates which contain some tag entries.
 * \param tagEntries Tag entries to look into.
 * \param ignoreSystemCrates If YES then no system crates will be returned.
 * \return An array of the parent crates for these entries.
 */
+ (NSArray *)parentCratesForTagEntries:(NSSet *)tagEntries
                    ignoreSystemCrates:(BOOL)ignoreSystemCrates;

/*!
 * Get the index of a tag entries for a given tag.
 * \param tag Tag to look for.
 * \param entries Entries to find the tag in.
 * \return The index of the tag entry if found or NSNotFound otherwise.
 */
+ (NSUInteger)indexOfTagEntryForTag:(NxATag *)tag
                          inEntries:(NSArray *)entries;

+ (BOOL)    parsedValues:(NSArray *)values
             forTagClass:(Class)tagClass
 areTheSameAsSavedValues:(NSArray *)savedValues;

/*
 * Return the first entry with a tag with a given name.
 * \param name Name to look for.
 * \param entries Entries to look into.
 * \return The first entry the tag with that name if found or nil otherwise.
 */
+ (NxATagEntry *)firstEntryWithTagName:(NSString *)name
                               entries:(NSArray *)entries;

/*
 * Return all the entries with a tag with a given name.
 * \param name Name to look for.
 * \param entries Entries to look into.
 * \return All the entries with the tag with that name.
 */
+ (NSArray *)entriesWithTagName:(NSString *)name
                        entries:(NSArray *)entries;

/*!
 * Mark a list of entries as modifiedon a certain date.
 * \param entries Entries to modifiy.
 * \param dateModified Date to det these entries as modified on.
 */
+ (void)markEntries:(NSArray *)entries
         asModified:(NSDate *)dateModified;

+ (void)setAllTagEntriesAsModified:(NSDate *)date
                         inContext:(NSManagedObjectContext *)context
            andAddAnyMessagesToLog:(NxAMessageLog *)log;

+ (void)destroyOrphansIn:(NSManagedObjectContext *)context
  andAddAnyMessagesToLog:(NxAMessageLog *)log;

#pragma mark Properties

/*! Tag associated with this entry. */
@property (readonly, nonatomic) NxATag *tag;

#pragma mark Instance Methods

- (void)destroy;

- (void)destroyButPreserveTag;

/*!
 * Compare an entry with a given modification date.
 * \param date Date to test against.
 * \return Whether the entry was modified before, on or after the date.
 */
- (NxATagEntryModificationCompareFlag)compareToModificationDate:(NSDate *)date;

@end
