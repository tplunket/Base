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

#import "NSXMLDocument+NxAUtility.h"

#pragma mark Implementation

@implementation NSXMLDocument (NxAUtility)

#pragma mark Factory Methods

- (instancetype)initWithDataAndIgnoreIllegalCharacters:(NSData *)data
                                          illegalChars:(NSCharacterSet *)illegalChars
                                     foundIllegalChars:(BOOL *)foundSomeIllegalChars
                                                   log:(NxAMessageLog *)log
{
    NxAAssertNotNil(data);
    NxAAssertNotNil(illegalChars);
    
    // -- Then, read the resulting XML string.
    NSMutableString *str = [[NSMutableString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    
    // -- Go through the XML, only caring about attribute value strings
    NSMutableArray *charactersToRemove = [NSMutableArray array];
    NSUInteger openQuotes = NSNotFound;
    NSUInteger escapedCharStart = NSNotFound;
    for (NSUInteger pos = 0; pos < str.length; ++pos) {
        NSUInteger currentChar = [str characterAtIndex:pos];
        
        if (currentChar == '\"') {
            if (openQuotes == NSNotFound) {
                openQuotes = pos;
            }
            else {
                
                openQuotes = NSNotFound;
            }
        }
        else if (openQuotes != NSNotFound) {
            if (escapedCharStart != NSNotFound) {
                if (currentChar == ';') {
                    NSString *escapedChar = [str substringWithRange:NSMakeRange(escapedCharStart + 3, pos - escapedCharStart - 3)];
                    NSScanner *scanner=[NSScanner scannerWithString:escapedChar];
                    
                    unsigned charValue;
                    [scanner scanHexInt:&charValue];
                    
                    if ([illegalChars characterIsMember:charValue]) {
                        [charactersToRemove addObject:[NSValue valueWithRange:NSMakeRange(escapedCharStart, pos - escapedCharStart + 1)]];
                    }
                    
                    escapedCharStart = NSNotFound;
                }
            }
            else if (currentChar == '&') {
                NSUInteger nextChar = (pos < (str.length - 1)) ? [str characterAtIndex:pos + 1] : 0;
                NSUInteger nextNextChar = (pos < (str.length - 2)) ? [str characterAtIndex:pos + 2] : 0;
                
                if ((nextChar == '#') && ((nextNextChar == 'x') || (nextNextChar == 'X'))) {
                    escapedCharStart = pos;
                    pos += 2;
                }
            }
            // -- If we find an illegal character, we make a note of its position.
            if ([illegalChars characterIsMember:currentChar]) {
                [charactersToRemove addObject:[NSValue valueWithRange:NSMakeRange(pos, 1)]];
            }
        }
    }
    
    if (foundSomeIllegalChars) {
        *foundSomeIllegalChars = (charactersToRemove.count != 0);
    }
    
    if (charactersToRemove.count) {
        NSUInteger index = charactersToRemove.count;
        
        // -- If we have characters to fix, we work thru them backwards, in order to not mess up our saved positions by modifying the XML.
        do {
            --index;
            
            NSValue *characterRange = charactersToRemove[index];
            [str deleteCharactersInRange:characterRange.rangeValue];
        }
        while (index > 0);
        
        // -- Finally we update the data with our corrected version
        data = [str dataUsingEncoding:NSUTF8StringEncoding];
    }
    
    NSError *systemError = nil;
    self = [self initWithData:data options:NSXMLNodeOptionsNone error:&systemError];
    if (!self) {
        [log addMessage:systemError.localizedDescription];
    }
    
    return self;
}

@end
