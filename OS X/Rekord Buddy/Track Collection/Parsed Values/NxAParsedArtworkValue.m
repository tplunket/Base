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

#import "Track Collection/Parsed Values/Internal/NxAParsedValue.h"
#import "NxAParsedArtworkValue.h"
#import "NxAArtwork.h"
#import "Track Collection/Tags/NxATag.h"

#pragma mark Private Interface

@interface NxAParsedArtworkValue ()

@property (strong, nonatomic) NSImage *p_image;

@end

#pragma mark Implementation

@implementation NxAParsedArtworkValue

#pragma mark Factory Methods

+ (instancetype)parsedValueWithNoImage
{
    NxAParsedArtworkValue *newValue = [[NxAParsedArtworkValue alloc] initPrivate];

    newValue.p_image = nil;

    return newValue;
}

+ (instancetype)parsedValueWithImage:(NSImage *)value
{

    
    NxAParsedArtworkValue *newValue = [[NxAParsedArtworkValue alloc] initPrivate];

    newValue.p_image = value;

    return newValue;
}

#pragma mark Overridden NxAParsedValue Accessors

- (NSString *)comparisonValue
{
    return self.p_image ? [NSString stringWithFormat:@"<%@>", [NxAArtwork hashForImage:self.p_image]] : @"nil";
}

#pragma mark Instance Methods

- (instancetype)initPrivate
{
    return [super initPrivate];
}

#pragma mark Overridden NxAParsedValue Instance Methods

- (NxATag *)tagForClass:(Class)tagClass
           usingContext:(NSManagedObjectContext *)context;
{
    NxAAssertNotNil(tagClass);
    NxAAssertNotNil(context);

    NxAArtwork *newArtwork = self.p_image ? [NxAArtwork artworkWithImage:self.p_image context:context] : nil;
    return [tagClass tagWithValue:newArtwork context:context];
}

#pragma mark Overridden NSObject Instance Methods

- (instancetype)init
{
    ALog(@"Illegal base class call from '%@'.", self.class);
    return nil;
}

@end
