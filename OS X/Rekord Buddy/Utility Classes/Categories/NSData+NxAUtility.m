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

#pragma mark Public Interface

@implementation NSData (NxAUtility)

#pragma mark Instance Methods

- (NSURL *)URLByResolvingBookmarkDataWithOptions:(NSURLBookmarkResolutionOptions)options
                                   relativeToURL:(NSURL *)relativeURL
                             bookmarkDataIsStale:(BOOL *)isStale
                                           error:(NSError **)error
{
    return [NSURL URLByResolvingBookmarkData:self
                                     options:options
                               relativeToURL:relativeURL
                         bookmarkDataIsStale:isStale
                                       error:error];
}

@end
