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

@class NxATrack;
@class NxATaggedObject;
@class NxATagEntry;
@class NxASortingCriteria;

#pragma mark - Public Interface

/*! Tag used to describe track properties. */
@interface NxATag : NSManagedObject

#pragma mark Factory Methods

+ (instancetype)tagWithName:(NSString *)name
                      value:(id)value
                    context:(NSManagedObjectContext *)context;

+ (instancetype)tagWithValue:(id)value
                     context:(NSManagedObjectContext *)context;

+ (instancetype)existingTagWithValue:(id)value
                             context:(NSManagedObjectContext *)context;

+ (instancetype)existingTagWithName:(NSString *)name
                              value:(id)value
                            context:(NSManagedObjectContext *)context;

#pragma mark Class Methods

/*!
 * Find tag properties.
 * \param object Tagged object to look into.
 * \return All tag properties of this tag's type if found or nil if none.
 */
+ (NSArray *)tagsIn:(NxATaggedObject *)object;

/*!
 * Find tag properties.
 * \param object Tagged object to look into.
 * \param name Name to look for.
 * \return All tag properties of this tag's type and with this name if found or nil if none.
 */
+ (NSArray *)tagsIn:(NxATaggedObject *)object
           withName:(NSString *)name;

/*!
 * Find tag properties.
 * \param tags Tags to look into.
 * \param name Name to look for.
 * \return All tag properties of this tag's type and with this name if found or nil if none.
 */
+ (NSArray *)tagsInTags:(NSArray *)tags
               withName:(NSString *)name;

/*!
 * Find tag properties.
 * \param tags Tags to look into.
 * \param name Name to lavoid.
 * \return All tag properties of this tag's type and without this name if found or nil if none.
 */
+ (NSArray *)tagsInTags:(NSArray *)tags
            withoutName:(NSString *)name;

/*!
 * Get all the tracks which tag(s) match a certain predicate.
 * \param partialPredicateFormat Test to use. Should be only be the comparaison operation.
 *                               operator and the right-side comparaison value.
 * \param context Object context to look for the tracks in.
 * \return A set of all the tracks which tag(s) matches the test.
 */
+ (NSSet *)tracksWithTagMatchingPredicate:(NSString *)partialPredicateFormat
                                  context:(NSManagedObjectContext *)context;

/*!
 * Filter a set of tags.
 * \param tags Tags to filter.
 * \param description Description of the property to filter the tags by.
 * \return All tags matching the property name.
 */
+ (NSArray *)filterTags:(NSArray *)tags
            description:(NSString *)description;

/*!
 * Name of the type for this class of tags.
 * \return The tag type name.
 */
+ (NSString *)tagType;

/*!
 * Indicates if this tag type shares tags or not.
 * \return YES if this tag type shares its tags between tagged objects, NO if it does.
 */
+ (BOOL)sharesTags;

/*!
 * Indicates if this tag type is a boolean tag (on/off) or not.
 * \return YES if this tag type is boolean, NO otherwise.
 */
+ (BOOL)isABooleanTagType;

/*!
 * Indicates if this tag type only allows one tag of the given type to be added to a tagged object.
 * \return YES if this tag type create unique tags, NO otherwise.
 */
+ (BOOL)isASingleValueTagType;

/*!
 * Indicates if this tag type should be sorted by property name as well as by type.
 * \return YES if this tag type should be sorted by property name, NO if type only is sufficient.
 */
+ (BOOL)isATagTypeSortedByPropertyName;

/*!
 * Indicates if this tag type allows users to modify its values.
 * \return YES if can modify the values, NO otherwise.
 */
+ (BOOL)valuesAreUserModifiable;

#pragma mark Properties

@property (readonly, nonatomic) BOOL hasName;

@property (readonly, nonatomic) NSString *name;

/*! User-facing description for this type of tag property. */
@property (readonly, nonatomic) NSString *propertyDescription;

@property (readonly, nonatomic) NSString *stringValue;

@property (readonly, nonatomic) NSString *stringValueWithNameIfAny;

/*! Whether this tag has a nil value or not. */
@property (readonly, nonatomic) BOOL isNil;

@property (readonly, nonatomic) BOOL isAnOrphan;

#pragma mark Instance Methods

- (void)destroy;

/*!
 * Create a sorting criteria for this tag's type.
 * \param context Managed object context to create the new object in.
 * \return A sorting criteria with the given key.
 */
- (NxASortingCriteria *)sortingCriteriaInContext:(NSManagedObjectContext *)context;

/*!
 * Check to see if a list of sorting criterias contains one for a given tag.
 * \param set Set of sorting criterias to look into.
 * \return YES if a matching criteria is found, NO otherwise.
 */
- (BOOL)isSortingCriteriaFoundIn:(NSOrderedSet *)set;

@end
