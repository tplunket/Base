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

#import "NxAArtwork.h"
#import "NSManagedObjectContext+NxAUtility.h"
#import "NxAHash.h"
#import "NxAParsedArtworkValue.h"
#import "Track Collection/Tags/Properties/Internal/NxAArtworkProperty.h"

#pragma mark Implementation

@implementation NxAArtworkProperty

#pragma mark Factory Methods

+ (instancetype)artworkPropertyWithImage:(NSImage *)image
                                 context:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(image);
    NxAAssertNotNil(context);

    NxAParsedArtworkValue *artworkValue = [NxAParsedArtworkValue parsedValueWithImage:image];
    NxAArtworkProperty *result = (NxAArtworkProperty *)[artworkValue tagForClass:[NxAArtworkProperty class] usingContext:context];
    NxAAssertNotNil(result);

    return result;
}

#pragma mark Overridden NxATag Class Methods

+ (NSString *)p_managedEntityName
{
    return @"ArtworkProperty";
}

+ (Class)p_valueClass
{
    return [NxAArtwork class];
}

+ (BOOL)sharesTags
{
    // -- Artwork properties are not shared between different tagged objects,
    // -- each object gets an individual one.
    return NO;
}

#pragma mark Accessors

// -- Automatically generated for Core Data properties.
@dynamic p_value;

#pragma mark Overridden NxATag Accessors

- (NSString *)stringValue
{
    return self.p_value.stringValue;
}

#pragma mark Overridden NxATag Instance Methods

- (void)destroy
{
    NxAArtwork *artwork = self.p_value;
    self.p_value = nil;

    if (artwork.isAnOrphan) {
        [artwork destroy];
    }

    [super destroy];
}

#pragma mark Overridden NSObject Methods

- (NSString *)description
{
    return @"Artwork";
}

@end
