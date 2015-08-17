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

/*! Artwork used for cover art for a track, artist picture or record label logo. */
@interface NxAArtwork : NSManagedObject

#pragma mark Factory Methods

/*!
 * Create or retrieve and existing artwork.
 * \param data Data to use for the artwork.
 * \param context Managed object context to create the new object in.
 * \return An artwork matching the data or nil if failed.
 */
+ (instancetype)artworkWithData:(NSData *)data
                        context:(NSManagedObjectContext *)context;


/*!
 * Create or retrieve and existing artwork.
 * \param image Image to use for the artwork.
 * \param context Managed object context to create the new object in.
 * \return An artwork matching the data or nil if failed.
 */
+ (instancetype)artworkWithImage:(NSImage *)image
                         context:(NSManagedObjectContext *)context;

#pragma mark Class Methods

+ (NSString *)hashForImage:(NSImage *)image;

#pragma mark Properties

@property (readonly, nonatomic) NSString *stringValue;

@property (readonly, nonatomic) BOOL isAnOrphan;

#pragma mark Instance Methods

- (void)destroy;

@end
