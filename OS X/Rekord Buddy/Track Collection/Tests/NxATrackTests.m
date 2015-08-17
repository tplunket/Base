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

#import "NxATrackTests.h"
#import "NxAOSXAudioFileTests.h"
#import "Track Collection/Internal/NxATrack.h"
#import "NxATrackTitle.h"
#import "Track Collection/Tags/Track Properties/NxATrackKey.h"
#import "NxATrackGenre.h"
#import "NxATrackComments.h"
#import "NxATrackNumber.h"
#import "NxATrackRating.h"
#import "NxATrackPlayCount.h"
#import "NxATrackRecordLabel.h"
#import "Track Collection/Tags/Track Properties/NxATrackAlbum.h"
#import "NxATrackMixName.h"
#import "NxATrackArtistCredit.h"
#import "NxATrackRemixerCredit.h"
#import "NxATrackProducerCredit.h"
#import "Track Collection/Tags/Marker Properties/NxAGridMarkerProperty.h"
#import "Track Collection/Tags/Marker Properties/Internal/NxAMarkerProperty.h"
#import "Track Collection/Tags/Marker Properties/Internal/NxAGridMarkerProperty.h"
#import "NxATrackDateAdded.h"
#import "NxATrackDateReleased.h"
#import "NxATrackTag.h"
#import "NxATrack+NxAOSXAudioFile.h"
#import "NSDate+NxAUtility.h"
#import "NxATrack+Parsing.h"

#pragma mark Implementation

@implementation NxATrackTests

#pragma mark Class Methods

+ (NxATrack *)emptyTestTrackInContext:(NSManagedObjectContext *)context
                         usingKeyName:(NSString *)keyName
                       lastModifiedOn:(NSDate *)modificationDate
{
    NxATrack *track = [NxATrackTests stubbedTrackWithPath:@"/Users/didier/Music/Gigs/25 I'm In Love (feat. Donae'O) (Deep Mix).m4a" context:context];
    [track ifNotAlreadyPresentAddNilTagsForTagClass:[NxATrackTitle class] createdOnDate:modificationDate];
    [track ifNotAlreadyPresentAddNilTagsForTagClass:[NxATrackArtistCredit class] createdOnDate:modificationDate];
    [track ifNotAlreadyPresentAddNilTagsForTagClass:[NxATrackProducerCredit class] createdOnDate:modificationDate];
    [track ifNotAlreadyPresentAddNilTagsForTagClass:[NxATrackRemixerCredit class] createdOnDate:modificationDate];
    [track ifNotAlreadyPresentAddNilTagsForTagClass:[NxATrackGenre class] createdOnDate:modificationDate];
    [track ifNotAlreadyPresentAddNilTagsForTagClass:[NxATrackMixName class] createdOnDate:modificationDate];
    [track ifNotAlreadyPresentAddNilTagsForTagClass:[NxATrackComments class] createdOnDate:modificationDate];
    [track ifNotAlreadyPresentAddNilTagsForTagClass:[NxATrackRecordLabel class] createdOnDate:modificationDate];
    [track ifNotAlreadyPresentAddNilTagsForTagClass:[NxATrackDateAdded class] createdOnDate:modificationDate];
    [track ifNotAlreadyPresentAddNilTagsForTagClass:[NxATrackRating class] createdOnDate:modificationDate];
    [track ifNotAlreadyPresentAddNilTagsForTagClass:[NxATrackPlayCount class] createdOnDate:modificationDate];
    [track ifNotAlreadyPresentAddNilTagsForTagClass:[NxATrackNumber class] createdOnDate:modificationDate];
    [track ifNotAlreadyPresentAddNilTagsForTagClass:[NxATrackAlbum class] createdOnDate:modificationDate];
    [track ifNotAlreadyPresentAddNilTagsForTagClass:[NxATrackDateReleased class] createdOnDate:modificationDate];
    [track ifNotAlreadyPresentAddNilTagsForTagClass:[NxATrackTag class] createdOnDate:modificationDate];
    [track ifNotAlreadyPresentAddNilTagsForTagClass:[NxACueMarkerProperty class] createdOnDate:modificationDate];
    [track ifNotAlreadyPresentAddNilTagsForTagClass:[NxATrackKey class] createdOnDate:modificationDate];
    [track markAsModifiedOn:modificationDate];

    NxAAssertNotNil(track);
    return track;
}

+ (NxATrack *)testTrack1InContext:(NSManagedObjectContext *)context
                          keyName:(NSString *)keyName
                 modificationDate:(NSDate *)modificationDate
{
    NxATrack *track = [NxATrackTests stubbedTrackWithPath:@"/Users/didier/Music/Gigs/25 I'm In Love (feat. Donae'O) (Deep Mix).m4a" context:context];
    [track p_replaceTagClass:[NxATrackTitle class]
                    withTags:@[ [NxATrackTitle tagWithValue:@"I'm In Love (Deep Mix)" context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackArtistCredit class]
                    withTags:@[ [NxATrackArtistCredit tagWithValue:@"Artful" context:context],
                                [NxATrackArtistCredit tagWithValue:@"Donae'O" context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackProducerCredit class]
                    withTags:@[ [NxATrackProducerCredit tagWithValue:@"My Digital Enemy" context:context],
                                [NxATrackProducerCredit tagWithValue:@"Pleasurekraft" context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackRemixerCredit class]
                    withTags:@[ [NxATrackRemixerCredit tagWithValue:@"Third Party" context:context],
                                [NxATrackRemixerCredit tagWithValue:@"Rick James" context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackGenre class]
                    withTags:@[ [NxATrackGenre tagWithValue:@"House" context:context],
                                [NxATrackGenre tagWithValue:@"Progressive House" context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackMixName class]
                    withTags:@[ [NxATrackMixName tagWithValue:@"Original Mix" context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackComments class]
                    withTags:@[ [NxATrackComments tagWithValue:@"Some snazzy comments!" context:context ] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackRecordLabel class]
                    withTags:@[ [NxATrackRecordLabel tagWithValue:@"Size Records" context:context ] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackDateAdded class]
                    withTags:@[ [NxATrackDateAdded  tagWithValue:[NSDate dateWithStringInLocalTimeZone:@"2014-06-17 07:00:00" format:@"yyyy-MM-dd HH:mm:ss"]
                                                         context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackRating class]
                    withTags:@[ [NxATrackRating tagWithValue:@(3) context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackPlayCount class]
                    withTags:@[ [NxATrackPlayCount tagWithValue:@(142) context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackNumber class]
                    withTags:@[ [NxATrackNumber tagWithValue:@(25) context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackAlbum class]
                    withTags:@[ [NxATrackAlbum tagWithValue:@"Defected Presents Dimitri from Paris In the House of Disco" context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackDateReleased class]
                    withTags:@[ [NxATrackDateReleased tagWithValue:[NSDate dateWithStringInLocalTimeZone:@"2012-03-23 07:00:00" format:@"yyyy-MM-dd HH:mm:ss"]
                                                           context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackTag class]
                    withTags:@[ [NxATrackTag tagWithValue:@"Awesome" context:context],
                                [NxATrackTag tagWithValue:@"Play:Now" context:context] ]
            modificationDate:modificationDate];
    [track markAsModifiedOn:modificationDate];

    NSArray *markers = @[ [NxAGridMarkerProperty gridMarkerAtPosition:[NSDecimalNumber decimalNumberWithString:@"0.068"]
                                                                  bpm:[NSDecimalNumber decimalNumberWithString:@"124.0"]
                                                          customFlags:@[ ]
                                                              context:context],
                          [NxACueMarkerProperty cueMarkerWithName:@"Test2"
                                                         position:[NSDecimalNumber decimalNumberWithString:@"0.068"]
                                                           hotCue:@(0)
                                                      customFlags:@[ ]
                                                          context:context],
                          [NxACueMarkerProperty cueMarkerWithName:@"Test3"
                                                         position:[NSDecimalNumber decimalNumberWithString:@"31.036"]
                                                           hotCue:@(1)
                                                      customFlags:@[ ]
                                                          context:context],
                          [NxAGridMarkerProperty gridMarkerAtPosition:[NSDecimalNumber decimalNumberWithString:@"50.068"]
                                                                  bpm:[NSDecimalNumber decimalNumberWithString:@"112.0"]
                                                          customFlags:@[ ]
                                                              context:context],
                          [NxACueMarkerProperty cueMarkerWithName:@"Test5"
                                                         position:[NSDecimalNumber decimalNumberWithString:@"62.004"]
                                                      customFlags:@[ ]
                                                          context:context],
                          [NxAGridMarkerProperty gridMarkerAtPosition:[NSDecimalNumber decimalNumberWithString:@"200.068"]
                                                                  bpm:[NSDecimalNumber decimalNumberWithString:@"102.0"]
                                                          customFlags:@[ ]
                                                              context:context],
                          [NxACueMarkerProperty cueMarkerWithName:@"Test7"
                                                         position:[NSDecimalNumber decimalNumberWithString:@"371.681"]
                                                      customFlags:@[ ]
                                                          context:context]
                          ];

    [track replaceTagClass:[NxAMarkerProperty class] withTags:markers modificationDate:modificationDate];

    NxATrackKey *keyTag = [NxATrackKey tagWithName:keyName
                                             value:[NxATrackKey keyValueForString:@"Fm"]
                                           context:context];
    NxAAssertNotNil(keyTag);
    [track p_replaceTagClass:[NxATrackKey class]
                     andName:keyName
                    withTags:@[ keyTag ]
            modificationDate:modificationDate];

    NxAAssertNotNil(track);

    return track;
}

+ (NxATrack *)testTrack2InContext:(NSManagedObjectContext *)context
                          keyName:(NSString *)keyName
                 modificationDate:(NSDate *)modificationDate
{
    NxATrack *track = [NxATrackTests stubbedTrackWithPath:@"/Users/didier/Music/Gigs/Need U (100%) feat. A*M*E (Original Mix) - A*M*E, Duke Dumont.aiff" context:context];
    [track p_replaceTagClass:[NxATrackTitle class]
                    withTags:@[ [NxATrackTitle tagWithValue:@"Need U (100%) (Original Mix)" context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackArtistCredit class]
                    withTags:@[ [NxATrackArtistCredit tagWithValue:@"Duke Dumont" context:context],
                                [NxATrackArtistCredit tagWithValue:@"A*M*E" context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackProducerCredit class]
                    withTags:@[ [NxATrackProducerCredit tagWithValue:@"Steve Angello" context:context],
                                [NxATrackProducerCredit tagWithValue:@"Roger Sanchez" context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackRemixerCredit class]
                    withTags:@[ [NxATrackRemixerCredit tagWithValue:@"Arty" context:context],
                                [NxATrackRemixerCredit tagWithValue:@"Bingo Players" context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackGenre class]
                    withTags:@[ [NxATrackGenre tagWithValue:@"Deep House" context:context],
                                [NxATrackGenre tagWithValue:@"Tech House" context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackMixName class]
                    withTags:@[ [NxATrackMixName tagWithValue:@"Club Mix" context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackComments class]
                    withTags:@[ [NxATrackComments tagWithValue:@"Some even better comments." context:context ] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackRecordLabel class]
                    withTags:@[ [NxATrackRecordLabel tagWithValue:@"Win Records/Downtown Records" context:context ] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackDateAdded class]
                    withTags:@[ [NxATrackDateAdded  tagWithValue:[NSDate dateWithStringInLocalTimeZone:@"2014-09-10 07:00:00" format:@"yyyy-MM-dd HH:mm:ss"]
                                                         context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackRating class]
                    withTags:@[ [NxATrackRating tagWithValue:@(5) context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackPlayCount class]
                    withTags:@[ [NxATrackPlayCount tagWithValue:@(4243) context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackNumber class]
                    withTags:@[ [NxATrackNumber tagWithValue:@(2) context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackAlbum class]
                    withTags:@[ [NxATrackAlbum tagWithValue:@"Need U (100%) [feat. A*M*E]" context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackDateReleased class]
                    withTags:@[ [NxATrackDateReleased tagWithValue:[NSDate dateWithStringInLocalTimeZone:@"2012-03-23 07:00:00" format:@"yyyy-MM-dd HH:mm:ss"]
                                                           context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackTag class]
                    withTags:@[ [NxATrackTag tagWithValue:@"Even Better" context:context],
                                [NxATrackTag tagWithValue:@"Play:Later" context:context] ]
            modificationDate:modificationDate];
    [track markAsModifiedOn:modificationDate];

    NSArray *markers = @[[NxAGridMarkerProperty gridMarkerAtPosition:[NSDecimalNumber decimalNumberWithString:@"0.009"]
                                                                 bpm:[NSDecimalNumber decimalNumberWithString:@"120.0"]
                                                         customFlags:@[ ]
                                                             context:context],
                         [NxACueMarkerProperty cueMarkerWithName:@"OtherTest2"
                                                        position:[NSDecimalNumber decimalNumberWithString:@"0.009"]
                                                          hotCue:@(0)
                                                     customFlags:@[ ]
                                                         context:context],
                         [NxACueMarkerProperty cueMarkerWithName:@"OtherTest3"
                                                        position:[NSDecimalNumber decimalNumberWithString:@"31.428"]
                                                          hotCue:@(1)
                                                     customFlags:@[ ]
                                                         context:context],
                         [NxACueMarkerProperty cueMarkerWithName:@"OtherTest4"
                                                        position:[NSDecimalNumber decimalNumberWithString:@"62.647"]
                                                     customFlags:@[ ]
                                                         context:context],
                         [NxACueMarkerProperty cueMarkerWithName:@"OtherTest5"
                                                        position:[NSDecimalNumber decimalNumberWithString:@"406.062"]
                                                     customFlags:@[ ]
                                                         context:context]
                         ];
    [track replaceTagClass:[NxAMarkerProperty class] withTags:markers modificationDate:modificationDate];

    NxATrackKey *keyTag = [NxATrackKey tagWithName:keyName
                                             value:[NxATrackKey keyValueForString:@"Cm"]
                                           context:context];
    NxAAssertNotNil(keyTag);
    [track p_replaceTagClass:[NxATrackKey class]
                     andName:keyName
                    withTags:@[ keyTag ]
            modificationDate:modificationDate];

    NxAAssertNotNil(track);

    return track;
}

+ (NxATrack *)testTrack3InContext:(NSManagedObjectContext *)context
                          keyName:(NSString *)keyName
                 modificationDate:(NSDate *)modificationDate
{
    NxATrack *track = [NxATrackTests stubbedTrackWithPath:@"/Users/didier/Music/Gigs/Terra (Original Mix) - Manuel De La Mare and Luigi Rocca.aiff" context:context];
    [track p_replaceTagClass:[NxATrackTitle class]
                    withTags:@[ [NxATrackTitle tagWithValue:@"Terra (Original Mix)" context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackArtistCredit class]
                    withTags:@[ [NxATrackArtistCredit tagWithValue:@"Manuel De La Mare" context:context],
                                [NxATrackArtistCredit tagWithValue:@"Luigi Rocca" context:context] ]
            modificationDate:modificationDate];
    [track ifNotAlreadyPresentAddNilTagsForTagClass:[NxATrackProducerCredit class] createdOnDate:modificationDate];
    [track ifNotAlreadyPresentAddNilTagsForTagClass:[NxATrackRemixerCredit class] createdOnDate:modificationDate];
    [track p_replaceTagClass:[NxATrackGenre class]
                    withTags:@[ [NxATrackGenre tagWithValue:@"Tech House" context:context] ]
            modificationDate:modificationDate];
    [track ifNotAlreadyPresentAddNilTagsForTagClass:[NxATrackMixName class] createdOnDate:modificationDate];
    [track ifNotAlreadyPresentAddNilTagsForTagClass:[NxATrackComments class] createdOnDate:modificationDate];
    [track p_replaceTagClass:[NxATrackRecordLabel class]
                    withTags:@[ [NxATrackRecordLabel tagWithValue:@"303Lovers" context:context ] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackDateAdded class]
                    withTags:@[ [NxATrackDateAdded  tagWithValue:[NSDate dateWithStringInLocalTimeZone:@"2013-11-28 07:00:00" format:@"yyyy-MM-dd HH:mm:ss"]
                                                         context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackRating class]
                    withTags:@[ [NxATrackRating tagWithValue:@(0) context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackPlayCount class]
                    withTags:@[ [NxATrackPlayCount tagWithValue:@(0) context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackNumber class]
                    withTags:@[ [NxATrackNumber tagWithValue:@(1) context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackAlbum class]
                    withTags:@[ [NxATrackAlbum tagWithValue:@"Terra" context:context] ]
            modificationDate:modificationDate];
    [track p_replaceTagClass:[NxATrackDateReleased class]
                    withTags:@[ [NxATrackDateReleased tagWithValue:[NSDate dateWithStringInLocalTimeZone:@"2013-10-09 07:00:00" format:@"yyyy-MM-dd HH:mm:ss"]
                                                           context:context] ]
            modificationDate:modificationDate];
    [track ifNotAlreadyPresentAddNilTagsForTagClass:[NxATrackTag class] createdOnDate:modificationDate];
    [track markAsModifiedOn:modificationDate];

    NxAMarkerProperty *secondMarker = [NxACueMarkerProperty cueMarkerWithName:@"ThirdTest2"
                                                                     position:[NSDecimalNumber decimalNumberWithString:@"0.4845890"]
                                                                       hotCue:@(0)
                                                                  customFlags:@[ NxAMarkerIsLoadMarker ]
                                                                      context:context];
    NxAMarkerProperty *fifthMarker = [NxACueMarkerProperty cueMarkerWithName:@"ThirdTest5"
                                                                    position:[NSDecimalNumber decimalNumberWithString:@"93.3878"]
                                                                 customFlags:@[ NxAMarkerIsFadeInMarker ]
                                                                     context:context];
    NxAMarkerProperty *sixthMarker = [NxACueMarkerProperty cueMarkerWithName:@"ThirdTest6"
                                                                    position:[NSDecimalNumber decimalNumberWithString:@"341.130"]
                                                                 customFlags:@[ NxAMarkerIsFadeOutMarker ]
                                                                     context:context];

    NSArray *markers = @[[NxAGridMarkerProperty gridMarkerAtPosition:[NSDecimalNumber decimalNumberWithString:@"0.4845890"]
                                                               bpm:[NSDecimalNumber decimalNumberWithString:@"124.0"]
                                                         customFlags:@[ ]
                                                           context:context],
                         secondMarker,
                         [NxACueMarkerProperty cueMarkerWithName:@"ThirdTest3"
                                                        position:[NSDecimalNumber decimalNumberWithString:@"31.4523"]
                                                          hotCue:@(1)
                                                     customFlags:@[ ]
                                                         context:context],
                         [NxACueMarkerProperty cueMarkerWithName:@"ThirdTest4"
                                                        position:[NSDecimalNumber decimalNumberWithString:@"62.4201"]
                                                          hotCue:@(2)
                                                     customFlags:@[ ]
                                                         context:context],
                         fifthMarker,
                         sixthMarker
                         ];
    [track replaceTagClass:[NxAMarkerProperty class] withTags:markers modificationDate:modificationDate];

    NxATrackKey *keyTag = [NxATrackKey tagWithName:keyName
                                             value:[NxATrackKey keyValueForString:@"12m"]
                                           context:context];
    NxAAssertNotNil(keyTag);
    [track p_replaceTagClass:[NxATrackKey class]
                     andName:keyName
                    withTags:@[ keyTag ]
            modificationDate:modificationDate];

    NxAAssertNotNil(track);

    return track;
}

+ (NxATrack *)stubbedTrackWithPath:(NSString *)path
                           context:(NSManagedObjectContext *)context;
{
    NxAOSXAudioFile *stubFile = [NxAOSXAudioFileTests stubbedOSXAudioFileWithPath:path context:context];
    NxATrack *stubbedTrack = [NxATrack trackFromOSXAudioFile:stubFile
                                                     context:context];
    NxAAssertNotNil(stubbedTrack);

    return stubbedTrack;
}

@end
