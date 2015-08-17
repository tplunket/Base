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

#pragma mark Public Interface

@interface NSData (NxAUtility)

#pragma mark Instance Methods

/*!
 * Get the URL contained in this bookmark data.
 * \param options Options taken into account when resolving the bookmark data.
 * \param relativeURL The base URL that the bookmark data is relative to.
 * \param isStale On return, if YES, the bookmark data is stale.
 * \param error The error that occurred in the case that the URL cannot be created.
 * \return A new URL made by resolving this bookmark data.
 */
- (NSURL *)URLByResolvingBookmarkDataWithOptions:(NSURLBookmarkResolutionOptions)options
                                   relativeToURL:(NSURL *)relativeURL
                             bookmarkDataIsStale:(BOOL *)isStale
                                           error:(NSError **)error;

@end
