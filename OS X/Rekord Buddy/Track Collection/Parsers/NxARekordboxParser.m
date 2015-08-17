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

#import "NxAAppDelegate.h"
#import "NxAMessageLog.h"
#import "NSXMLDocument+NxAUtility.h"
#import "NSXMLElement+NxAUtility.h"
#import "NSFileManager+NxAUtility.h"
#import "Track Collection/Crates/NxACrate.h"
#import "NxATrack+Parsing.h"
#import "NxATaggedObject+XMLParsing.h"
#import "Track Collection/Tags/Crate Properties/NxATrackProperty.h"
#import "NxATrackRecordLabel.h"
#import "Track Collection/Tags/Track Properties/NxATrackAlbum.h"
#import "NxATrackGenre.h"
#import "NxATrackArtistCredit.h"
#import "NxATrackRemixerCredit.h"
#import "NxATrackProducerCredit.h"
#import "Track Collection/Tags/Track Properties/NxATrackKey.h"
#import "NxATrackKey+RekordboxParsing.h"
#import "NxATrackTitle.h"
#import "NxATrackComments.h"
#import "NxATrackMixName.h"
#import "NxATrackNumber.h"
#import "NxATrackRating.h"
#import "NxATrackPlayCount.h"
#import "NxATrackDateAdded.h"
#import "NxATrackDateReleased.h"
#import "NxATrackTag.h"
#import "Track Collection/Categories/Rekordbox Parsing/NxAMarkerProperty+RekordboxParsing.h"
#import "Track Collection/Tags/Marker Properties/NxAGridMarkerProperty.h"
#import "Track Collection/AudioFiles/OSX/NxAOSXAudioFile.h"
#import "NxATrack+RekordboxParsing.h"
#import "Track Collection/Parsers/Internal/NxARekordboxParser.h"
#import "NSError+NxAUtility.h"

#pragma mark Debug Defines

// -- This flag can be used to stop in the debugger while reading a specific track.
#define NXADebugBreakAtSpecificTrack                            0
#define NxADebugBreakAtTrackTitleStartsWith                     @""

#pragma mark - Implementation

@implementation NxARekordboxParser

#pragma mark Class Methods

+ (NSXMLDocument *)p_xmlDocumentFromFileAtURL:(NSURL *)fileURL
                            loggingMessagesIn:(NxAMessageLog *)log
{
    NxAAssertNotNil(fileURL);

    // -- Read the file into a binary data blob.
    NSData *xmlData = [NSData dataWithContentsOfURL:fileURL];
    if (!xmlData) {
        return nil;
    }

    // -- Try to convert the blob into a valid XML document, escaping out any illegal characters.
    BOOL foundIllegalCharacters = NO;
    NSXMLDocument *xmlDoc = [[NSXMLDocument alloc] initWithDataAndIgnoreIllegalCharacters:xmlData
                                                                             illegalChars:[NSCharacterSet characterSetWithRange:NSMakeRange(0, 0x1f)]
                                                                        foundIllegalChars:&foundIllegalCharacters
                                                                                      log:log];
    if (!xmlDoc) {
        return nil;
    }

    if (foundIllegalCharacters) {
        // -- If we've found illegal characters, let the user know.
        NxAWarning *warning = [NxAWarning warningWithDescription:[NSString stringWithFormat:NSLocalizedString(@"Found illegal characters in your Rekordbox collection file. These characters are not valid XML and have been ignored.",
                                                                                                              @"Tells the user that the collection file contains illegal characters which have been ignored.")]];
        [log addWarning:warning];
    }

    return xmlDoc;
}

+ (BOOL)p_collectionRootElementIsValid:(NSXMLElement *)rootElement
                     loggingMessagesIn:(NxAMessageLog *)log
{
    NxAAssertNotNil(rootElement);
    
    // -- Make sure the root element is the right one.
    if (![rootElement.name isEqualToString:@"DJ_PLAYLISTS"]) {
        return NO;
    }

    // -- Then check it's the right version.
    NSString *versionValue = [[rootElement attributeForName:@"Version"] stringValue];
    if (![versionValue isEqualToString:@"1.0.0"]) {
        return NO;
    }

    // -- Then make sure the product which wrote the file is supported.
    NSXMLElement *productElement = [rootElement elementForName:@"PRODUCT"];
    if (!productElement) {
        return NO;
    }

    NSString *nameValue = [[productElement attributeForName:@"Name"] stringValue];
    if ([nameValue isEqualToString:@"rekordbox"]) {
        NSString *productVersionValue = [[productElement attributeForName:@"Version"] stringValue];
        NSString *companyValue = [[productElement attributeForName:@"Company"] stringValue];

        if (![productVersionValue isEqualToString:@"3.2.0"] || ![companyValue isEqualToString:@"Pioneer"]) {
            NxAWarning *warning = [NxAWarning warningWithDescription:[NSString stringWithFormat:@"Your Rekordbox collection file was written by a version of Rekordbox not officially supported by this version of Rekord Buddy. Supported version is 3.2.0. If you've just updated Rekordbox or use an old version, proceed with caution as Rekord Buddy might incorrectly save your Rekordbox collection file."]];
            [log addWarning:warning];
        }
    }
    else if (![nameValue isEqualToString:@"Rekord Buddy"]) {
        return NO;
    }

    return YES;
}

+ (void)makeSureCollectionFileExistsAt:(NSURL *)url
{
    NSString *location = url.path;
    
    NSFileManager *fileManager = [NxAOSXAudioFile fileManager];
    if (![fileManager fileExistsAtPath:location isDirectory:nil]) {
        NxAAppDelegate *appDelegate = (NxAAppDelegate *)[NSApp delegate];

        NSString *defaultFile = [NSString stringWithFormat:@"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n<DJ_PLAYLISTS Version=\"1.0.0\">\n<PRODUCT Name=\"Rekord Buddy\" Version=\"%@\" Company=\"Next Audio Labs\"/>\n<COLLECTION Entries=\"0\"/>\n<PLAYLISTS>\n<NODE Type=\"0\" Name=\"ROOT\" Count=\"0\"/>\n</PLAYLISTS>\n</DJ_PLAYLISTS>\n",
                                 appDelegate.appVersionAsString];
        
        NSData *defaultContent = [defaultFile dataUsingEncoding:NSUTF8StringEncoding];
        [defaultContent writeToFile:location atomically:YES];
    }
}

+ (BOOL)isValidRekordboxXML:(NSURL *)url
{
    NSXMLDocument *xmlDoc = [[NSXMLDocument alloc] initWithContentsOfURL:url
                                                                 options:0
                                                                   error:nil];
    if (!xmlDoc) {
        return NO;
    }
    
    NSXMLElement *topElement = [xmlDoc rootElement];
    if (![topElement.name isEqualToString:@"DJ_PLAYLISTS"]) {
        return NO;
    }
    
    NSString *versionValue = [[topElement attributeForName:@"Version"] stringValue];
    if (![versionValue isEqualToString:@"1.0.0"]) {
        return NO;
    }
    
    return YES;
}

#pragma mark Accessors

- (NSMutableDictionary *)p_rekordboxTrackIDByObjectID
{
    if (_p_rekordboxTrackIDByObjectID) {
        return _p_rekordboxTrackIDByObjectID;
    }

    NSMutableDictionary *tracksByRekordboxTrackID = self.p_tracksByRekordboxTrackID;
    NSArray *allIDs = tracksByRekordboxTrackID.allKeys;

    _p_rekordboxTrackIDByObjectID = [NSMutableDictionary dictionaryWithCapacity:allIDs.count];
    NxAAssertNotNil(_p_rekordboxTrackIDByObjectID);

    for (NSString *trackID in allIDs) {
        NxATrack *track = tracksByRekordboxTrackID[trackID];

        _p_rekordboxTrackIDByObjectID[track.objectID] = trackID;
    }

    return _p_rekordboxTrackIDByObjectID;
}

- (NSString *)p_nextAvailableRekordboxTrackID
{
    if (self.p_lastRekordboxTrackID == NSIntegerMax) {
        NSUInteger lastUsedID = 1;
        NSArray *allIDs = self.p_rekordboxTrackIDByObjectID.allValues;

        for (NSString *trackID in allIDs) {
            NSUInteger trackIDInt = trackID.integerValue;
            if (trackIDInt > lastUsedID) {
                lastUsedID = trackIDInt;
            }
        }

        // -- The next ID will be the one right after the highest one we found.
        ++lastUsedID;
        
        self.p_lastRekordboxTrackID = lastUsedID;
    }

    NSString *nextID = [NSString stringWithFormat:@"%ld", self.p_lastRekordboxTrackID];
    ++self.p_lastRekordboxTrackID;

    return nextID;
}

- (NSMutableDictionary *)p_tracksByRekordboxTrackID
{
    if (!_p_tracksByRekordboxTrackID) {
        _p_tracksByRekordboxTrackID = [NSMutableDictionary dictionary];
    }

    return _p_tracksByRekordboxTrackID;
}

- (NSMutableDictionary *)p_tracksByRekordboxTrackLocation
{
    if (!_p_tracksByRekordboxTrackLocation) {
        _p_tracksByRekordboxTrackLocation = [NSMutableDictionary dictionary];
    }

    return _p_tracksByRekordboxTrackLocation;
}

#pragma mark Instance Methods

- (NxATrack *)p_trackForRekordboxPlaylistIDEntryElement:(NSXMLElement *)element
{
    NxAAssertNotNil(element);

    NSXMLNode *keyAttribute = [element attributeForName:@"Key"];
    if (!keyAttribute) {
        return nil;
    }

    NxATrack *track = [self p_trackForRekordboxTrackID:keyAttribute.stringValue];
    return track;
}

- (NxATrack *)p_trackForRekordboxPlaylistPathEntryElement:(NSXMLElement *)element
{
    NxAAssertNotNil(element);

    NSString *location = [[element attributeForName:@"Key"] stringValue];
    NxAAssertNotNil(location);

    NxATrack *track = [self p_trackForRekordboxFileLocation:location];
    return track;
}

- (void)p_trackForRekordboxTrackID:(NSString *)trackID
                                is:(NxATrack *)track
{
    self.p_tracksByRekordboxTrackID[trackID] = track;
}

- (NxATrack *)p_trackForRekordboxTrackID:(NSString *)trackID
{
    return self.p_tracksByRekordboxTrackID[trackID];
}

- (void)p_trackForRekordboxFileLocation:(NSString *)location
                                     is:(NxATrack *)track
{
    self.p_tracksByRekordboxTrackLocation[location] = track;
}

- (NxATrack *)p_trackForRekordboxFileLocation:(NSString *)location
{
    return self.p_tracksByRekordboxTrackLocation[location];
}

- (instancetype)initPrivate
{
    return [super init];
}

- (instancetype)init
{
    ALog(@"We only create this type of objects via our factory methods.");
    return nil;
}

- (NSString *)rekordboxTrackIDFor:(NxATrack *)track
{
    NxAAssertNotNil(track);

    NSString *trackID = self.p_rekordboxTrackIDByObjectID[track.objectID];
    return trackID;
}

- (NSString *)rekordboxTrackIDOrNextFreeTrackIDFor:(NxATrack *)track
{
    NSString *trackID = [self rekordboxTrackIDFor:track];
    if (!trackID) {
        trackID = self.p_nextAvailableRekordboxTrackID;
        NxAAssertNotNil(trackID);

        NSManagedObjectID *objectID = track.objectID;
        NxAAssertNotNil(objectID);

        self.p_rekordboxTrackIDByObjectID[objectID] = trackID;
    }

    return trackID;
}

#pragma mark NxAParserImplementation Protocol

@synthesize parseOnlyToForceUpdateSavedValues;

+ (instancetype)implementationWithContext:(NSManagedObjectContext *)context
                        loggingMessagesIn:(NxAMessageLog *)log
{
    NxARekordboxParser *implementation = [[NxARekordboxParser alloc] initPrivate];

    implementation.p_context = context;
    implementation.p_lastRekordboxTrackID = NSIntegerMax;
    implementation.p_log = log;
    implementation.parseOnlyToForceUpdateSavedValues = NO;

    return implementation;
}

+ (NSString *)nameOfTheApplication
{
    return @"Rekordbox";
}

+ (NSString *)versionPrefix
{
    return @"RBX";
}

- (Class)parserClass
{
    return [self class];
}

- (NxAMessageLog *)log
{
    return self.p_log;
}

- (id)rootFolderSource
{
    NxAAssertNotNil(self.p_collectionFile);

    NSXMLElement *playlistsElement = [self.p_collectionFile.rootElement elementForName:@"PLAYLISTS"];
    NSXMLElement *rootFolderElement = [playlistsElement elementForName:@"NODE"];
    NxAAssertNotNil(rootFolderElement);

    return rootFolderElement;
}

- (id)collectionSource
{
    NxAAssertNotNil(self.p_collectionFile);

    NSXMLElement *collectionElement = [self.p_collectionFile.rootElement elementForName:@"COLLECTION"];
    NxAAssertNotNil(collectionElement);

    return collectionElement;
}

- (NSDate *)collectionModificationDate
{
    NxAAssertNotNil(_collectionModificationDate);
    return _collectionModificationDate;
}

- (NSDate *)rootCrateModificationDate
{
    NxAAssertNotNil(_rootCrateModificationDate);
    return _rootCrateModificationDate;
}

- (BOOL)openCollection
{
    // -- Get the location of the Rekordbox collection file.
    NSURL *collectionFileURL = self.collectionFileURL;

    // -- Read the collection file into an XML document.
    self.p_collectionFile = [NxARekordboxParser p_xmlDocumentFromFileAtURL:collectionFileURL
                                                         loggingMessagesIn:self.log];
    if (self.p_collectionFile) {
        NSXMLElement *rootElement = self.p_collectionFile.rootElement;
        if ([NxARekordboxParser p_collectionRootElementIsValid:rootElement
                                             loggingMessagesIn:self.log]) {
            self.collectionModificationDate = [NSFileManager modificationDateForFileAtURL:collectionFileURL];
            self.rootCrateModificationDate = self.collectionModificationDate;
            return YES;
        }
    }

    NSError *error = [NSError errorWithCode:NxACollectionParseErrorCode
                                description:NSLocalizedString(@"Error reading your Rekordbox collection.",
                                                              @"Generic error message when reading the collection fails.")
                         recoverySuggestion:NSLocalizedString(@"Try to import a new XML file by dropping it in your Documents folder.",
                                                              @"Tells the user try import a new file.")];
    [self.log addError:error];

    return NO;
}

- (NSString *)nameOfPlaylistSource:(id)source
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NSXMLElement *playlistElement = source;
    NSString *name = [[playlistElement attributeForName:@"Name"] stringValue];
    return name;
}

- (NSString *)nameOfFolderEntrySource:(id)source
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NSXMLElement *entryElement = source;
    NSString *name = [[entryElement attributeForName:@"Name"] stringValue];
    return name;
}

- (BOOL)folderEntrySourceIsAPlaylist:(id)source
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NSXMLElement *entryElement = source;
    return [[[entryElement attributeForName:@"Type"] stringValue] isEqualToString:@"1"];
}

- (BOOL)folderEntrySourceIsAFolder:(id)source
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NSXMLElement *entryElement = source;
    return [[[entryElement attributeForName:@"Type"] stringValue] isEqualToString:@"0"];
}

- (NSString *)locationPathForTrackSource:(id)source
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NSXMLElement *trackElement = source;

    NSString *location = [[trackElement attributeForName:@"Location"] stringValue];

    NSString *locationCorrected = [location stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];

    NSUInteger prefixLengthToRemove = [@"file:/" length];
    if ([locationCorrected hasPrefix:@"file://localhost"]) {
        prefixLengthToRemove = [@"file://localhost" length];
    }

    locationCorrected = [locationCorrected substringFromIndex:prefixLengthToRemove];
    NxAAssertNotNil(locationCorrected);

    return locationCorrected;
}

- (void)trackForLocation:(NSString *)location
                      is:(NxATrack *)track
{
    NxAAssertNotNil(location);
    NxAAssertNotNil(track);
    [self p_trackForRekordboxFileLocation:location is:track];
}

- (NSDate *)modificationDateForTrackSource:(id)source
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    return self.collectionModificationDate;
}

- (NxAParsingResultFlags)parseTrackSource:(id)source
                       whichWasModifiedOn:(NSDate *)modificationDate
                                 forTrack:(NxATrack *)track
{
    NxAAssertNotNil(track);
    NxAAssertNotNil(modificationDate);
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NSXMLElement *trackElement = source;

    if (!self.parseOnlyToForceUpdateSavedValues) {
        NSString *rekordboxTrackID = [[trackElement attributeForName:@"TrackID"] stringValue];
        NxAAssertNotNil(rekordboxTrackID);
        [self p_trackForRekordboxTrackID:rekordboxTrackID is:track];
    }

#if NXADebugBreakAtSpecificTrack
    NSString *debugTitle = [[trackElement attributeForName:@"TITLE"] stringValue];
    NxALaunchDebuggerIf([debugTitle hasPrefix:NxADebugBreakAtTrackTitleStartsWith]);
#endif

    NxAParsingResultFlags result = NxAParsedObjectWasUpToDate;

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseStringFromXMLAttributeNamed:@"Name"
                                                                       inElement:trackElement
                                                              whichWasModifiedOn:modificationDate
                                                                   usingTagClass:[NxATrackTitle class]
                                                                       andParser:self],
                                         self.log, @"Title");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseMultipleStringsFromXMLAttributeNamed:@"Artist"
                                                                              seperatedBy:@", "
                                                                                     orBy:@","
                                                                                inElement:trackElement
                                                                       whichWasModifiedOn:modificationDate
                                                                            usingTagClass:[NxATrackArtistCredit class]
                                                                                andParser:self],
                                         self.log, @"Artists");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseStringFromXMLAttributeNamed:@"Album"
                                                                       inElement:trackElement
                                                              whichWasModifiedOn:modificationDate
                                                                   usingTagClass:[NxATrackAlbum class]
                                                                       andParser:self],
                                         self.log, @"Album");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseIntegerFromXMLAttributeNamed:@"TrackNumber"
                                                                        inElement:trackElement
                                                               whichWasModifiedOn:modificationDate
                                                                    usingTagClass:[NxATrackNumber class]
                                                                        andParser:self],
                                         self.log, @"Album Track Number");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseStringFromXMLAttributeNamed:@"Mix"
                                                                       inElement:trackElement
                                                              whichWasModifiedOn:modificationDate
                                                                   usingTagClass:[NxATrackMixName class]
                                                                       andParser:self],
                                         self.log, @"Remix Name");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseMultipleStringsFromXMLAttributeNamed:@"Genre"
                                                                              seperatedBy:@" / "
                                                                                     orBy:@"/"
                                                                                inElement:trackElement
                                                                       whichWasModifiedOn:modificationDate
                                                                            usingTagClass:[NxATrackGenre class]
                                                                                andParser:self],
                                         self.log, @"Genre");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseStringFromXMLAttributeNamed:@"Label"
                                                                       inElement:trackElement
                                                              whichWasModifiedOn:modificationDate
                                                                   usingTagClass:[NxATrackRecordLabel class]
                                                                       andParser:self],
                                         self.log, @"Record Label");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseStringFromXMLAttributeNamed:@"Comments"
                                                                       inElement:trackElement
                                                              whichWasModifiedOn:modificationDate
                                                                   usingTagClass:[NxATrackComments class]
                                                                       andParser:self],
                                         self.log, @"Comments");

    if (!self.parseOnlyToForceUpdateSavedValues) {
        NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                             [track ifNotAlreadyPresentAddNilTagsForTagClass:[NxATrackTag class]
                                                                               createdOnDate:modificationDate],
                                             self.log, @"Track Tags (set to nil)");
    }

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseMultipleStringsFromXMLAttributeNamed:@"Remixer"
                                                                              seperatedBy:@", "
                                                                                     orBy:@","
                                                                                inElement:trackElement
                                                                       whichWasModifiedOn:modificationDate
                                                                            usingTagClass:[NxATrackRemixerCredit class]
                                                                                andParser:self],
                                         self.log, @"Remixers");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseMultipleStringsFromXMLAttributeNamed:@"Composer"
                                                                              seperatedBy:@", "
                                                                                     orBy:@","
                                                                                inElement:trackElement
                                                                       whichWasModifiedOn:modificationDate
                                                                            usingTagClass:[NxATrackProducerCredit class]
                                                                                andParser:self],
                                         self.log, @"Producers");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseIntegerFromXMLAttributeNamed:@"PlayCount"
                                                                        inElement:trackElement
                                                               whichWasModifiedOn:modificationDate
                                                                    usingTagClass:[NxATrackPlayCount class]
                                                                        andParser:self],
                                         self.log, @"Playcount");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseIntegerFromXMLAttributeNamed:@"Rating"
                                                                 convertingItWith:^(NSInteger value) {
                                                                     // -- 0 star = "0", 1 star = "51", 2 stars = "102", 3 stars = "153", 4 stars = "204", 5 stars = "255"
                                                                     if (value < 0) {
                                                                         return (NSInteger)0;
                                                                     }

                                                                     value = value / 51;
                                                                     value = value > 5 ? 5 : value;

                                                                     return value;
                                                                 }
                                                                        inElement:trackElement
                                                               whichWasModifiedOn:modificationDate
                                                                    usingTagClass:[NxATrackRating class]
                                                                        andParser:self],
                                         self.log, @"Rating");


    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseDateFromXMLAttributeNamed:@"DateAdded"
                                                                   usingFormat:@"yyyy-M-d"
                                                                     inElement:trackElement
                                                            whichWasModifiedOn:modificationDate
                                                                 usingTagClass:[NxATrackDateAdded class]
                                                                     andParser:self],
                                         self.log, @"Date Added");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseRekordboxKeyElement:@"Tonality"
                                                             fromElement:trackElement
                                                     elementModifiedDate:modificationDate
                                                                  parser:self],
                                         self.log, @"Rekordbox Key");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseRekordboxMarkerElementsIn:trackElement
                                                           elementModifiedDate:modificationDate
                                                                        parser:self],
                                         self.log, @"Markers");

    if (!self.parseOnlyToForceUpdateSavedValues) {
        NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                             [track ifNotAlreadyPresentAddNilTagsForTagClass:[NxATrackDateReleased class]
                                                                               createdOnDate:modificationDate],
                                             self.log, @"Date Released (set to nil)");
    }

    return result;
}

- (NxATrack *)trackForPlaylistEntrySource:(id)entry
{
    NxAAssertNotNil(entry);
    NxAAssertTrue([entry isKindOfClass:[NSXMLElement class]]);

    NSXMLElement *entryElement = entry;
    NSXMLElement *parentElement = (NSXMLElement *)entryElement.parent;

    NSXMLNode *keyTypeAttribute = [parentElement attributeForName:@"KeyType"];
    NxAAssertNotNil(keyTypeAttribute);

    BOOL keyTypeIsAnID = [keyTypeAttribute.stringValue isEqualToString:@"0"];

    if (keyTypeIsAnID) {
        return [self p_trackForRekordboxPlaylistIDEntryElement:entryElement];
    }
    else {
        return [self p_trackForRekordboxPlaylistPathEntryElement:entryElement];
    }
}

- (NSArray *)childrenOfCollectionSource:(id)source
{
    NxAAssertNotNil(source);
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NxAAssertTrue([[source name] isEqualToString:@"COLLECTION"]);

    NSArray *children = [source elementsForName:@"TRACK"];
    return children;
}

- (NSArray *)childrenOfPlaylistSource:(id)source
{
    NxAAssertNotNil(source);
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NxAAssertTrue([[[source attributeForName:@"Type"] stringValue] isEqualToString:@"1"]);

    NSArray *children = [source elementsForName:@"TRACK"];
    return children;
}

- (NSArray *)childrenOfFolderSource:(id)source
{
    NxAAssertNotNil(source);
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NxAAssertTrue([[[source attributeForName:@"Type"] stringValue] isEqualToString:@"0"]);

    NSArray *children = [source elementsForName:@"NODE"];
    return children;
}

- (NSSet *)removeAndReturnValidChildrenFromCollectionSource:(id)source
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    return [NSSet setWithArray:[source removeAndReturnChildren]];
}

- (NSSet *)removeAndReturnValidChildrenFromPlaylistSource:(id)source
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    return [NSSet setWithArray:[source removeAndReturnChildren]];
}

- (NSSet *)removeAndReturnValidChildrenFromFolderSource:(id)source
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    return [NSSet setWithArray:[source removeAndReturnChildren]];
}

- (NSUInteger)numberOfChildrenLeftInCollectionSource:(id)source
{
    NxAAssertNotNil(source);
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NxAAssertTrue([[source name] isEqualToString:@"COLLECTION"]);

    NSXMLElement *collectionElement = source;
    return collectionElement.children.count;
}

- (NSUInteger)numberOfChildrenLeftInFolderSource:(id)source
{
    NxAAssertNotNil(source);
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NxAAssertTrue([[source name] isEqualToString:@"NODE"]);

    NSXMLElement *folderElement = source;
    return folderElement.children.count;
}

- (void)updateTrackSource:(id)source
                 forTrack:(NxATrack *)track
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NSXMLElement *trackElement = (NSXMLElement *)source;

    [track updateXMLStringAttribute:@"Name" forElement:trackElement tagClass:[NxATrackTitle class]];
    [track updateXMLMultipleStringAttribute:@"Artist" forElement:trackElement seperator:@", " tagClass:[NxATrackArtistCredit class]];
    [track updateXMLMultipleStringAttribute:@"Composer" forElement:trackElement seperator:@", " tagClass:[NxATrackProducerCredit class]];
    [track updateXMLMultipleStringAttribute:@"Remixer" forElement:trackElement seperator:@", " tagClass:[NxATrackRemixerCredit class]];
    [track updateXMLMultipleStringAttribute:@"Genre" forElement:trackElement seperator:@", " tagClass:[NxATrackGenre class]];
    [track updateXMLStringAttribute:@"Comments" forElement:trackElement tagClass:[NxATrackComments class]];
    [track updateXMLStringAttribute:@"Label" forElement:trackElement tagClass:[NxATrackRecordLabel class]];
    [track updateXMLStringAttribute:@"Mix" forElement:trackElement tagClass:[NxATrackMixName class]];
    [track updateXMLIntegerAttribute:@"PlayCount" forElement:trackElement tagClass:[NxATrackPlayCount class] exportBlock:nil];
    [track updateXMLIntegerAttribute:@"TrackNumber" forElement:trackElement tagClass:[NxATrackNumber class] exportBlock:nil];
    [track updateXMLStringAttribute:@"Album" forElement:trackElement tagClass:[NxATrackAlbum class]];

    [track updateXMLIntegerAttribute:@"Rating" forElement:trackElement tagClass:[NxATrackRating class] exportBlock:^(NSInteger value) {
        return value * 51;
    }];

    [track updateXMLDateAttribute:@"DateAdded" forElement:trackElement tagClass:[NxATrackDateAdded class] timeZone:nil format:@"yyyy-MM-dd"];
    [track updateXMLDateAttribute:@"Year" forElement:trackElement tagClass:[NxATrackDateReleased class] timeZone:nil format:@"yyyy"];

    NxATrackKey *key = nil;
    NSArray *keys = [NxATrackKey tagsIn:track withName:NxAKeyPropertyRekordboxKeyName];
    if (keys.count) {
        NxAAssertTrue(keys.count == 1);

        if (keys.count > 1) {
            NxAWarning *warning = [NxAWarning warningWithDescription:NSLocalizedString(@"in your collection with more than one Rekordbox keys. Rekordbox only supports one.",
                                                                                       @"")];
            [self.log addWarning:warning inContextWithPerObjectWarningInfo:@""];
        }

        key = keys[0];
    }

    [NxATrackKey updateRekordboxKeyElementForKey:key inElement:trackElement];

    NSArray *markers = [NxAMarkerProperty tagsIn:track];
    if (markers.count) {
        NSDecimalNumber *averageBPM = [NxAGridMarkerProperty averageBpmForMarkers:markers];
        [trackElement addAttributeWithName:@"AverageBpm" stringValue:[NSString stringWithFormat:@"%3.2f", averageBPM.floatValue]];

        [NxAMarkerProperty updateRekordboxMarkers:markers trackElement:trackElement log:self.log];
    }
    else {
        [trackElement removeAttributeForName:@"AverageBpm"];
    }
}

- (id)emptyTrackSourceFor:(NxATrack *)track
{
    NSXMLElement *trackElement = [NSXMLElement elementWithName:@"TRACK"];

    NxAOSXAudioFile *localFile = (NxAOSXAudioFile *)track.fileForCurrentPlatform;

    NSString *trackID = [self rekordboxTrackIDOrNextFreeTrackIDFor:track];
    [trackElement addAttributeWithName:@"TrackID" stringValue:trackID];

    NSString *location = [localFile.url.path stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    NxAAssertTrue([location hasPrefix:@"/"]);

    NSString *newLocation = [NSString stringWithFormat:@"file://localhost%@", location];
    [trackElement addAttributeWithName:@"Location" stringValue:newLocation];

    return trackElement;
}

- (id)emptyPlaylistSourceFor:(NxACrate *)playlist
{
    NSXMLElement *playlistElement = [NSXMLElement elementWithName:@"NODE"];
    [playlistElement addAttributeWithName:@"Name" stringValue:playlist.name];
    [playlistElement addAttributeWithName:@"Type" stringValue:@"1"];
    [playlistElement addAttributeWithName:@"KeyType" stringValue:@"0"];
    [playlistElement addAttributeWithName:@"Entries" stringValue:@"0"];
    return playlistElement;
}

- (id)emptyFolderSourceFor:(NxACrate *)folder
{
    NSXMLElement *folderElement = [NSXMLElement elementWithName:@"NODE"];
    [folderElement addAttributeWithName:@"Name" stringValue:folder.name];
    [folderElement addAttributeWithName:@"Type" stringValue:@"0"];
    [folderElement addAttributeWithName:@"Count" stringValue:@"0"];
    return folderElement;
}

- (id)playlistEntrySourceFor:(NxATrack *)track
{
    NSString *trackID = [self rekordboxTrackIDFor:track];
    if (!trackID) {
        NxAWarning *warning = [NxAWarning warningWithDescription:NSLocalizedString(@"in a Rekordbox playlist with an invalid track.",
                                                                                   @"")];
        [self.log addWarning:warning inContextWithPerObjectWarningInfo:track.description];
        
        return nil;
    }

    NSXMLElement *playlistEntryElement = [NSXMLElement elementWithName:@"TRACK"];
    [playlistEntryElement addAttributeWithName:@"Key" stringValue:trackID];

    return playlistEntryElement;
}

- (void)addTrackSource:(id)source
             asChildOf:(id)collectionSource
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NxAAssertTrue([collectionSource isKindOfClass:[NSXMLElement class]]);
    [collectionSource addChild:source];
}

- (void)addPlaylistEntrySource:(id)source
                     asChildOf:(id)playlistSource
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NxAAssertTrue([playlistSource isKindOfClass:[NSXMLElement class]]);
    [playlistSource addChild:source];
}

- (void)addFolderEntrySource:(id)source
                   asChildOf:(id)folderSource
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NxAAssertTrue([folderSource isKindOfClass:[NSXMLElement class]]);
    [folderSource addChild:source];
}

- (void)setEntriesCountForCollectionSource:(id)source
                                        to:(NSNumber *)count
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NSXMLElement *collectionElement = source;
    [[collectionElement attributeForName:@"Entries"] setStringValue:count.stringValue];
}

- (void)setEntriesCountForPlaylistSource:(id)source
                                      to:(NSNumber *)count
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NSXMLElement *playlistElement = (NSXMLElement *)source;

    NxAAssertTrue([[[playlistElement attributeForName:@"Type"] stringValue] isEqualToString:@"1"]);

    NSXMLNode *countNode = [playlistElement attributeForName:@"Entries"];
    NxAAssertNotNil(countNode);

    [countNode setStringValue:count.stringValue];
}

- (void)setEntriesCountForFolderSource:(id)source
                                    to:(NSNumber *)count
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NSXMLElement *folderElement = (NSXMLElement *)source;

    NxAAssertTrue([[[folderElement attributeForName:@"Type"] stringValue] isEqualToString:@"0"]);

    NSXMLNode *countNode = [folderElement attributeForName:@"Count"];
    NxAAssertNotNil(countNode);

    [countNode setStringValue:count.stringValue];
}

- (void)deleteCollectionEntrySources:(NSSet *)objects
{
    [NSXMLElement deleteXMLElements:objects];
}

- (void)deletePlaylistEntrySources:(NSSet *)objects
{
    [NSXMLElement deleteXMLElements:objects];
}

- (void)deleteFolderEntrySources:(NSSet *)objects
{
    [NSXMLElement deleteXMLElements:objects];
}

- (void)saveCollection
{
    // -- If we did change stuff, we mark the file as changed by us.
    NSXMLElement *rootElement = self.p_collectionFile.rootElement;
    NSXMLElement *productElement = [rootElement elementForName:@"PRODUCT"];

    [productElement removeAttributeForName:@"Name"];
    [productElement removeAttributeForName:@"Version"];
    [productElement removeAttributeForName:@"Company"];

    NxAAppDelegate *appDelegate = (NxAAppDelegate *)[NSApp delegate];

    [productElement addAttributeWithName:@"Name" stringValue:@"Rekord Buddy"];
    [productElement addAttributeWithName:@"Version" stringValue:appDelegate.appVersionAsString];
    [productElement addAttributeWithName:@"Company" stringValue:@"www.nextaudiolabs.com"];

    NSData *xmlData = [self.p_collectionFile XMLDataWithOptions:NSXMLNodePrettyPrint];
    if (xmlData == nil) {
        NSError *error = [NSError errorWithInternalCause:NSLocalizedString(@"Couldn't convert Rekordbox collection to XML document during output.",
                                                                           @"Tell the user we couldn't convert his collection for saving.")];
        [self.log addError:error];

        return;
    }

    // TODO: Make sure the file modification date is set to the last modified tag.

    NSError *systemError = nil;
    if(![xmlData writeToURL:self.collectionFileURL options:NSDataWritingAtomic error:&systemError]) {
        NSError *error = [NSError errorWithCode:NxACollectionParseErrorCode
                                    description:[NSString stringWithFormat:NSLocalizedString(@"Error writing to Rekordbox collection file '%@'.",
                                                                                             @"."),
                                                 self.collectionFileURL.path]
                             recoverySuggestion:nil
                                 additionalInfo:NSLocalizedString(@"Make sure that the file is not write protected or being accessed by another program.",
                                                                  @".")];
        [self.log addError:error];

        return;
    }
}

@end
