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

@class NxAMessageLog;

#import "NxAAudioFile.h"
#import "NxATrack.h"

#pragma mark Constants

/*!
 * Keys used to store ID3 tags information from an audio file.
 */
extern NSString *const NxAOSXAudioFileID3SizeKey;
extern NSString *const NxAOSXAudioFileID3LengthKey;
extern NSString *const NxAOSXAudioFileID3BitRateKey;
extern NSString *const NxAOSXAudioFileID3BitDepthKey;
extern NSString *const NxAOSXAudioFileID3SampleRateKey;
extern NSString *const NxAOSXAudioFileID3CoverArtKey;

#pragma mark - Public Interface

/*!
 * Audio file for a track on an OS X computer.
 * \attention This is an abstract class which should never be instantiated directly.
 */
@interface NxAOSXAudioFile : NxAAudioFile

#pragma mark Factory Methods

/*!
 * Create a new existing audio file from a file's path.
 * \details Supported file formats are AAC, AIFF, FLAC, MP3, OGG VORBIS or WAV.
 *          File has to be located in one of the user's music folder for the url to be valid.
 * \param url URL of an audio file.
 * \param context Managed object context to create the new object in.
 * \return An audio file pointing to the file or nil if creation failed.
 */
+ (instancetype)fileWithURL:(NSURL *)url
                    context:(NSManagedObjectContext *)context;

#pragma mark Class Methods

+ (NSString *)bootVolumeName;

+ (NSSet *)otherVolumeNames;

+ (NSFileManager *)fileManager;

/*!
 * Find any existing file given a path.
 * \param url URL to look for.
 * \param context Managed object context to create the new object in.
 * \return A file if it exists or nil otherwise.
 */
+ (NxAOSXAudioFile *)existingFileWithURL:(NSURL *)url
                                 context:(NSManagedObjectContext *)context;

#pragma mark Properties

@property (readonly, nonatomic) NSURL *url;

#pragma mark Instance Methods

/*!
 * Get the audio file's modification date.
 * \return The file's modification date.
 */
- (NSDate *)modificationDate;

@end
