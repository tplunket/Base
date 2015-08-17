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

/*! Custom category of the NSFileManager with utility functions. */
@interface NSFileManager (NxAUtility)

#pragma mark Class Methods

+ (NSString *)pathForTemporaryFileWithPrefix:(NSString *)prefix;

+ (NSDate *)modificationDateForFileAtURL:(NSURL *)url;

#pragma mark Instance Methods

/*!
 * Safely move an item to another location, overwriting any exisiting file.
 * \details If the destination already exits and the move fails, the previous item remains in place.
 * \param srcURL Source item to move.
 * \param dstURL Where to move it to.
 * \param error If non-nil, system errors will be written here.
 * \return YES if successful, NO Otherwise.
 */
- (BOOL)safeMoveItemAtURL:(NSURL *)srcURL
           toOverwriteURL:(NSURL *)dstURL
                    error:(NSError **)error;

@end
