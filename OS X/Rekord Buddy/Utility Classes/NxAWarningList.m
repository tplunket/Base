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

#import "NxAWarningList.h"

#import "NSAttributedString+NxAUtility.h"

#pragma mark Private Interface

@interface NxAWarningList ()

#pragma mark Properties

/*! All regular warnings, sorted by description. */
@property (strong, nonatomic) NSMutableDictionary *p_warnings;

/*! All group warnings, sorted by description. */
@property (strong, nonatomic) NSMutableDictionary *p_objectWarnings;

/*! Number of warning currently stored in the list */
@property (readwrite, assign, nonatomic) NSUInteger count;

@end

#pragma mark - Implementation

@implementation NxAWarningList

#pragma mark Accessors

- (NSMutableDictionary *)p_warnings
{
    if (!_p_warnings) {
        _p_warnings = [NSMutableDictionary dictionaryWithCapacity:16];
    }

    return _p_warnings;
}

- (NSMutableDictionary *)p_objectWarnings
{
    if (!_p_objectWarnings) {
        _p_objectWarnings = [NSMutableDictionary dictionaryWithCapacity:16];
    }

    return _p_objectWarnings;
}

#pragma mark Instance Methods

- (void)reset
{
    [self.p_warnings removeAllObjects];
    [self.p_objectWarnings removeAllObjects];

    self.count = 0;
}

- (void)addWarning:(NxAWarning *)warning
{
    NxAAssertNotNil(warning);
    
    // -- Make sure we don't already have this warning in store.
    if (self.p_warnings[warning.description]) {
        return;
    }
    
    self.p_warnings[warning.description] = warning;

    ++self.count;
}

- (void)addWarning:(NxAWarning *)warning
         forObject:(id <NxAWarningContextObject>)object
     perObjectInfo:(NSString *)perObjectInfo;
{
    NxAAssertNotNil(warning);
    NxAAssertNotNil(object);
    NxAAssertNotNil(perObjectInfo);

    NSMutableArray *existingWarnings = nil;
    for (NxAWarning *existingWarning in self.p_objectWarnings.allKeys) {
        if ([warning.description isEqualToString:existingWarning.description]) {
            existingWarnings = self.p_objectWarnings[existingWarning];
        }
    }

    if (!existingWarnings) {
        // -- If we've never seen this warning before, create a place to store all the objects it occurs on.
        existingWarnings = [NSMutableArray arrayWithCapacity:16];
        self.p_objectWarnings[warning] = existingWarnings;
    }
    
    // -- Make sure we don't already have this object info string in store for this warning.
    for (NSArray *entry in existingWarnings) {
        NxAAssertTrue(entry.count == 2);
        
        if ([entry[0] isEqualTo:object]) {
            return;
        }
    }
    
    [existingWarnings addObject:@[ object, perObjectInfo ]];
    
    ++self.count;
}

- (NSAttributedString *)warningText
{
    // -- Make sure we have some warnings stored
    if (self.count == 0) {
        return nil;
    }
    
    NSMutableAttributedString *printVersion = [[NSAttributedString stringWithString:@""] mutableCopy];
    NxAAssertNotNil(printVersion);
    
    // -- Add all the regular warnings we have.
    for (NxAWarning *warning in self.p_warnings.allValues) {
        [printVersion appendAttributedString:[NSAttributedString stringWithString:[NSString stringWithFormat:@"%@\n", warning.description]]];
        
        if (warning.additionalUrl) {
            // -- Some of them have additional info via a URL that the user can click on.
            [printVersion appendAttributedString:[NSAttributedString stringWithString:@"Related information can be found by "]];
            [printVersion appendAttributedString:[NSAttributedString stringWithHyperlink:@"clicking this link" url:[NSURL URLWithString:warning.additionalUrl]]];
            [printVersion appendAttributedString:[NSAttributedString stringWithString:@".\n\n"]];
        }
    }

    // -- Add all the object warnings we have.
    for (NxAWarning *warning in self.p_objectWarnings.allKeys) {
        NSMutableArray *objects = self.p_objectWarnings[warning];
        NxAAssertFalse(objects.count == 0);
        
        NSArray *firstEntry = objects[0];
        NxAAssertTrue(firstEntry.count == 2);
        
        id <NxAWarningContextObject> firstObject = firstEntry[0];

        // -- Tell the user how many warning we found for this particular object.
        [printVersion appendAttributedString:[NSAttributedString stringWithString:[NSString stringWithFormat:@"Found %ld %@(s) %@\n",
                                                                                   (unsigned long)objects.count,
                                                                                   firstObject.objectTypeName,
                                                                                   warning.description]]];
        
        if (warning.additionalUrl) {
            // -- If this warning has an additional URL to print, print it only once.
            [printVersion appendAttributedString:[NSAttributedString stringWithString:@"Related information can be found by "]];
            [printVersion appendAttributedString:[NSAttributedString stringWithHyperlink:@"clicking this link"
                                                                                     url:[NSURL URLWithString:warning.additionalUrl]]];
            [printVersion appendAttributedString:[NSAttributedString stringWithString:@".\n"]];
        }
        
        NSMutableString *objectList = [NSMutableString stringWithString:@""];
        NxAAssertNotNil(objectList);
        
        for (NSArray *entry in objects) {
            NxAAssertTrue(entry.count == 2);

            id <NxAWarningContextObject> object = entry[0];
            NSString *additionalObjectInfo = entry[1];

            if (additionalObjectInfo.length) {
                [objectList appendFormat:@"\t%@ (%@)\n", object.description, additionalObjectInfo];
            }
            else {
                [objectList appendFormat:@"\t%@\n", object.description];
            }
        }

        [printVersion appendAttributedString:[NSAttributedString stringWithString:objectList]];
    }
    
    [printVersion appendAttributedString:[NSAttributedString stringWithString:@"\nIf the problems persists, get some help in the "]];
    [printVersion appendAttributedString:[NSAttributedString stringWithHyperlink:@"Next Audio Labs FAQ" url:[NSURL URLWithString:@"http://nextaudiolabs.com/faq"]]];
    [printVersion appendAttributedString:[NSAttributedString stringWithString:@" or email "]];
    [printVersion appendAttributedString:[NSAttributedString stringWithHyperlink:@"support@nextaudiolabs.com." url:[NSURL URLWithString:@"mailto:support@nextaudiolabs.com"]]];

    NSAttributedString *resultString = [[NSAttributedString alloc] initWithAttributedString:printVersion];
    NxAAssertNotNil(resultString);
    return resultString;
}

@end
