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

#import "Track Collection/Tags/Track Properties/Internal/NxATrackCredit.h"

@implementation NxATrackCredit

#pragma mark Factory Methods

+ (instancetype)tagWithValue:(id)value
                     context:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(context);

    NxAArtist *artist = [NxAArtist artistWithName:value context:context];
    if (!artist) {
        return nil;
    }

    NxATrackCredit *newCredit = (NxATrackCredit *)[super tagWithValue:value
                                                              context:context];
    NxAAssertNotNil(newCredit);

    newCredit.p_artist = artist;
    
    return newCredit;
}

#pragma mark Overridden NxATag Class Methods

+ (Class)p_valueClass
{
    return [NSString class];
}

#pragma mark Accessors

// -- Automatically generated for Core Data properties.
@dynamic p_artist;

#pragma mark Overridden NxATag Accessors

- (NSString *)propertyDescription
{
    // -- This should be overloaded by any derived class
    ALog(@"Illegal base class call from '%@'.", self.class);

    return nil;
}

@end
