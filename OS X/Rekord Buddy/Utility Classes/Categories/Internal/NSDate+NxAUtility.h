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

#import "NSDate+NxAUtility.h"

#pragma mark Private Interface

@interface NSDate (NxAUtility_Private)

#pragma mark Class Methods

/*!
 * Set a local timezone to be used for testing purposes.
 * \param timeZone Timezone name to use as the the local timezone from now on.
 */
+ (void)p_setTestingLocalTimeZone:(NSString *)timeZone;

@end
