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

@class NxACrate;
@class NxAAudioFile;

#import "NxATaggedObject.h"

#pragma mark Public Interface

/*! Musical track with audio and various properties. */
@interface NxATrack : NxATaggedObject

#pragma mark Class Methods

/*!
 * Get an array of tracks from the pasteboard.
 * \param pasteboard Pasteboard to read the track representation of the track.
 * \param context Managed object context to get the tracks from.
 * \return An array of tracks matching the representations or nil if failed.
 */
+ (NSArray *)tracksFromPasteboard:(NSPasteboard *)pasteboard
                          context:(NSManagedObjectContext *)context;

+ (NSArray *)sortTracks:(NSArray *)tracks
             byTagClass:(Class)tagClass
     withTagDescription:(NSString *)tagDescription
           fromCriteria:(NxASortingCriteria *)criteria
          inParentCrate:(NxACrate *)parentCrate;

#pragma mark Properties

@property (readonly, nonatomic) NxAAudioFile *fileForCurrentPlatform;

@property (readonly, nonatomic) NSString *stringValue;

@property (readonly, nonatomic) BOOL isAnOrphan;

@property (readonly, nonatomic) NSDate *dateModified;

#pragma mark Instance Methods

- (void)clearCaches;

- (void)markAsModifiedOn:(NSDate *)date;

- (void)replaceTagClass:(Class)tagClass
               withTags:(NSArray *)tags
       modificationDate:(NSDate *)date;

@end
