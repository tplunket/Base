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

@implementation NSManagedObjectContext (NxAUtility)

#pragma mark Class Methods

+ (instancetype)contextInMemoryWithConcurrencyType:(NSManagedObjectContextConcurrencyType)concurrencyType
                                             error:(NSError **)error
{
    // -- ObjectModel from any models in app bundle.
    NSManagedObjectModel *managedObjectModel = [NSManagedObjectModel mergedModelFromBundles:nil];

    // -- Coordinator with in-mem store type.
    NSPersistentStoreCoordinator *coordinator = [[NSPersistentStoreCoordinator alloc] initWithManagedObjectModel:managedObjectModel];
    [coordinator addPersistentStoreWithType:NSInMemoryStoreType configuration:nil URL:nil options:nil error:error];

    // -- Context with concurrency type.
    NSManagedObjectContext *context = [[NSManagedObjectContext alloc] initWithConcurrencyType:concurrencyType];
    context.persistentStoreCoordinator = coordinator;

    return context;
}

+ (NSArray *)entitiesMatchingPredicate:(NSString *)partialPredicateFormat
                            entityName:(NSString *)entityName
                               keyPath:(NSString *)keyPath
                               context:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(partialPredicateFormat);
    NxAAssertNotNil(entityName);
    NxAAssertNotNil(keyPath);
    NxAAssertNotNil(context);

    NSFetchRequest *request = [NSFetchRequest fetchRequestWithEntityName:entityName];
    NSString *predicateFormat = [NSString stringWithFormat:@"%@ %@", keyPath, partialPredicateFormat];

    NSPredicate *predicate = [NSPredicate predicateWithFormat:predicateFormat];
    [request setPredicate:predicate];

    NSArray *entities = [context executeFetchRequest:request error:nil];
    if (!entities.count) {
        entities = nil;
    }

    return entities;
}

#pragma mark Instance Methods

- (id)insertNewObjectForEntity:(NSString *)entityName
{
    NxAAssertNotNil(entityName);

    id newObject = [NSEntityDescription insertNewObjectForEntityForName:entityName
                                                 inManagedObjectContext:self];
    NxAAssertNotNil(newObject);
    
    return newObject;
}

- (NSFetchRequest *)fetchRequestFromTemplate:(NSString *)name
{
    NxAAssertNotNil(name);

    NSPersistentStoreCoordinator *psc = self.persistentStoreCoordinator;
    NxAAssertNotNil(psc);

    NSManagedObjectModel *model = psc.managedObjectModel;
    NxAAssertNotNil(model);

    // -- Look up the context for an existing ID/Path combination.
    NSFetchRequest *request = [model fetchRequestTemplateForName:name];

    return request;
}

- (NSArray *)fetchFromTemplate:(NSString *)name
{
    NSFetchRequest *request = [self fetchRequestFromTemplate:name];

    NSError *systemError = nil;
    NSArray *results = [self executeFetchRequest:request error:&systemError];

    if (systemError) {
        NSLog(@"%@", systemError);
        return nil;
    }

    return results;
}

- (NSFetchRequest *)fetchRequestFromTemplate:(NSString *)name
                       substitutionVariables:(NSDictionary *)variables
{
    NxAAssertNotNil(name);
    NxAAssertNotNil(variables);

    NSPersistentStoreCoordinator *psc = self.persistentStoreCoordinator;
    NxAAssertNotNil(psc);

    NSManagedObjectModel *model = psc.managedObjectModel;
    NxAAssertNotNil(model);

    // -- Look up the context for an existing ID/Path combination.
    NSFetchRequest *request = [model fetchRequestFromTemplateWithName:name
                                                substitutionVariables:variables];
    
    return request;
}

- (NSArray *)fetchFromTemplate:(NSString *)name
         substitutionVariables:(NSDictionary *)variables
{
    NSFetchRequest *request = [self fetchRequestFromTemplate:name substitutionVariables:variables];

    NSError *systemError = nil;
    NSArray *results = [self executeFetchRequest:request error:&systemError];

    if (systemError) {
        NSLog(@"%@", systemError);
        return nil;
    }

    return results;
}

- (NSFetchRequest *)fetchRequestForEntityName:(NSString *)entityName
                                withPredicate:(id)stringOrPredicate, ...
{
    NSEntityDescription *entity = [NSEntityDescription entityForName:entityName
                                              inManagedObjectContext:self];

    NSFetchRequest *request = [[NSFetchRequest alloc] init];
    [request setEntity:entity];

    if (stringOrPredicate) {
        NSPredicate *predicate;
        if ([stringOrPredicate isKindOfClass:[NSString class]])
        {
            va_list variadicArguments;
            va_start(variadicArguments, stringOrPredicate);
            predicate = [NSPredicate predicateWithFormat:stringOrPredicate
                                               arguments:variadicArguments];
            va_end(variadicArguments);
        }
        else
        {
            NxAAssertTrue([stringOrPredicate isKindOfClass:[NSPredicate class]]);
            predicate = (NSPredicate *)stringOrPredicate;
        }

        [request setPredicate:predicate];
    }

    return request;
}

- (NSArray *)fetchForEntityName:(NSString *)entityName
                  withPredicate:(id)stringOrPredicate, ...
{
    NSEntityDescription *entity = [NSEntityDescription
                                   entityForName:entityName inManagedObjectContext:self];

    NSFetchRequest *request = [[NSFetchRequest alloc] init];
    [request setEntity:entity];

    if (stringOrPredicate) {
        NSPredicate *predicate;
        if ([stringOrPredicate isKindOfClass:[NSString class]])
        {
            va_list variadicArguments;
            va_start(variadicArguments, stringOrPredicate);
            predicate = [NSPredicate predicateWithFormat:stringOrPredicate
                                               arguments:variadicArguments];
            va_end(variadicArguments);
        }
        else
        {
            NxAAssertTrue([stringOrPredicate isKindOfClass:[NSPredicate class]]);
            predicate = (NSPredicate *)stringOrPredicate;
        }

        [request setPredicate:predicate];
    }

    NSError *systemError = nil;
    NSArray *results = [self executeFetchRequest:request error:&systemError];

    if (systemError) {
        NSLog(@"%@", systemError);
        return nil;
    }

    return results;
}

- (NSManagedObject *)objectWithURI:(NSURL *)uri
{
    NSManagedObjectID *objectID = [self.persistentStoreCoordinator managedObjectIDForURIRepresentation:uri];

    if (!objectID) {
        return nil;
    }

    NSManagedObject *objectForID = [self objectWithID:objectID];
    if (![objectForID isFault]) {
        return objectForID;
    }

    NSFetchRequest *request = [[NSFetchRequest alloc] init];
    [request setEntity:[objectID entity]];

    // -- Equivalent to predicate = [NSPredicate predicateWithFormat:@"SELF = %@", objectForID];
    NSPredicate *predicate = [NSComparisonPredicate predicateWithLeftExpression:[NSExpression expressionForEvaluatedObject]
                                                                rightExpression:[NSExpression expressionForConstantValue:objectForID]
                                                                       modifier:NSDirectPredicateModifier
                                                                           type:NSEqualToPredicateOperatorType
                                                                        options:0];
    [request setPredicate:predicate];

    NSArray *results = [self executeFetchRequest:request error:nil];

    if ([results count] > 0 ) {
        return [results objectAtIndex:0];
    }

    return nil;
}

- (NSArray *)allObjectsWithEntityType:(NSString *)name
                                error:(NSError **)error
{
    NSFetchRequest *request = [[NSFetchRequest alloc] init];

    NSEntityDescription *entity = [NSEntityDescription entityForName:name inManagedObjectContext:self];
    if (!entity) {
        // -- Entity doesnt exist, we just return nil.
        return nil;
    }

    [request setEntity:entity];

    NSArray *array = [self executeFetchRequest:request error:error];
    return array;
}

@end
