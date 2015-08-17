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

@class NxAMessageLog;

#import "NxAPreferencesViewController.h"
#import "NxAParserImplementation.h"

#pragma mark Constants

#define NxAPreferencesViewControllerItemMinimumWidth        10

#pragma mark Public Interface

@protocol NxAParserPreferencesViewController <NxAPreferencesViewController>

#pragma mark Properties

@property (readonly, nonatomic) BOOL collectionIsAvailable;

@property (readonly, nonatomic) NSString *nameOfTheApplication;

#pragma mark Instance Methods

- (id<NxAParserImplementation>)parserImplementationUsingContext:(NSManagedObjectContext *)context
                                              loggingMessagesIn:(NxAMessageLog *)log;

- (void)resetAll;

@end
