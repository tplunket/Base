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

#include <SeratoDb/TrackFiles/TrackFileFactory.hpp>
#include <SeratoDb/TrackFiles/TrackFile.hpp>

#import "Track Collection/Tags/NxATag.h"
#import "NxATagEntry.h"
#import "NxAID3Parser.h"
#import "NxATrackBitRate.h"
#import "NxATrackSampleRate.h"
#import "NxATrackSize.h"
#import "NxATrackLength.h"
#import "NxATrackBitDepth.h"
#import "NxAArtwork.h"
#import "Track Collection/Tags/Properties/NxAArtworkProperty.h"
#import "Track Collection/AudioFiles/OSX/NxAOSXAudioFile.h"
#import "Track Collection/Categories/Parsing/Internal/NxATaggedObject+Parsing.h"
#import "Track Collection/Internal/NxATrack.h"
#import "NxATrack+NxAOSXAudioFile.h"
#import "NxAParsedIntegerValue.h"
#import "NxAParsedArtworkValue.h"

using namespace NxA;

#pragma mark Debug Defines

// -- This flag can be used to stop in the debugger while reading a specific track.
#define NXADebugBreakAtSpecificTrack                            0
#define NxADebugBreakAtTrackFilenameContains                    @""

#pragma mark - Implementation

@implementation NxATrack (ID3Parsing)

#pragma mark Instance Methods

- (NxAParsingResultFlags)parseID3TagsInAudioFileWithParser:(id<NxAParserImplementation>)parser
                                          forceToBeUpdated:(BOOL)forceToBeUpdated
{
    NxAAudioFile *audioFile = self.fileForCurrentPlatform;
    NxAAssertTrue([audioFile isKindOfClass:[NxAOSXAudioFile class]]);

    NxAOSXAudioFile *file = (NxAOSXAudioFile *)audioFile;
    NSDate *fileModificationDate = forceToBeUpdated ? [NSDate distantPast] : file.modificationDate;
    __block Serato::TrackFile::Pointer *trackFile = nullptr;

#if NXADebugBreakAtSpecificTrack
    NxALaunchDebuggerIf([file.url.path containsString:NxADebugBreakAtTrackFilenameContains]);
#endif

    NSInteger result = NxAParsedObjectWasUpToDate;

    result |= [self p_replaceTagsOfClass:[NxATrackBitRate class]
                             ifOlderThan:fileModificationDate
                andParsedValuesFromBlock:^() {
                    if (!trackFile) {
                        trackFile = new Serato::TrackFile::Pointer(Serato::TrackFileFactory::trackFileForPath(String::stringWith(file.url.path.UTF8String)));
                    }

                    uinteger32 bitRate = (*trackFile)->bitRateInKiloBitsPerSecond();
                    if (bitRate) {
                        return @[ [NxAParsedIntegerValue parsedValueWithInteger:bitRate] ];
                    }
                    else {
                        return @[ [NxAParsedIntegerValue parsedValueWithNoInteger] ];
                    }
                }
        areDifferentThanPreviouslyParsedWith:parser
     thenCallThisEveryTimeATagIsReplaced:nil];

    result |= [self p_replaceTagsOfClass:[NxATrackSampleRate class]
                             ifOlderThan:fileModificationDate
                andParsedValuesFromBlock:^() {
                    if (!trackFile) {
                        trackFile = new Serato::TrackFile::Pointer(Serato::TrackFileFactory::trackFileForPath(String::stringWith(file.url.path.UTF8String)));
                    }

                    uinteger32 sampleRate = (*trackFile)->sampleRateInSamplesPerSecond();
                    if (sampleRate) {
                        return @[ [NxAParsedIntegerValue parsedValueWithInteger:sampleRate] ];
                    }
                    else {
                        return @[ [NxAParsedIntegerValue parsedValueWithNoInteger] ];
                    }
                }
        areDifferentThanPreviouslyParsedWith:parser
     thenCallThisEveryTimeATagIsReplaced:nil];

    result |= [self p_replaceTagsOfClass:[NxATrackSize class]
                             ifOlderThan:fileModificationDate
                andParsedValuesFromBlock:^() {
                    if (!trackFile) {
                        trackFile = new Serato::TrackFile::Pointer(Serato::TrackFileFactory::trackFileForPath(String::stringWith(file.url.path.UTF8String)));
                    }

                    count size = (*trackFile)->size();
                    if (size) {
                        return @[ [NxAParsedIntegerValue parsedValueWithInteger:size] ];
                    }
                    else {
                        return @[ [NxAParsedIntegerValue parsedValueWithNoInteger] ];
                    }
                }
        areDifferentThanPreviouslyParsedWith:parser
     thenCallThisEveryTimeATagIsReplaced:nil];

    result |= [self p_replaceTagsOfClass:[NxATrackBitDepth class]
                             ifOlderThan:fileModificationDate
                andParsedValuesFromBlock:^() {
                    if (!trackFile) {
                        trackFile = new Serato::TrackFile::Pointer(Serato::TrackFileFactory::trackFileForPath(String::stringWith(file.url.path.UTF8String)));
                    }

                    if ((*trackFile)->hasBitDepth()) {
                        return @[ [NxAParsedIntegerValue parsedValueWithInteger:(*trackFile)->bitDepthInBits()] ];
                    }
                    else {
                        return @[ [NxAParsedIntegerValue parsedValueWithNoInteger] ];
                    }
                }
        areDifferentThanPreviouslyParsedWith:parser
     thenCallThisEveryTimeATagIsReplaced:nil];

    result |= [self p_replaceTagsOfClass:[NxATrackLength class]
                             ifOlderThan:fileModificationDate
                andParsedValuesFromBlock:^() {
                    if (!trackFile) {
                        trackFile = new Serato::TrackFile::Pointer(Serato::TrackFileFactory::trackFileForPath(String::stringWith(file.url.path.UTF8String)));
                    }

                    uinteger32 length = (*trackFile)->lengthInMilliseconds();
                    if (length) {
                        return @[ [NxAParsedIntegerValue parsedValueWithInteger:length] ];
                    }
                    else {
                        return @[ [NxAParsedIntegerValue parsedValueWithNoInteger] ];
                    }
                }
        areDifferentThanPreviouslyParsedWith:parser
     thenCallThisEveryTimeATagIsReplaced:nil];

    result |= [self p_replaceTagsOfClass:[NxAArtworkProperty class]
                             ifOlderThan:fileModificationDate
                andParsedValuesFromBlock:^() {
                    if (!trackFile) {
                        trackFile = new Serato::TrackFile::Pointer(Serato::TrackFileFactory::trackFileForPath(String::stringWith(file.url.path.UTF8String)));
                    }

                    Blob::Pointer artwork = (*trackFile)->artwork();
                    if (artwork->size()) {
                        NSData *artworkData = [NSData dataWithBytes:artwork->data() length:artwork->size()];
                        NSImage *coverArt = [[NSImage alloc] initWithData:artworkData];
                        return @[ [NxAParsedArtworkValue parsedValueWithImage:coverArt] ];
                    }
                    else {
                        return @[ [NxAParsedArtworkValue parsedValueWithNoImage] ];
                    }
                }
        areDifferentThanPreviouslyParsedWith:parser
     thenCallThisEveryTimeATagIsReplaced:nil];

    if (trackFile) {
        delete trackFile;
    }

    // -- TODO: ID3 audio properties are not being written back out anywhere yet so we clear the result flag for now.
    result = result;
    result = NxAParsedObjectWasUpToDate;

    return (NxAParsingResultFlags)result;
}

@end
