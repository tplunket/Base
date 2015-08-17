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

@class NxACrate;

#import "NxAMessageLog.h"

#import <XCTest/XCTest.h>

#define HC_SHORTHAND
#import <OCHamcrest/OCHamcrest.h>
#undef is

#define MOCKITO_SHORTHAND
#import <OCMockito/OCMockito.h>

#pragma mark Macros

#define assertThatNothingWasLoggedIn(mockLog) \
        [verifyCount(mockLog, never()) addWarning:anything()]; \
        [verifyCount(mockLog, never()) addWarning:anything() inContextWithPerObjectWarningInfo:anything()]; \
        [verifyCount(mockLog, never()) addError:anything()];

#define assertThatNothingWasLogged() NxAMessageLog *mockLog = self.mockLog; \
        assertThatNothingWasLoggedIn(mockLog)

#pragma mark - Public Interface

/*! Base class for all our test cases. */
@interface NxATestCase : XCTestCase

#pragma mark Class Methods

+ (NSXMLElement *)xmlElementFromString:(NSString *)elementString;

#pragma mark Properties

/*!
 * Managed object context set up for each test.
 * \details This is guaranteed not to be nil.
 */
@property (readonly, nonatomic) NSManagedObjectContext *context;

/*!
 * Mock message log used to see if things get logged or not.
 * \details This is guaranteed not to be nil.
 */
@property (readonly, nonatomic) NxAMessageLog *mockLog;

#pragma mark Instance Methods

- (id)mockParserWithClass:(Class)classToMock;

- (NxACrate *)folderWithName:(NSString *)name
 withEmptyContentLastModifiedOn:(NSDate *)modificationDate;

- (NxACrate *)playlistWithName:(NSString *)name
withEmptyContentLastModifiedOn:(NSDate *)modificationDate;

@end
