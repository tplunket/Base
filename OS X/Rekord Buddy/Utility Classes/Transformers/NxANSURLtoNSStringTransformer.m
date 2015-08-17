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

#import "NxANSURLToNSStringTransformer.h"

#pragma mark Implementation

@implementation NxANSURLToNSStringTransformer

#pragma mark NSValueTransformer Protocol

/*!
 * What type of class this transformer outputs.
 * \return A class type.
 */
+ (Class)transformedValueClass
{
    return [NSString class];
}

/*!
 * Does this transformer allow for reverse transformation.
 * \return YES if reverse-transformation is supported, NO otherwise.
 */
+ (BOOL)allowsReverseTransformation
{
    return YES;
}

/*!
 * Transform an NSURL to an NSString.
 * \param value The URL to transform.
 * \return A string representing the URL's path.
 */
- (id)transformedValue:(id)value
{
    if (!value) {
        return nil;
    }

    BOOL valueHasCorrectType = [value isKindOfClass:[NSURL class]];
    if (!valueHasCorrectType) {
        ALog(@"Value (%@) is not an NSURL.", [value class]);
        return nil;
    }

    NSURL *url = value;
    return url.path;
}

/*!
 * Transform an NSString to an NSURL.
 * \param value The string to transform.
 * \return A URL representing the path contained in the string.
 */
- (id)reverseTransformedValue:(id)value
{
    if (!value) {
        return nil;        
    }
    
    BOOL valueHasCorrectType = [value isKindOfClass:[NSString class]];
    if (!valueHasCorrectType) {
        ALog(@"Value (%@) is not an NSString.", [value class]);
        return nil;
    }

    return valueHasCorrectType ? [NSURL URLWithString:value] : nil;
}

@end
