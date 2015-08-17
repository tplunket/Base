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
#import "NxAHash.h"
#import "Track Collection/Internal/NxAArtwork.h"

#pragma mark Implementation

@implementation NxAArtwork

#pragma mark Factory Methods

+ (instancetype)artworkWithData:(NSData *)data
                        context:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(data);
    NxAAssertNotNil(context);

    NSImage *coverArt = [[NSImage alloc] initWithData:data];
    return [NxAArtwork artworkWithImage:coverArt context:context];
}

+ (instancetype)artworkWithImage:(NSImage *)image
                         context:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(image);
    NxAAssertNotNil(context);

    NSString *hash = [NxAArtwork hashForImage:image];

    // -- Look for an existing artwork.
    NSArray *results = [context fetchFromTemplate:@"ArtworkWithHash"
                            substitutionVariables:@{ @"HASH" : hash }];
    NxAAssertTrue(results.count <= 1);

    if (results.count == 1) {
        return results[0];
    }

    // -- If we didn't find any results then we don't have already an existing artwork like this one.
    NxAArtwork *newArtwork = [context insertNewObjectForEntity:@"Artwork"];

    newArtwork.p_hash = hash;
    newArtwork.p_image = image;

    NxAAssertNotNil(newArtwork);

    return newArtwork;
}

#pragma mark Class Methods

+ (NSString *)hashForImage:(NSImage *)image
{
    NSString *hash = [NxAHash hashFor:image.TIFFRepresentation];
    return hash;
}

#pragma mark Accessors

// -- Automatically generated for Core Data properties.
@dynamic p_hash;
@dynamic p_image;
@dynamic p_tracks;
@dynamic p_artists;
@dynamic p_recordLabels;

- (NSString *)stringValue
{
    return self.p_hash;
}

- (BOOL)isAnOrphan
{
    return (self.p_tracks.count == 0) && (self.p_artists.count == 0) && (self.p_recordLabels.count == 0);
}

#pragma mark Instance Methods

- (void)destroy
{
    NxAAssertTrue(self.isAnOrphan);
    
    [self.managedObjectContext deleteObject:self];
}

@end
