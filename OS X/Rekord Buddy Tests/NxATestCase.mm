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

#import "NxATestCase.h"
#import "Track Collection/Parsers/Internal/NxARekordboxParser.h"
#import "Track Collection/Crates/NxACrate.h"
#import "NSManagedObjectContext+NxAUtility.h"
#import "Utility Classes/Categories/Internal/NSDate+NxAUtility.h"
#import "Track Collection/AudioFiles/OSX/Internal/NxAOSXAudioFile.h"

// -- This forces the linker to link the object files where these tests
// -- are defined. Otherwise they would get stripped out.
#include "Base/Tests/Tests.hpp"
#include "SeratoDB/Tests/Tests.hpp"
#include "Persistence/Tests/Tests.hpp"

#pragma mark Private Interface

@interface NxATestCase ()

#pragma mark Properties

@property (strong, nonatomic) NSManagedObjectContext *context;

@property (strong, nonatomic) NxAMessageLog *mockLog;

@end

#pragma mark - Implementation

@implementation NxATestCase

#pragma mark Class Methods

+ (NSXMLElement *)xmlElementFromString:(NSString *)elementString
{
    NSError *error = nil;
    NSXMLElement *element = [[NSXMLElement alloc] initWithXMLString:elementString
                                                              error:&error];
    NxAAssertNotNil(element);

    return element;
}

#pragma mark Instance Methods

- (id<NxAParserImplementation>)mockParserWithClass:(Class)classToMock
{
    id<NxAParserImplementation> mockParser = mockObjectAndProtocol(classToMock, @protocol(NxAParserImplementation));

    [given([mockParser log]) willReturn:self.mockLog];
    [given([mockParser parserClass]) willReturn:classToMock];

    return mockParser;
}

- (NxACrate *)folderWithName:(NSString *)name
 withEmptyContentLastModifiedOn:(NSDate *)modificationDate
{
    NxACrate *crate = [NxACrate folderWithName:name
                withEmptyContentLastModifiedOn:modificationDate
                                     inContext:self.context];
    NxAAssertNotNil(crate);

    return crate;
}

- (NxACrate *)playlistWithName:(NSString *)name
 withEmptyContentLastModifiedOn:(NSDate *)modificationDate
{
    NxACrate *crate = [NxACrate playlistWithName:name
                  withEmptyContentLastModifiedOn:modificationDate
                                       inContext:self.context];
    NxAAssertNotNil(crate);

    return crate;
}

#pragma mark Overidden XCTest methods

- (void)setUp
{
    [super setUp];

    // -- We set up a test context that will be used solely in memory.
    NSManagedObjectContext *context = [NSManagedObjectContext contextInMemoryWithConcurrencyType:NSMainQueueConcurrencyType
                                                                                           error:nil];
    NxAAssertNotNil(context);
    self.context = context;

    NxAMessageLog *mocklog = mock([NxAMessageLog class]);
    NxAAssertNotNil(mocklog);
    self.mockLog = mocklog;

    // -- Then we make sure all the tests execute with the same local timezone.
    [NSDate p_setTestingLocalTimeZone:@"US/Eastern"];

    // -- And that our volume names are consistent too.
    [NxAOSXAudioFile p_setTestingBootVolumeName:@"Test Macintosh HD"];
    [NxAOSXAudioFile p_setTestingOtherVolumeNames:[NSSet setWithObjects:@"EXTERNAL", @"OTHER", nil]];

    // -- In case any tests have set this up, we reset the testing stubs.
    [NxAOSXAudioFile p_setTestingFileManager:nil];

}

@end
