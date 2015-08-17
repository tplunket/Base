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

#import "NSAttributedString+NxAUtility.h"

#pragma mark Implementation

@implementation NSAttributedString (NxAUtility)

#pragma mark Factory Methods

+ (instancetype)stringWithString:(NSString *)string
{
    NxAAssertNotNil(string);
    
    NSAttributedString *attrString = [[NSAttributedString alloc] initWithString:string];
    
    return attrString;
}

+ (instancetype)stringWithString:(NSString *)string
                      attributes:(NSDictionary *)attributes
{
    NxAAssertNotNil(string);
    NxAAssertNotNil(attributes);

    NSAttributedString *attrString = [[NSAttributedString alloc] initWithString:string attributes:attributes];
    
    return attrString;
}

+ (instancetype)stringWithHyperlink:(NSString *)string
                                url:(NSURL *)url
{
    NxAAssertNotNil(string);
    NxAAssertNotNil(url);

    NSMutableAttributedString *attrString = [[NSMutableAttributedString alloc] initWithString:string];
    NSRange range = NSMakeRange(0, attrString.length);
 	
    [attrString beginEditing];
    
    [attrString addAttribute:NSLinkAttributeName value:[url absoluteString] range:range];
 	
    // -- Make the text appear in blue.
    [attrString addAttribute:NSForegroundColorAttributeName value:[NSColor blueColor] range:range];
 	
    // -- Make the text appear with an underline.
    [attrString addAttribute:NSUnderlineStyleAttributeName value:@(NSSingleUnderlineStyle) range:range];
 	
    [attrString endEditing];
 	
    return [[NSAttributedString alloc] initWithAttributedString:attrString];
}

@end
