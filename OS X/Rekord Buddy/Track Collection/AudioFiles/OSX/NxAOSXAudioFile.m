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

#import "Track Collection/AudioFiles/OSX/Internal/NxAOSXAudioFile.h"
#import "Track Collection/AudioFiles/Internal/NxAAudioFile.h"
#import "NxAMessageLog.h"
#import "NSManagedObjectContext+NxAUtility.h"
#import "NSData+NxAUtility.h"
#import "NxAOSXAudioData.h"
#import "MACAddress.h"

#pragma mark Constants

NSString *const NxAOSXAudioFileID3SizeKey = @"ID3Size";
NSString *const NxAOSXAudioFileID3LengthKey = @"ID3Length";
NSString *const NxAOSXAudioFileID3BitRateKey = @"ID3BitRate";
NSString *const NxAOSXAudioFileID3BitDepthKey = @"ID3BitDepth";
NSString *const NxAOSXAudioFileID3SampleRateKey = @"ID3SampleRate";
NSString *const NxAOSXAudioFileID3CoverArtKey = @"ID3CoverArt";

#pragma mark Static Variables

/*! Hard coded boot volume name used for stubbing during unit tests. */
static NSString *p_NxAOSXAudioFileTestingBootVolumeName = nil;

/*! Hard coded names of other volumes used for stubbing during unit tests. */
static NSSet *p_NxAOSXAudioFileTestingOtherVolumeNames = nil;

/*! Hard coded file manager used for stubbing during unit tests. */
static NSFileManager *p_NxAOSXAudioFileTestingFileManager = nil;

#pragma mark - Implementation

@implementation NxAOSXAudioFile

#pragma mark Factory Methods

+ (instancetype)p_fileInContext:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(context);

    NxAOSXAudioFile *newFile = [context insertNewObjectForEntity:@"OSXAudioFile"];
    
    newFile.p_deviceID = [NxAOSXAudioFile p_deviceID];

    return newFile;
}

+ (instancetype)fileWithURL:(NSURL *)url
                    context:(NSManagedObjectContext *)context
{
    NSData *bookmark = [url bookmarkDataWithOptions:0
                     includingResourceValuesForKeys:nil
                                      relativeToURL:nil
                                              error:nil];
    NxAAssertNotNil(bookmark);

    // -- Add a new audio file to the context.
    NxAOSXAudioFile *newFile = [NxAOSXAudioFile p_fileInContext:context];
    NxAAssertNotNil(newFile);

    newFile.p_bookmark = bookmark;
    newFile.p_path = url.path;

    return newFile;
}

#pragma mark Class Methods

/*!
 * Get a unique Device ID for this computer
 * \return The Device ID string.
 */
+ (NSString *)p_deviceID
{
    static NSString *macAddress = nil;
    static dispatch_once_t onceToken = 0;
    dispatch_once(&onceToken, ^{
        // -- The device ID if prefixed with the platform name and ends with the MAC address of the first interface.
        macAddress = [NSString stringWithFormat:@"OSX:%@", [MACAddress address]];
    });
    
    return macAddress;
}

+ (void)p_setTestingOtherVolumeNames:(NSSet *)names
{
    p_NxAOSXAudioFileTestingOtherVolumeNames = names;
}

+ (void)p_setTestingBootVolumeName:(NSString *)name
{
    p_NxAOSXAudioFileTestingBootVolumeName = name;
}

+ (void)p_setTestingFileManager:(NSFileManager *)manager
{
    p_NxAOSXAudioFileTestingFileManager = manager;
}

+ (NSString *)bootVolumeName
{
    if (p_NxAOSXAudioFileTestingBootVolumeName) {
        // -- If we're in the middle of a unit test, we use the stubbed data.
        return p_NxAOSXAudioFileTestingBootVolumeName;
    }

    static NSString *_bootVolumeName = nil;
    static dispatch_once_t onceToken = 0;
    dispatch_once(&onceToken, ^{
        NSFileManager *fileManager = [NSFileManager defaultManager];

        // -- Get a list URLS for all the mounted volumes with also their names.
        NSArray *mountedVolumes = [fileManager mountedVolumeURLsIncludingResourceValuesForKeys:@[ NSURLNameKey ]
                                                                                       options:NSVolumeEnumerationSkipHiddenVolumes];
        NxAAssertTrue(mountedVolumes.count != 0);

        // -- We only care about the first one which should be the boot volume.
        NSURL *bootVolumeURL = mountedVolumes[0];

        // -- Get its name.
        NSString *name;
        BOOL returnValue = [bootVolumeURL getResourceValue:&name
                                                    forKey:NSURLNameKey
                                                     error:nil];
        NxAAssertTrue(returnValue);
        NxAAssertNotNil(name);

        _bootVolumeName = name;
    });

    return _bootVolumeName;
}

+ (NSSet *)otherVolumeNames
{
    if (p_NxAOSXAudioFileTestingOtherVolumeNames) {
        // -- If we're in the middle of a unit test, we use the stubbed data.
        return p_NxAOSXAudioFileTestingOtherVolumeNames;
    }

    static NSSet *_otherVolumeNames = nil;
    static dispatch_once_t onceToken = 0;
    dispatch_once(&onceToken, ^{
        NSFileManager *fileManager = [NSFileManager defaultManager];

        // -- Get a list URLS for all the mounted volumes with also their names.
        NSArray *mountedVolumes = [fileManager mountedVolumeURLsIncludingResourceValuesForKeys:@[ NSURLNameKey ]
                                                                                       options:NSVolumeEnumerationSkipHiddenVolumes];
        NxAAssertTrue(mountedVolumes.count != 0);

        NSMutableSet *names = [NSMutableSet set];

        for (NSUInteger index = 1; index < mountedVolumes.count; ++index) {
            NSString *name;
            BOOL returnValue = [mountedVolumes[index] getResourceValue:&name
                                                                forKey:NSURLNameKey
                                                                 error:nil];
            NxAAssertTrue(returnValue);
            NxAAssertNotNil(name);

            [names addObject:name];
        }

        _otherVolumeNames = names;
    });
    
    return _otherVolumeNames;
}

+ (NSFileManager *)fileManager
{
    if (p_NxAOSXAudioFileTestingFileManager) {
        return p_NxAOSXAudioFileTestingFileManager;
    }

    return [NSFileManager defaultManager];
}

+ (NxAOSXAudioFile *)existingFileWithURL:(NSURL *)url
                                 context:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(url);

    // -- Look for an existing ID/Path combination.
    NSArray *results = [context fetchFromTemplate:@"FileOnDeviceWithPath"
                            substitutionVariables:@{ @"PATH" : url.path,
                                                     @"DEVICEID" : [NxAOSXAudioFile p_deviceID] }];
    if (!results) {
        return nil;
    }

    if (results.count == 0) {
        // -- If we didn't find any results then we don't have already an existing file.
        return nil;
    }

    NxAAssertTrue(results.count == 1);

    return results[0];
}

#pragma mark Accessors

// -- Automatically generated for Core Data properties.
@dynamic p_bookmark;
@dynamic p_path;

- (NSURL *)url
{
    BOOL stale = NO;
    NSError *error = nil;

    NSData *bookmark = self.p_bookmark;
    NSURL *url = [bookmark URLByResolvingBookmarkDataWithOptions:0
                                                   relativeToURL:nil
                                             bookmarkDataIsStale:&stale
                                                           error:&error];
    if (!url) {
        if (stale) {
            NSLog(@"Stale bookmark for '%@'.", url.path);
        }
        else {
            NSLog(@"Error resolving bookmark for '%@'.\n%@", url.path, error);
        }
    }

    return url;
}

- (NxAAudioData *)audioData
{
    NxAOSXAudioData *data = [NxAOSXAudioData dataWithContentsOfURL:self.url sampleRate:44100 channels:2];
    return data;
}

#pragma mark Overrissen NxAAudioFile Accessors

- (BOOL)isLocal
{
    NSString *deviceID = [NxAOSXAudioFile p_deviceID];
    return [self.p_deviceID isEqualToString:deviceID];
}

- (NSString *)path
{
    return self.url.path;
}

#pragma mark Instance Methods

- (NSDate *)modificationDate
{
    NSURL *url = self.url;
    NxAAssertNotNil(url);

    NSFileManager *fileManager = [NxAOSXAudioFile fileManager];

    NSDictionary *attributes = [fileManager attributesOfItemAtPath:url.path
                                                             error:nil];
    NxAAssertNotNil(attributes);

    NSDate *fileModificationDate = attributes.fileModificationDate;
    return fileModificationDate;
}

#pragma mark Overridden NxAAudioFile Methods

- (void)addURLToPasteboardArray:(NSMutableArray *)array
{
    NSURL *url = self.url;
    if (url) {
        [array addObject:url];
    }
}

#pragma mark Overridden NSObject Methods

- (NSString *)description
{
    return self.url.path;
}

@end
