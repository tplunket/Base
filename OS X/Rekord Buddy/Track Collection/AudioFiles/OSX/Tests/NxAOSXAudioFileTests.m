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

#import "NSData+NxAUtility.h"
#import "NxAOSXAudioFileTests.h"

#pragma mark Implementation

@implementation NxAOSXAudioFileTests

#pragma mark Class Methods

+ (NxAOSXAudioFile *)stubbedOSXAudioFileWithPath:(NSString *)path
                                         context:(NSManagedObjectContext *)context;
{
    NSURL *stubURL = mock([NSURL class]);
    NSData *stubData = mock([NSData class]);

    // -- If asked for a bookmark we return a dummy one.
    [[given([stubURL bookmarkDataWithOptions:0
              includingResourceValuesForKeys:nil
                               relativeToURL:nil
                                       error:nil])
      withMatcher:anything()
      forArgument:3]
      willReturn:stubData];

    // -- If asked for a path we return the one we're faking.
    stubProperty(stubURL, path, path);

    // -- If the bookmark is asked for a URL, we return the stub.
    [[[given([stubData URLByResolvingBookmarkDataWithOptions:0
                                              relativeToURL:nil
                                        bookmarkDataIsStale:nil
                                                      error:nil])
       withMatcher:anything()
       forArgument:2]
      withMatcher:anything()
      forArgument:3]
     willReturn:stubURL];

    NxAOSXAudioFile *stubFile = [NxAOSXAudioFile fileWithURL:stubURL context:context];
    NxAAssertNotNil(stubFile);

    return stubFile;
}

@end
