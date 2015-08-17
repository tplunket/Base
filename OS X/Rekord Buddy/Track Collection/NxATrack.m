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

#import "Track Collection/Internal/NxATrack.h"
#import "Track Collection/Tags/NxATag.h"
#import "Track Collection/Crates/NxACrate.h"
#import "Track Collection/Tags/Crate Properties/NxATrackProperty.h"
#import "NSNumber+NxAUtility.h"
#import "NSDate+NxAUtility.h"
#import "NxAAudioFile.h"
#import "Track Collection/AudioFiles/OSX/NxAOSXAudioFile.h"
#import "NSString+NxAUtility.h"
#import "NSOrderedSet+NxAUtility.h"
#import "NxAAudioData.h"
#import "NxATrackTitle.h"
#import "NxATrackArtistCredit.h"
#import "NxATrackTag.h"
#import "NxATrackDateModified.h"
#import "NxATrackSortingBucket.h"
#import "NxATagEntry.h"
#import "NxATaggedObject.h"
#import "NSManagedObjectContext+NxAUtility.h"

#pragma mark Implementation

@implementation NxATrack

#pragma mark Class Methods

+ (NSArray *)tracksFromPasteboard:(NSPasteboard *)pasteboard
                          context:(NSManagedObjectContext *)context
{
    NSArray *urls = [pasteboard readObjectsForClasses:@[ [NSURL class] ] options:nil ];
    NSMutableArray *tracks = [NSMutableArray array];
    NSMutableArray *fileURLs = [NSMutableArray array];

    // -- First we look for core-data URIs to see if we have any track objects on the pasteboard.
    for (NSURL *url in urls) {
        NSString *desc = url.description;

        // -- If this is not a core data URI then it can't be a Track, therefore we skip it.
        if (![desc hasPrefix:@"x-coredata://"]) {
            continue;
        }

        NSManagedObject *object = [context objectWithURI:url];
        if ([object isKindOfClass:[NxATrack class]]) {
            NxATrack *track = (NxATrack *)object;
            [tracks addObject:track];
        }
        else {
            // -- If we find a URL, we save it in case we need it later.
            [fileURLs addObject:url];
        }
    }

    // -- If we found track objects then we're done and we can return, any extra file URLs on
    // -- the pasteboard are just duplicates of the track objects put there for the finder.
    if (tracks.count) {
        return tracks;
    }

    // -- If we didn't find URLs or URIs then we've got nothing to read from the pasteboard.
    if (!fileURLs.count) {
        return nil;
    }

    // -- If we didn't find any URIs we parse the URLs for track files.
    for (NSURL *url in fileURLs) {
        // -- We find out if the track already exists.
        NxAAudioFile *existingFile = [NxAOSXAudioFile existingFileWithURL:url context:context];
        if (!existingFile.parentTrack) {
            return nil;
        }

        [tracks addObject:existingFile.parentTrack];
    }
    
    return tracks;
}

+ (NSArray *)sortTracks:(NSArray *)tracks
             byTagClass:(Class)tagClass
     withTagDescription:(NSString *)tagDescription
           fromCriteria:(NxASortingCriteria *)criteria
          inParentCrate:(NxACrate *)parentCrate
{
    NSMutableDictionary *matchingTracksByTagValue = [NSMutableDictionary dictionary];
    NSMutableDictionary *tagsByTagValue = [NSMutableDictionary dictionary];
    NSMutableArray *otherTracks = [NSMutableArray array];
    BOOL tagTypeIsBoolean = [tagClass isABooleanTagType];

    for (NxATrack *track in tracks) {
        // -- For each track we look at its tags which match the sorting type.
        NSArray *matchingTags = [tagClass tagsIn:track];

        NxATag *firstTag = matchingTags.firstObject;
        Class firstTagClass = firstTag.class;

        if ([firstTagClass isATagTypeSortedByPropertyName]) {
            // -- If needed, we filter all tags to use only the ones with that property name.
            matchingTags = [NxATag filterTags:matchingTags description:tagDescription];

            // -- Then we update our local variables.
            firstTag = matchingTags.firstObject;
        }

        if (matchingTags.count && !firstTag.isNil) {
            // -- If we still have tags that match and they are not nil, we got through each difference value found for the tag.
            for (NxATag *tag in matchingTags) {
                NSString *tagStringValue = tagTypeIsBoolean ? tagDescription : tag.stringValue;
                NxAAssertNotNil(tagStringValue);

                NSMutableOrderedSet *matchingTracks = matchingTracksByTagValue[tagStringValue];
                if (!matchingTracks) {
                    // -- If we don't have somwehere to store tracks for this value, we create a new set.
                    matchingTracks = [NSMutableOrderedSet orderedSet];
                    matchingTracksByTagValue[tagStringValue] = matchingTracks;
                }

                // -- Then we sort the track by this value.
                [matchingTracks addObject:track];

                // -- And we keep a sample of this tag, to re-use when creating buckets.
                tagsByTagValue[tagStringValue] = tag;
            }
        }
        else {
            [otherTracks addObject:track];
        }
    }

    NSMutableArray *buckets = [NSMutableArray array];

    // -- We sort all the buckets found by description.
    NSSortDescriptor *sortDesc = [[NSSortDescriptor alloc] initWithKey:@"description"
                                                             ascending:YES
                                                              selector:@selector(localizedStandardCompare:)];
    NSArray *sortedValues = [matchingTracksByTagValue.allKeys sortedArrayUsingDescriptors:@[ sortDesc ]];

    // -- Then we create buckets for each different value found.
    for (NSString *tagValue in sortedValues) {
        NxATag *tag = tagsByTagValue[tagValue];
        NSArray *matchingTracks = matchingTracksByTagValue[tagValue];

        NxATrackSortingBucket *bucket = [NxATrackSortingBucket bucketForTracks:matchingTracks
                                                                       withTag:tag
                                                                  fromCriteria:criteria
                                                                   parentCrate:parentCrate];
        [buckets addObject:bucket];
    }

    if (otherTracks.count) {
        // -- If we had tracks that did not fit the criteria, we add a bucket for those at the end.
        NSString *missingDescription = tagTypeIsBoolean ? [NSString stringWithFormat:@"Not %@", tagDescription] : [NSString stringWithFormat:@"No %@", tagDescription];
        NxATrackSortingBucket *bucket = [NxATrackSortingBucket bucketForTracks:otherTracks
                                                               withoutTagClass:tagClass
                                                                tagDescription:tagDescription
                                                             bucketDescription:missingDescription
                                                                  fromCriteria:criteria
                                                                   parentCrate:parentCrate];
        [buckets addObject:bucket];
    }

    return buckets;
}

#pragma mark Accessors

// -- Automatically generated for Core Data properties.
@dynamic p_files;
@dynamic p_parentTrackProperties;

// -- Automatically generated for properties.
@synthesize p_audioData = _p_audioData;

- (NxAAudioFile *)fileForCurrentPlatform
{
    for (NxAAudioFile *file in self.p_files) {
        if (file.isLocal) {
            return file;
        }
    }

    return nil;
}

- (NxAAudioData *)p_audioData
{
    if (!_p_audioData) {
        _p_audioData = [self.fileForCurrentPlatform audioData];
    }

    return _p_audioData;
}

- (NSString *)stringValue
{
    return self.fileForCurrentPlatform.path;
}

- (BOOL)isAnOrphan
{
    return self.p_parentTrackProperties.count == 0;
}

- (NSDate *)dateModified
{
    NxAAssertTrue([self hasNonNilTagsWithType:[NxATrackDateModified tagType]]);
    return [[[NxATrackDateModified tagsIn:self] firstObject] date];
}

#pragma mark Instance Methods

- (void)clearCaches
{
    self.p_audioData = nil;
}

- (void)markAsModifiedOn:(NSDate *)date
{
    [super p_replaceTagClass:[NxATrackDateModified class]
                    withTags:@[ [NxATrackDateModified tagWithValue:date context:self.managedObjectContext] ]
            modificationDate:date];
}

- (void)replaceTagClass:(Class)tagClass
               withTags:(NSArray *)tags
       modificationDate:(NSDate *)date
{
    [super p_replaceTagClass:tagClass withTags:tags modificationDate:date];
    [self markAsModifiedOn:date];
}

#pragma mark Overridden NxATaggedObject Instance Methods

- (void)destroy
{
    if (self.p_parentTrackProperties.count) {
        NSSet *parentTrackPropertiesCopy = self.p_parentTrackProperties.copy;
        [[self mutableSetValueForKey:@"p_parentTrackProperties"] removeAllObjects];

        for (NxATrackProperty *property in parentTrackPropertiesCopy) {
            [property destroy];
        }
    }

    self.p_audioData = nil;

    if (self.p_files.count) {
        NSSet *filesCopy = self.p_files.copy;
        [[self mutableSetValueForKey:@"p_files"] removeAllObjects];

        for (NxAAudioFile *file in filesCopy) {
            [file destroy];
        }
    }

    [super destroy];
}

#pragma mark Overridden NSObject Methods

- (NSString *)description
{
    NSString *trackArtist = nil;
    if ([self hasNonNilTagsWithType:[NxATrackArtistCredit tagType]]) {
        NSArray *artistTags = [NxATrackArtistCredit tagsIn:self];
        if (artistTags.count) {
            trackArtist = [NSString stringSeparatedBy:@", " withStringValuesFromArray:artistTags];
        }
    }

    NSString *trackTitle = nil;
    if ([self hasNonNilTagsWithType:[NxATrackTitle tagType]]) {
        trackTitle = [[[NxATrackTitle tagsIn:self] firstObject] stringValue];
    }

    if (trackTitle) {
        if (trackArtist) {
            return [NSString stringWithFormat:@"%@ by %@", trackTitle, trackArtist];
        }
        else {
            return [NSString stringWithFormat:@"%@ by <MISSING ARTIST>", trackTitle];
        }
    }
    else {
        NxAAudioFile *localFile = self.fileForCurrentPlatform;
        NSString *trackLocation = localFile.description;

        if (trackArtist) {
            if (trackLocation) {
                return [NSString stringWithFormat:@"<MISSING TITLE> by %@ (from file '%@')", trackArtist, trackLocation];
            }
            else {
                return [NSString stringWithFormat:@"<MISSING TITLE> by %@", trackArtist];
            }
        }
        else {
            if (trackLocation) {
                return [NSString stringWithFormat:@"<Track using file '%@'>", trackLocation];
            }
            else {
                return @"<UNKNOWN TRACK>";
            }
        }
    }
}

@end
