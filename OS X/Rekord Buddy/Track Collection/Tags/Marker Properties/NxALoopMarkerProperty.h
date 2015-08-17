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

#pragma mark Public Interface

/*! Marker used to indicate a loop in a track. */
@interface NxALoopMarkerProperty : NxACueMarkerProperty

#pragma mark Factory Methods

+ (instancetype)loopMarkerWithName:(NSString *)name
                          position:(NSDecimalNumber *)position
                            length:(NSDecimalNumber *)length
                            hotCue:(NSNumber *)hotCue
                           context:(NSManagedObjectContext *)context;

+ (instancetype)loopMarkerWithName:(NSString *)name
                          position:(NSDecimalNumber *)position
                            length:(NSDecimalNumber *)length
                           context:(NSManagedObjectContext *)context;

@end
