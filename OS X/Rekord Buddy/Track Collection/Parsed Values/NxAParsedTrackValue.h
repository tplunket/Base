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

@class NxAParsingOperation;
@class NxAMessageLog;

#import "NxAParsedValue.h"
#import "NxAParserImplementation.h"

#pragma mark Public Interface

@interface NxAParsedTrackValue : NxAParsedValue

#pragma mark Factory Methods

+ (instancetype)parsedValueWithNoTrack;

+ (instancetype)parsedValueWithSourceTrack:(id)source
                      whichHasTheLocalPath:(NSString *)path
                        andCanBeParsedWith:(id<NxAParserImplementation>)parser
                       andParsingOperation:(NxAParsingOperation *)operation
                         loggingMessagesIn:(NxAMessageLog *)log;

@end
