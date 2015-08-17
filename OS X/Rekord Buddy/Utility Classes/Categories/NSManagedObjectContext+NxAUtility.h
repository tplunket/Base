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

@interface NSManagedObjectContext (NxAUtility)

#pragma mark Class Methods

/*!
 * Utility method to get an in-memory core data context.
 * \param concurrencyType The concurrency pattern with which context will be used.
 * \param error If there is a problem executing the fetch, upon return contains an instance of NSError that describes the problem.
 * \return A managed object context created all in-memory.
 */
+ (instancetype)contextInMemoryWithConcurrencyType:(NSManagedObjectContextConcurrencyType)concurrencyType
                                             error:(NSError **)error;

/*!
 * Get all the entities which match a certain predicate.
 * \param partialPredicateFormat Test to use. Should be only be the comparaison operation
 *                               operator and the right-side comparaison value.
 * \param entityName Name of the type of entity to return.
 * \param keyPath Left-side comparaison keypath name.
 * \param context Object context to look for the tracks in.
 * \return A set of entities which matches the test.
 */
+ (NSArray *)entitiesMatchingPredicate:(NSString *)partialPredicateFormat
                            entityName:(NSString *)entityName
                               keyPath:(NSString *)keyPath
                               context:(NSManagedObjectContext *)context;

#pragma mark Instance Methods

/*!
 * Add a new object of a given entity type.
 * \param entityName Name of the entity type to add.
 * \return The object added.
 */
- (id)insertNewObjectForEntity:(NSString *)entityName;

/*!
 * Get a fetch request based on a fetch request template.
 * \param name Name of the fetch template.
 * \return A fetch request.
 */
- (NSFetchRequest *)fetchRequestFromTemplate:(NSString *)name;

/*!
 * Fetch objects based on a fetch request template.
 * \param name Name of the fetch template.
 * \return An array of all results fetched.
 */
- (NSArray *)fetchFromTemplate:(NSString *)name;

/*!
 * Get a fetch request based on a fetch request template.
 * \param name Name of the fetch template.
 * \param variables List of variables and their values to substitute in the request.
 * \return A fetch request.
 */
- (NSFetchRequest *)fetchRequestFromTemplate:(NSString *)name
                       substitutionVariables:(NSDictionary *)variables;

/*!
 * Fetch objects based on a fetch request template.
 * \param name Name of the fetch template.
 * \param variables List of variables and their values to substitute in the request.
 * \return An array of all results fetched.
 */
- (NSArray *)fetchFromTemplate:(NSString *)name
         substitutionVariables:(NSDictionary *)variables;

/*!
 * Get a fetch request for a given entity type.
 * \param entityName Name of the entity to fetch.
 * \param stringOrPredicate Either a string representation of a predicate or the predicate itself.
 * \return A fetch request.
 */
- (NSFetchRequest *)fetchRequestForEntityName:(NSString *)entityName
                                withPredicate:(id)stringOrPredicate, ...;

/*!
 * Fetch objects based on a fetch request template.
 * \param entityName Name of the entity to fetch.
 * \param stringOrPredicate Either a string representation of a predicate or the predicate itself.
 * \return An array of all results fetched.
 */
- (NSArray *)fetchForEntityName:(NSString *)entityName
                  withPredicate:(id)stringOrPredicate, ...;

/*!
 * Get a valid managed object from an object ID url.
 * \details Taken from http://www.cocoawithlove.com/2008/08/safely-fetching-nsmanagedobject-by-uri.html
 * \param uri A url constructed from a manages object's id.
 * \return A valid object if found, nil otherwise.
 */
- (NSManagedObject *)objectWithURI:(NSURL *)uri;

/*!
 * Get all the objects of a given entity type
 * \param name Name of the entity type to look for.
 * \param error If non nil, will be assigned any error produced by this method.
 * \return All objects of the given type, if any.
 */
- (NSArray *)allObjectsWithEntityType:(NSString *)name
                                error:(NSError **)error;

@end
