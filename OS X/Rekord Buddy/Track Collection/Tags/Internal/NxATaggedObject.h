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

#import "Track Collection/Tags/NxATaggedObject.h"

#pragma mark Private Interface

@interface NxATaggedObject ()

#pragma mark Properties

@property (strong, nonatomic) NSOrderedSet *p_tagEntries;

@property (strong, nonatomic) NSOrderedSet *p_savedTagValues;

@property (readonly, nonatomic) BOOL p_isInitialized;

#pragma mark Transient Properties

@property (strong, nonatomic) NSMutableDictionary *p_tagsByType;

@property (strong, nonatomic) NSMutableDictionary *p_tagEntriesByType;

@property (strong, nonatomic) NSMutableDictionary *p_savedTagValuesByIdentifier;

#pragma mark Instance Methods

- (void)p_initTaggedObject;

/*!
 * Get all internal entries for a given type.
 * \param tagType Type of tag to look for.
 * \return An array of all entries for that type.
 */
- (NSArray *)p_entriesForType:(NSString *)tagType;

- (void)p_replaceTagClass:(Class)tagClass
                 withTags:(NSArray *)tags
         modificationDate:(NSDate *)date;

- (void)p_replaceTagClass:(Class)tagClass
                  andName:(NSString *)name
                 withTags:(NSArray *)tags
         modificationDate:(NSDate *)date;

/*!
 * Replace tags.
 * \param tagClass Class of tag to replace.
 * \param values Values to replace any existing tags with.
 * \param date Date the new values were created / last modified.
 */
- (void)p_replaceTagClass:(Class)tagClass
         withParsedValues:(NSArray *)values
         modificationDate:(NSDate *)date;

/*!
 * Replace tags.
 * \param tagClass Class of tags to replace.
 * \param name Name of the tags to replace.
 * \param values Values to replace any existing tags with.
 * \param date Date the new values were created / last modified.
 */
- (void)p_replaceTagClass:(Class)tagClass
                  andName:(NSString *)name
         withParsedValues:(NSArray *)values
         modificationDate:(NSDate *)date;

/*!
 * Move tags to a given position.
 * \param type Type of tag to move.
 * \param tags Tags to move.
 * \param index Index position to move the tags to.
 */
- (void)p_moveTagType:(NSString *)type
                 tags:(NSArray *)tags
           toPosition:(NSInteger)index;

@end
