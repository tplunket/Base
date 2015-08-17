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

#import "NxATrackRecordLabel.h"
#import "NxAArtwork.h"

#pragma mark Private Interface

@interface NxATrackRecordLabel ()

#pragma mark Properties

/*! Artwork used by this record label. */
@property (strong, nonatomic) NxAArtwork *p_artwork;

@end

#pragma mark - Implementation

@implementation NxATrackRecordLabel

#pragma mark Overridden NxATag Class Methods

+ (NSString *)p_managedEntityName
{
    return @"TrackRecordLabel";
}

+ (BOOL)isASingleValueTagType
{
    // -- Only one record label at a time can be added to a tagged object.
    return YES;
}

#pragma mark Accessors

// -- Automatically generated for Core Data properties.
@dynamic p_artwork;

#pragma mark Overridden NxATag Instance Methods

- (void)destroy
{
    NxAAssertTrue(self.isAnOrphan);

    NxAArtwork *artwork = self.p_artwork;
    self.p_artwork = nil;
    if (artwork.isAnOrphan) {
        [artwork destroy];
    }

    [self.managedObjectContext deleteObject:self];
}

#pragma mark Overridden NxATag Accessors

- (NSString *)propertyDescription
{
    return @"Record Label";
}

@end
