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

#import "NxATrackKey+TraktorParsing.h"
#import "Track Collection/Tags/Track Properties/Internal/NxATrackKey.h"
#import "NSXMLElement+NxAUtility.h"

#pragma mark Constants

NSString *const NxAKeyPropertyTraktorKeyName = @"TraktorKey";

#pragma mark Implementation

@implementation NxATrackKey (TraktorParsing)

#pragma mark Class Methods

+ (NSNumber *)keyValueForTraktorKeyValue:(NSUInteger)value;
{
    if (value > NxAMusicalKeyMaxIndex) {
        return nil;
    }

    // -- Traktor keys use the same index we do.
    return @(value);
}

/*!
 * Convert an internal key value to a Traktor key value.
 * \param value Internal key value to convert.
 * \return A Traktor key value, if key is supported or nil otherwise.
 */
+ (NSNumber *)p_traktorKeyValueForKeyValue:(NSUInteger)value;
{
    if (value > NxAMusicalKeyMaxIndex) {
        return nil;
    }

    // -- Traktor keys use the same index we do.
    return @(value);
}

#pragma mark Instance Methods

- (void)updateTraktorKeyElementInEntry:(NSXMLElement *)entryElement
{
    NSString *keyStringValue = [[NxATrackKey p_traktorKeyValueForKeyValue:self.p_value.integerValue] stringValue];

    NSXMLElement *musicalKeyElement = [entryElement elementForName:@"MUSICAL_KEY"];
    if (!musicalKeyElement) {
        musicalKeyElement = [NSXMLElement elementWithName:@"MUSICAL_KEY"];
        [entryElement addChild:musicalKeyElement];
    }

    NSXMLNode *valueAttribute = [musicalKeyElement attributeForName:@"VALUE"];
    if (!valueAttribute) {
        [musicalKeyElement addAttributeWithName:@"VALUE" stringValue:keyStringValue];
    }
    else {
        [valueAttribute setStringValue:keyStringValue];
    }
}

- (void)updateTraktorKeyElementInInfoElement:(NSXMLElement *)infoElement
{
    NSString *keyStringValue = [self stringValueForNotation:NxAMusicalNotationStandardAllSharps];

    NSXMLNode *keyAttribute = [infoElement attributeForName:@"KEY"];
    if (!keyAttribute) {
        [infoElement addAttributeWithName:@"KEY" stringValue:keyStringValue];
    }
    else {
        [keyAttribute setStringValue:keyStringValue];
    }
}

@end
