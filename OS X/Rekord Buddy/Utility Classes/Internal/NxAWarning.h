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

#import "Utility Classes/NxAWarning.h"

#pragma mark Private Interface

@interface NxAWarning ()

#pragma mark Properties

/*! Description of this warning. */
@property (strong, nonatomic) NSString *p_description;

@property (readwrite, strong, nonatomic) NSString *additionalUrl;

@end

