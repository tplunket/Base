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

#import "Track Collection/Tags/Marker Properties/NxACueMarkerProperty.h"
#import "Track Collection/Tags/Internal/NxATag.h"

#pragma mark Private Interface

@interface NxAMarkerProperty ()

#pragma mark Factory Methods

+ (instancetype)p_markerAtPosition:(NSDecimalNumber *)position
                         inContext:(NSManagedObjectContext *)context;

#pragma mark Properties

/*! Position of this marker in seconds. */
@property (strong, nonatomic) NSDecimalNumber *p_pos;

@property (strong, nonatomic) NSSet *p_customFlagStrings;

#pragma mark Instance Methods

- (BOOL)p_hasCustomFlag:(NSString *)flag;

- (void)p_addCustomFlag:(NSString *)flag;

@end
