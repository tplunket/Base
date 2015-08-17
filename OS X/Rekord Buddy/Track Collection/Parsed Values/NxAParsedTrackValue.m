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

#import "Track Collection/Parsed Values/Internal/NxAParsedValue.h"
#import "NxAParsedTrackValue.h"
#import "NxATrack.h"
#import "NxATrack+NxAWarningContextObject.h"
#import "NxATrack+NxAOSXAudioFile.h"
#import "Track Collection/AudioFiles/OSX/NxAOSXAudioFile.h"
#import "Track Collection/Tags/Crate Properties/NxATrackProperty.h"
#import "Track Collection/Operations/NxAParsingOperation.h"
#import "NxAMessageLog.h"

#pragma mark Private Interface

@interface NxAParsedTrackValue ()

@property (strong, nonatomic) NSString *p_path;

@property (strong, nonatomic) id p_source;

@property (strong, nonatomic) id<NxAParserImplementation> p_parser;

@property (strong, nonatomic) NxAParsingOperation *p_operation;

@property (strong, nonatomic) NxAMessageLog *p_log;

@end

#pragma mark Implementation

@implementation NxAParsedTrackValue

#pragma mark Factory Methods

+ (instancetype)parsedValueWithNoTrack
{
    NxAParsedTrackValue *newValue = [[NxAParsedTrackValue alloc] initPrivate];
    NxAAssertNotNil(newValue);

    newValue.p_path = nil;

    return newValue;
}

+ (instancetype)parsedValueWithSourceTrack:(id)source
                      whichHasTheLocalPath:(NSString *)path
                        andCanBeParsedWith:(id<NxAParserImplementation>)parser
                       andParsingOperation:(NxAParsingOperation *)operation
                         loggingMessagesIn:(NxAMessageLog *)log
{
    NxAAssertNotNil(source);
    NxAAssertNotNil(path);
    NxAAssertNotNil(parser);
    NxAAssertNotNil(operation);
    NxAAssertNotNil(log);

    NxAParsedTrackValue *newValue = [[NxAParsedTrackValue alloc] initPrivate];
    NxAAssertNotNil(newValue);

    newValue.p_source = source;
    newValue.p_path = path;
    newValue.p_parser = parser;
    newValue.p_operation = operation;
    newValue.p_log = log;

    return newValue;
}

#pragma mark Overridden NxAParsedValue Accessors

- (NSString *)comparisonValue
{
    return self.p_path ? [NSString stringWithFormat:@"<%@>", self.p_path] : @"nil";
}

#pragma mark Instance Methods

- (instancetype)initPrivate
{
    return [super initPrivate];
}

#pragma mark Overridden NxAParsedValue Instance Methods

- (NxATag *)tagForClass:(Class)tagClass usingContext:(NSManagedObjectContext *)context;
{
    NxAAssertTrue(tagClass == [NxATrackProperty class]);
    NxAAssertNotNil(context);

    NSString *locationPath = self.p_path;
    if (!locationPath) {
        return [tagClass tagWithValue:nil context:context];
    }

    NxATrack *newTrack = nil;

    // -- We find out if the track already exists.
    NSURL *locationURL = [NSURL fileURLWithPath:locationPath];
    NxAOSXAudioFile *file = [NxAOSXAudioFile existingFileWithURL:locationURL context:context];
    if (file) {
        // -- If this file was already in our collection, get its parent track.
        newTrack = file.parentTrack;
    }
    else {
        NSFileManager *fileManager = [NSFileManager defaultManager];
        if (![fileManager fileExistsAtPath:locationURL.path]) {
            return nil;
        }
        
        // -- Otherwise we create a new file if the file exists.
        file = [NxAOSXAudioFile fileWithURL:locationURL context:context];

        // -- And a new track which points to that file.
        newTrack = [NxATrack trackFromOSXAudioFile:file context:context];
    }

    [self.p_parser trackForLocation:locationPath is:newTrack];

    [self.p_operation parseTrackSource:self.p_source forTrack:newTrack];

    return [tagClass tagWithValue:newTrack context:context];
}

#pragma mark Overridden NSObject Instance Methods

- (instancetype)init
{
    ALog(@"Illegal base class call from '%@'.", self.class);
    return nil;
}

@end
