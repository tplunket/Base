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

#import "NSFileManager+NxAUtility.h"

#pragma mark Implementation

@implementation NSFileManager (NxAUtility)

#pragma mark Class Methods

+ (NSString *)pathForTemporaryFileWithPrefix:(NSString *)prefix
{
    NSString *  result;
    CFUUIDRef   uuid;
    CFStringRef uuidStr;
    
    uuid = CFUUIDCreate(NULL);
    assert(uuid != NULL);
    
    uuidStr = CFUUIDCreateString(NULL, uuid);
    assert(uuidStr != NULL);
    
    result = [NSTemporaryDirectory() stringByAppendingPathComponent:[NSString stringWithFormat:@"%@-%@", prefix, uuidStr]];
    assert(result != nil);
    
    CFRelease(uuidStr);
    CFRelease(uuid);
    
    return result;
}

+ (NSDate *)modificationDateForFileAtURL:(NSURL *)url
{
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSDictionary *attributes = [fileManager attributesOfItemAtPath:url.path
                                                             error:nil];
    if (!attributes) {
        return nil;
    }

    return attributes.fileModificationDate;
}

#pragma mark Instance Methods

- (BOOL)safeMoveItemAtURL:(NSURL *)srcURL
           toOverwriteURL:(NSURL *)destURL
                    error:(NSError **)error
{
    NSURL *backupURL = nil;
    
    if ([self fileExistsAtPath:destURL.path isDirectory:nil]) {
        backupURL = [NSURL fileURLWithPath:[NSFileManager pathForTemporaryFileWithPrefix:destURL.lastPathComponent]];
        
        if (![self moveItemAtURL:destURL toURL:backupURL error:nil]) {
            return NO;
        }
    }
    
    if (![self moveItemAtURL:srcURL toURL:destURL error:error]) {
        if (backupURL) {
            if ([self fileExistsAtPath:destURL.path isDirectory:nil]) {
                [self removeItemAtPath:destURL.path error:nil];
            }
            
            if (![self moveItemAtURL:backupURL toURL:destURL error:nil]) {
                return NO;
            }
        }
        
        return NO;
    }

    if (backupURL) {
        [self removeItemAtPath:backupURL.path error:nil];
    }

    return YES;
}

@end
