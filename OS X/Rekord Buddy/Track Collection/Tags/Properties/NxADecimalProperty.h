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

#import "Track Collection/Tags/NxATag.h"

#pragma mark Public Interface

/*! Decimal number property for a track. */
@interface NxADecimalProperty : NxATag

#pragma mark Properties

/*! Value of the property as a string representing time in minutes:seconds. */
@property (readonly, nonatomic) NSString *timeStringValue;

#pragma mark Instance Methods

- (NSString *)stringValueWithFractionDigits:(NSUInteger)fractionDigits;

@end
