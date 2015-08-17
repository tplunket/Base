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

@class NxATag;

#import "NxATagEntry.h"

#pragma mark Constants

/*!
 *  enum NxAParsingResultFlags
 *  Whether an object we just parsed needs updating, caused us to update or was unchanged.
 */
typedef NS_ENUM(NSInteger, NxAParsingResultFlags) {
    NxAParsedObjectWasUpToDate      = 0,        /*!< The object parsed hadn't changed since the last sync. */
    NxAParsedObjectNeedsUpdating    = 1<<0,     /*!< The object parsed needs to be updated. */
    NxAParsedObjectCausedUpdating   = 1<<1,     /*!< The object parsed caused us to update the collection. */
};

#pragma mark Public Interface

/*! An object with a list of tags associated with it. */
@interface NxATaggedObject : NSManagedObject

#pragma mark Properties

/*! List of tags that can be used for sorting, one for each seperate tag types found in the object. */
@property (readonly, nonatomic) NSArray *tagTypesForSorting;

#pragma mark Instance Methods

/*!
 * Compare all entries for a tag class with a given modification date.
 * \param tagClass Class of tags to test.
 * \param date Date to check against.
 * \return Whether the entries were modified before, on or after the date.
 */
- (NxATagEntryModificationCompareFlag)compareTagClass:(Class)tagClass
                                   toModificationDate:(NSDate *)date;

/*!
 * Compare all entries for a tag class and a certain name with a given modification date.
 * \param tagClass Class of tags to test.
 * \param name Name of the tags to test.
 * \param date Date to check against.
 * \return Whether the entry was modified before, on or after the date.
 */
- (NxATagEntryModificationCompareFlag)compareTagClass:(Class)tagClass
                                             withName:(NSString *)name
                                   toModificationDate:(NSDate *)date;

- (void)destroy;

- (void)addNilTagForTagClass:(Class)tagClass
                  modifiedOn:(NSDate *)date;

- (void) addTag:(NxATag *)tag
 lastModifiedOn:(NSDate *)date;

- (void)removeTagEntry:(NxATagEntry *)entry;

/*!
 * Force this object to rest its caches.
 */
- (void)resetCaches;

- (void)removeTagsWithClass:(Class)tagClass;

- (void)removeTagsWithClass:(Class)tagClass
                    andName:(NSString *)name;

- (BOOL)hasTagsWithType:(NSString *)type;

- (BOOL)hasNonNilTagsWithType:(NSString *)type;

- (NSArray *)allTagsWithType:(NSString *)type;

- (NSArray *)allTagsWithType:(NSString *)type
                     andName:(NSString *)name;

- (NSArray *)nonNilTagsWithType:(NSString *)type;

- (NSArray *)nonNilTagsWithType:(NSString *)type
                        andName:(NSString *)name;

- (void)saveValues:(NSArray *)newValues
       forTagClass:(Class)tagClass
        andVersion:(NSString *)versionPrefix;

- (void)saveValues:(NSArray *)values
       forTagClass:(Class)tagClass
           andName:(NSString *)name
        andVersion:(NSString *)versionPrefix;

- (BOOL)  parsedValues:(NSArray *)values
           forTagClass:(Class)tagClass
   areTheSameAsVersion:(NSString *)versionPrefix;

- (BOOL)  parsedValues:(NSArray *)values
           forTagClass:(Class)tagClass
              withName:(NSString *)name
   areTheSameAsVersion:(NSString *)versionPrefix;

@end
