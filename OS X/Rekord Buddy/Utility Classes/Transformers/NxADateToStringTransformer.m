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
#import "NxADateToStringTransformer.h"

#pragma mark Implementation

@implementation NxADateToStringTransformer

#pragma mark NSValueTransformer Protocol

/*!
 * What type of class this transformer outputs.
 * \return A class type.
 */
+ (Class)transformedValueClass
{
    return [NSDate class];
}

/*!
 * Does this transformer allow for reverse transformation.
 * \return YES if reverse-transformation is supported, NO otherwise.
 */
+ (BOOL)allowsReverseTransformation
{
    return NO;
}

/*!
 * Transform an NSDate to an NSString.
 * \param value The set to transform.
 * \return A string representing the date.
 */
- (id)transformedValue:(id)value
{
    if (!value) {
        return nil;
    }

    BOOL valueHasCorrectType = [value isKindOfClass:[NSDate class]];
    if (!valueHasCorrectType) {
        ALog(@"Incorrect Type");
        return nil;
    }

    NSDate *date = (NSDate *)value;
    NSString *dateString = [date stringValueInLocalTimeZoneWithFormat:@"yyyy/MM/dd"];

    return dateString;
}

@end
