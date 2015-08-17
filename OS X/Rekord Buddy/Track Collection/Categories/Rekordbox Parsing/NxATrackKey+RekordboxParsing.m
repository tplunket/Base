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

#import "NxATrackKey+RekordboxParsing.h"
#import "Track Collection/Tags/Track Properties/Internal/NxATrackKey.h"
#import "NSXMLElement+NxAUtility.h"

#pragma mark Constants

NSString *const NxAKeyPropertyRekordboxKeyName = @"RekordboxKey";

#pragma mark Implementation

@implementation NxATrackKey (RekordboxParsing)

#pragma mark Class Methods

+ (void)updateRekordboxKeyElementForKey:(NxATrackKey *)key
                              inElement:(NSXMLElement *)element
{
    NSXMLNode *attribute = [element attributeForName:@"Tonality"];
    NSString *keyValue = [key stringValueForNotation:NxAMusicalNotationStandard];
    if (attribute) {
        if (!keyValue) {
            [element removeAttributeForName:@"Tonality"];
        }
        else {
            [attribute setStringValue:keyValue];
        }
    }
    else if (key) {
        [element addAttributeWithName:@"Tonality" stringValue:keyValue];
    }
}

@end
