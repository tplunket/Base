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

#import "NxAArtwork.h"
#import "NSManagedObjectContext+NxAUtility.h"
#import "NxATrackArtistCredit.h"
#import "NxATrackRemixerCredit.h"
#import "NxATrackProducerCredit.h"
#import "Track Collection/Internal/NxAArtist.h"

#pragma mark Implementation

@implementation NxAArtist

#pragma mark Factory Methods

+ (instancetype)existingArtistWithName:(NSString *)name
                               context:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(context);
    
    // -- Look for an existing artist.
    NSArray *results = name ? [context fetchFromTemplate:@"ArtistWithName"
                                   substitutionVariables:@{ @"NAME" : name }] :
    [context fetchForEntityName:@"Artist" withPredicate:@"p_name == nil"];
    NxAAssertTrue(results.count < 2);

    return results.count ? results.firstObject : nil;
}

+ (instancetype)artistWithName:(NSString *)name
                       context:(NSManagedObjectContext *)context;
{
    NxAAssertNotNil(context);

    NxAArtist *artist = [NxAArtist existingArtistWithName:name context:context];
    if (artist) {
        return artist;
    }
    
    // -- If we didn't find any results then we don't have already an existing artist by that name.
    NxAArtist *newArtist = [context insertNewObjectForEntity:@"Artist"];
    
    newArtist.p_name = name;
    
    return newArtist;
}

#pragma mark Class Methods

+ (NSArray *)artistsMatchingPredicate:(NSString *)partialPredicateFormat
                              context:(NSManagedObjectContext *)context
{
    NSArray *artists = [NSManagedObjectContext entitiesMatchingPredicate:partialPredicateFormat
                                                              entityName:@"Artist"
                                                                 keyPath:@"p_name"
                                                                 context:context];
    if (!artists.count) {
        return nil;
    }

    return artists;
}

#pragma mark Accessors

// -- Automatically generated for Core Data properties.
@dynamic p_name;
@dynamic p_artwork;
@dynamic p_tracks;
@dynamic p_remixes;
@dynamic p_productions;

- (BOOL)isAnOrphan
{
    return self.p_tracks.isAnOrphan && self.p_remixes.isAnOrphan && self.p_productions.isAnOrphan;
}

#pragma mark Instance Methods

- (void)destroy
{
    NxAAssertTrue(self.isAnOrphan);

    NxAArtwork *artwork = self.p_artwork;
    self.p_artwork = nil;
    if (artwork.isAnOrphan) {
        [artwork destroy];
    }

    NxATrackArtistCredit *trackCredit = self.p_tracks;
    self.p_tracks = nil;
    [trackCredit destroy];

    NxATrackRemixerCredit *remixerCredit = self.p_remixes;
    self.p_remixes = nil;
    [remixerCredit destroy];

    NxATrackProducerCredit *producerCredit = self.p_productions;
    self.p_productions = nil;
    [producerCredit destroy];

    [self.managedObjectContext deleteObject:self];
}

#pragma mark Overridden NSObject Methods

- (NSString *)description
{
    return self.p_name;
}

@end
