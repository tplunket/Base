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

#import "Track Collection/Parsers/NxATraktorParser.h"
#import "NSXMLDocument+NxAUtility.h"
#import "NSXMLElement+NxAUtility.h"
#import "Track Collection/AudioFiles/OSX/NxAOSXAudioFile.h"
#import "NxATrack+TraktorParsing.h"
#import "NxATaggedObject+XMLParsing.h"
#import "NxATrack+NxAOSXAudioFile.h"
#import "NxAProgressViewController.h"
#import "Track Collection/Tags/Crate Properties/NxATrackProperty.h"
#import "NxATagEntry.h"
#import "NxATrackTag.h"
#import "NxATrackRecordLabel.h"
#import "Track Collection/Tags/Track Properties/NxATrackAlbum.h"
#import "NxATrackGenre.h"
#import "NxATrackArtistCredit.h"
#import "NxATrackRemixerCredit.h"
#import "NxATrackProducerCredit.h"
#import "Track Collection/Tags/Track Properties/NxATrackKey.h"
#import "NxATrackTitle.h"
#import "NxATrackComments.h"
#import "NxATrackMixName.h"
#import "NxATrackNumber.h"
#import "NxATrackRating.h"
#import "NxATrackPlayCount.h"
#import "NxATrackDateAdded.h"
#import "NxATrackDateReleased.h"
#import "NxATrackDateModified.h"
#import "NxATrackBitRate.h"
#import "NxATrackBitDepth.h"
#import "NxATrackSampleRate.h"
#import "NxATrackSize.h"
#import "NxATrackTitle.h"
#import "Track Collection/Tags/Properties/NxAArtworkProperty.h"
#import "NxATrackKey+TraktorParsing.h"
#import "NxAMarkerProperty+TraktorParsing.h"
#import "NSFileManager+NxAUtility.h"
#import "Track Collection/Parsers/Internal/NxATraktorParser.h"
#import "NSManagedObjectContext+NxAUtility.h"
#import "Track Collection/Crates/NxACrate.h"
#import "NxAAppDelegate.h"
#import "NSError+NxAUtility.h"
#import "NxATrack+NxAWarningContextObject.h"

#pragma mark Debug Defines

// -- This flag can be used to stop in the debugger while reading a specific track.
#define NXADebugBreakAtSpecificTrack                            0
#define NxADebugBreakAtTrackTitleStartsWith                     @""

#pragma mark - Implementation

@implementation NxATraktorParser

#pragma mark Class Methods

/*!
 * Read a Traktor collection file and convert it to an XML document.
 * \param fileURL Location of the Traktor collection file.
 * \param log Message log for any errors and warnings.
 * \return The XML document is reading succeeded or nil otherwise.
 */
+ (NSXMLDocument *)p_xmlDocumentFromFile:(NSURL *)fileURL
                                     log:(NxAMessageLog *)log
{
    NxAAssertNotNil(fileURL);

    // -- Read the file into a binary data blob.
    NSData *xmlData = [NSData dataWithContentsOfURL:fileURL];
    if (!xmlData) {
        NSError *error = [NSError errorWithCode:NxACollectionParseErrorCode
                                    description:NSLocalizedString(@"Error reading your Traktor collection.",
                                                                  @"Generic error message when reading the collection fails.")
                             recoverySuggestion:NSLocalizedString(@"It seems like the collection file is missing or is invalid.",
                                                                  @"Tells the user that the file might be invalid or missing.")
                                 additionalInfo:[NSString stringWithFormat:NSLocalizedString(@"Can't read Traktor collection file:\n\t'%@'",
                                                                                             @"Formatted string to let the user know which file we were unable to load."),
                                                 fileURL.path]];
        [log addError:error];

        return nil;
    }

    NSMutableCharacterSet *illegalCharacters = [NSMutableCharacterSet characterSetWithRange:NSMakeRange(0, 0x1f)];
    [illegalCharacters removeCharactersInRange:NSMakeRange(10, 10)];

    // -- Try to convert the blob into a valid XML document, escaping out any illegal characters.
    BOOL foundIllegalCharacters = NO;
    NSXMLDocument *xmlDoc = [[NSXMLDocument alloc] initWithDataAndIgnoreIllegalCharacters:xmlData
                                                                             illegalChars:illegalCharacters
                                                                        foundIllegalChars:&foundIllegalCharacters
                                                                                      log:log];
    if (!xmlDoc) {
        NSError *error = [NSError errorWithCode:NxACollectionParseErrorCode
                                    description:NSLocalizedString(@"Error reading your Traktor collection.",
                                                                  @"Generic error message when reading the collection fails.")
                             recoverySuggestion:NSLocalizedString(@"It seems like the collection file is missing or is invalid.",
                                                                  @"Tells the user that the file might be invalid or missing.")
                                 additionalInfo:[NSString stringWithFormat:NSLocalizedString(@"Error reading Traktor NML file:\n\t'%@'.",
                                                                                             @"Formatted string to let the user know which file we were unable to read."),
                                                 fileURL.path]];
        [log addError:error];

        return nil;
    }

    if (foundIllegalCharacters) {
        // -- If we've found illegal characters, let the user know.
        NxAWarning *warning = [NxAWarning warningWithDescription:[NSString stringWithFormat:NSLocalizedString(@"Found illegal characters in your Traktor collection file. These characters are not valid XML and have been ignored.",
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
    if (![rootElement.name isEqualToString:@"NML"]) {
        NSError *error = [NSError errorWithCode:NxACollectionParseErrorCode
                                    description:NSLocalizedString(@"Error reading your Traktor collection.",
                                                @"Generic error message when reading the collection fails.")
                             recoverySuggestion:nil
                                 additionalInfo:NSLocalizedString(@"Traktor collection file does not appear to be a valid Traktor NML file.",
                                                                  @"Tells the user the Traktor collection might be invalid.")];
        [log addError:error];

        return NO;
    }

    // -- Get the version number.
    NSString *versionValue = [[rootElement attributeForName:@"VERSION"] stringValue];
    if (!versionValue) {
        NSError *error = [NSError errorWithCode:NxACollectionParseErrorCode
                                    description:NSLocalizedString(@"Error reading your Traktor collection.",
                                                                  @"Generic error message when reading the collection fails.")
                             recoverySuggestion:nil
                                 additionalInfo:NSLocalizedString(@"Can't find 'VERSION' attribute in Traktor NML file.",
                                                                  @"Tells the user that the VERSION attribute is missing from the NML file.")];
        [log addError:error];

        return NO;
    }

    // -- Make sure the version number is supported.
    NSInteger versionNumber = versionValue.integerValue;
    if ((versionNumber < 14) && (versionNumber > 16)) {
        NSError *error = [NSError errorWithCode:NxACollectionParseErrorCode
                                    description:NSLocalizedString(@"Your Traktor collection file version is unsupported.",
                                                                  @"Tells the user the version number of the Traktor file is unsupported.")
                             recoverySuggestion:NSLocalizedString(@"The App Store might have an updated version of Rekord Buddy.",
                                                                  @"Tells the user to look on the App Store for an update.")
                                 additionalInfo:[NSString stringWithFormat:NSLocalizedString(@"Traktor collection is using unsupported Traktor NML version '%@'.",
                                                                                             @"Tells the user what version number their Traktor file is."),
                                                 versionValue]
                                  additionalUrl:@"http://nextaudiolabs.com"];
        [log addError:error];

        return NO;
    }

    return YES;
}

+ (BOOL)p_entryIsATrackEntry:(NSXMLElement *)entryElement
{
    NSXMLElement *loopInfoElement = [entryElement elementForName:@"LOOPINFO"];
    return loopInfoElement == nil;
}

+ (BOOL)p_folderEntryIsAnValidFolderEntry:(NSXMLElement *)folderElement
{
    NSString *name = [[folderElement attributeForName:@"NAME"] stringValue];
    return ![name hasPrefix:@"_"];
}

#pragma mark Accessors

- (NSMutableDictionary *)p_traktorPlaylistPathByTrackLocation
{
    if (!_p_traktorPlaylistPathByTrackLocation) {
        _p_traktorPlaylistPathByTrackLocation = [NSMutableDictionary dictionary];
    }

    return _p_traktorPlaylistPathByTrackLocation;
}

- (NSMutableDictionary *)p_traktorPlaylistPathByTrackID
{
    if (!_p_traktorPlaylistPathByTrackID) {
        _p_traktorPlaylistPathByTrackID = [NSMutableDictionary dictionary];
    }

    return _p_traktorPlaylistPathByTrackID;
}

- (NSMutableDictionary *)p_trackByTraktorPlaylistPath
{
    if (!_p_trackByTraktorPlaylistPath) {
        _p_trackByTraktorPlaylistPath = [NSMutableDictionary dictionary];
    }

    return _p_trackByTraktorPlaylistPath;
}

- (NSXMLElement *)p_trackCollectionElement
{
    NxAAssertNotNil(self.p_collectionFile);

    NSArray *collectionElements = [self.p_collectionFile.rootElement elementsForName:@"COLLECTION"];
    NxAAssertTrue(collectionElements.count == 1);

    NSXMLElement *collectionElement = collectionElements.firstObject;
    NxAAssertNotNil(collectionElement);

    return collectionElement;
}

#pragma mark Instance Methods

- (NxAParsingResultFlags)p_parseTraktorInfoElement:(NSXMLElement *)infoElement
                               traktorModifiedDate:(NSDate *)traktorModifiedDate
                                          forTrack:(NxATrack *)track
{
    NxAParsingResultFlags result = NxAParsedObjectWasUpToDate;

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseStringFromXMLAttributeNamed:@"LABEL"
                                                                       inElement:infoElement
                                                              whichWasModifiedOn:traktorModifiedDate
                                                                   usingTagClass:[NxATrackRecordLabel class]
                                                                       andParser:self],
                                         self.log, @"Record Label");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseStringFromXMLAttributeNamed:@"MIX"
                                                                       inElement:infoElement
                                                              whichWasModifiedOn:traktorModifiedDate
                                                                   usingTagClass:[NxATrackMixName class]
                                                                       andParser:self],
                                         self.log, @"Remix Name");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseStringFromXMLAttributeNamed:@"COMMENT"
                                                                       inElement:infoElement
                                                              whichWasModifiedOn:traktorModifiedDate
                                                                   usingTagClass:[NxATrackComments class]
                                                                       andParser:self],
                                         self.log, @"Comments");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseMultipleStringsFromXMLAttributeNamed:@"GENRE"
                                                                              seperatedBy:@" / "
                                                                                     orBy:@"/"
                                                                                inElement:infoElement
                                                                       whichWasModifiedOn:traktorModifiedDate
                                                                            usingTagClass:[NxATrackGenre class]
                                                                                andParser:self],
                                         self.log, @"Genres");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseMultipleStringsFromXMLAttributeNamed:@"REMIXER"
                                                                              seperatedBy:@", "
                                                                                     orBy:@","
                                                                                inElement:infoElement
                                                                       whichWasModifiedOn:traktorModifiedDate
                                                                            usingTagClass:[NxATrackRemixerCredit class]
                                                                                andParser:self],
                                         self.log, @"Remixers");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseMultipleStringsFromXMLAttributeNamed:@"PRODUCER"
                                                                              seperatedBy:@", "
                                                                                     orBy:@","
                                                                                inElement:infoElement
                                                                       whichWasModifiedOn:traktorModifiedDate
                                                                            usingTagClass:[NxATrackProducerCredit class]
                                                                                andParser:self],
                                         self.log, @"Producers");

    // -- The 'RATING' attribute is displayed in Traktor as the 'Comments2' column.
    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseMultipleStringsFromXMLAttributeNamed:@"RATING"
                                                                              seperatedBy:@", "
                                                                                     orBy:@","
                                                                                inElement:infoElement
                                                                       whichWasModifiedOn:traktorModifiedDate
                                                                            usingTagClass:[NxATrackTag class]
                                                                                andParser:self],
                                         self.log, @"Rating");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseDateFromXMLAttributeNamed:@"RELEASE_DATE"
                                                                   usingFormat:@"yyyy/M/d"
                                                                     inElement:infoElement
                                                            whichWasModifiedOn:traktorModifiedDate
                                                                 usingTagClass:[NxATrackDateReleased class]
                                                                     andParser:self],
                                         self.log, @"Date Released");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseDateFromXMLAttributeNamed:@"IMPORT_DATE"
                                                                   usingFormat:@"yyyy/M/d"
                                                                     inElement:infoElement
                                                            whichWasModifiedOn:traktorModifiedDate
                                                                 usingTagClass:[NxATrackDateAdded class]
                                                                     andParser:self],
                                         self.log, @"Date Added");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseIntegerFromXMLAttributeNamed:@"PLAYCOUNT"
                                                                        inElement:infoElement
                                                               whichWasModifiedOn:traktorModifiedDate
                                                                    usingTagClass:[NxATrackPlayCount class]
                                                                        andParser:self],
                                         self.log, @"Playcount");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseIntegerFromXMLAttributeNamed:@"RANKING"
                                                                 convertingItWith:^(NSInteger value) {
                                                                     // -- 0 star = "0", 1 star = "51", 2 stars = "102",
                                                                     // -- 3 stars = "153", 4 stars = "204", 5 stars = "255"
                                                                     if (value < 0) {
                                                                         return (NSInteger)0;
                                                                     }

                                                                     value = value / 51;
                                                                     value = value > 5 ? 5 : value;

                                                                     return value;
                                                                 }
                                                                        inElement:infoElement
                                                               whichWasModifiedOn:traktorModifiedDate
                                                                    usingTagClass:[NxATrackRating class]
                                                                        andParser:self],
                                         self.log, @"Rating");

    return result;
}

- (void)p_updateTraktorAlbumElementInEntry:(NSXMLElement *)entryElement
                                  forTrack:(NxATrack *)track
{
    NSXMLElement *albumElement = [entryElement elementForName:@"ALBUM"];
    NxATrackAlbum *albumTag = [[NxATrackAlbum tagsIn:track] firstObject];
    NxATrackNumber *trackNumberTag = [[NxATrackNumber tagsIn:track] firstObject];

    if (!albumTag && !trackNumberTag) {
        if (albumElement) {
            // -- If we have no tag and an element, we remove the element.
            [entryElement removeChildAtIndex:albumElement.index];
        }

        // -- If we have no tag to output, we return.
        return;
    }
    else if (!albumElement) {
        // -- If we have data to output and no element, we create a new element for it.
        albumElement = [NSXMLElement elementWithName:@"ALBUM"];
        [entryElement addChild:albumElement];
    }

    NSXMLNode *trackNumberAttribute = [albumElement attributeForName:@"TRACK"];
    if (trackNumberAttribute) {
        if (!trackNumberTag) {
            [albumElement removeAttributeForName:@"TRACK"];
        }
        else {
            [trackNumberAttribute setStringValue:trackNumberTag.stringValue];
        }
    }
    else if (trackNumberTag) {
        [albumElement addAttributeWithName:@"TRACK" stringValue:trackNumberTag.stringValue];
    }

    NSXMLNode *titleAttribute = [albumElement attributeForName:@"TITLE"];
    if (titleAttribute) {
        if (!albumTag) {
            [albumElement removeAttributeForName:@"TITLE"];
        }
        else {
            [titleAttribute setStringValue:albumTag.stringValue];
        }
    }
    else if (albumTag) {
        [albumElement addAttributeWithName:@"TITLE" stringValue:albumTag.stringValue];
    }
}

- (void)p_updateTraktorInfoElement:(NSXMLElement *)infoElement
                          forTrack:(NxATrack *)track
{
    NxAAssertNotNil(infoElement);

    [track updateXMLMultipleStringAttribute:@"GENRE"
                                 forElement:infoElement
                                  seperator:@", "
                                   tagClass:[NxATrackGenre class]];
    [track updateXMLStringAttribute:@"LABEL"
                         forElement:infoElement
                           tagClass:[NxATrackRecordLabel class]];
    [track updateXMLStringAttribute:@"MIX"
                         forElement:infoElement
                           tagClass:[NxATrackMixName class]];
    [track updateXMLStringAttribute:@"COMMENT"
                         forElement:infoElement
                           tagClass:[NxATrackComments class]];

    // -- The 'RATING' attribute is displayed in Traktor as the 'Comments2' column.
    [track updateXMLMultipleStringAttribute:@"RATING"
                                 forElement:infoElement
                                  seperator:@", "
                                   tagClass:[NxATrackTag class]];

    [track updateXMLMultipleStringAttribute:@"REMIXER"
                                 forElement:infoElement
                                  seperator:@", "
                                   tagClass:[NxATrackRemixerCredit class]];
    [track updateXMLMultipleStringAttribute:@"PRODUCER"
                                 forElement:infoElement
                                  seperator:@", "
                                   tagClass:[NxATrackProducerCredit class]];

    [track updateXMLIntegerAttribute:@"PLAYCOUNT"
                          forElement:infoElement
                            tagClass:[NxATrackPlayCount class]
                         exportBlock:nil];

    [track updateXMLIntegerAttribute:@"RANKING"
                          forElement:infoElement
                            tagClass:[NxATrackRating class]
                         exportBlock:^(NSInteger value) {
                             // -- 0 star = "0", 1 star = "51", 2 stars = "102", 3 stars = "153", 4 stars = "204", 5 stars = "255"
                             if (value < 0) {
                                 return (NSInteger)0;
                             }

                             value = value * 51;
                             value = value > 255 ? 255 : value;

                             return value;
                         }];

    [track updateXMLDateAttribute:@"IMPORT_DATE"
                       forElement:infoElement
                         tagClass:[NxATrackDateAdded class]
                         timeZone:nil
                           format:@"yyyy/M/d"];

    [track updateXMLDateAttribute:@"RELEASE_DATE"
                       forElement:infoElement
                         tagClass:[NxATrackDateReleased class]
                         timeZone:nil
                           format:@"yyyy/M/d"];
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

#pragma mark NxAParserImplementation Protocol

@synthesize parseOnlyToForceUpdateSavedValues;

+ (instancetype)implementationWithContext:(NSManagedObjectContext *)context
                        loggingMessagesIn:(NxAMessageLog *)log
{
    NxATraktorParser *implementation = [[NxATraktorParser alloc] initPrivate];

    implementation.p_context = context;
    implementation.p_log = log;
    implementation.parseOnlyToForceUpdateSavedValues = NO;

    return implementation;
}

+ (NSString *)nameOfTheApplication
{
    return @"Traktor";
}

+ (NSString *)versionPrefix
{
    return @"TRK";
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

    NSXMLElement *playlistElements = [self.p_collectionFile.rootElement elementForName:@"PLAYLISTS"];
    NSXMLElement *rootFolderElement = [playlistElements elementForName:@"NODE"];

    if (!rootFolderElement) {
        NSError *error = [NSError errorWithInternalCause:@"Error finding the root playlist in your Traktor collection."];
        [self.log addError:error];

        return NO;
    }

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
    // -- Get the location of the Traktor collection file.
    NSURL *collectionFileURL = self.collectionFileURL;

    // -- Read the collection file into an XML document.
    NxAAssertNil(self.p_collectionFile);
    self.p_collectionFile = [NxATraktorParser p_xmlDocumentFromFile:collectionFileURL
                                                                log:self.log];
    if (self.p_collectionFile) {
        NSXMLElement *rootElement = self.p_collectionFile.rootElement;
        if ([NxATraktorParser p_collectionRootElementIsValid:rootElement
                                           loggingMessagesIn:self.log]) {
            self.collectionModificationDate = [NSFileManager modificationDateForFileAtURL:collectionFileURL];
            NxAAssertNotNil(self.collectionModificationDate);

            self.rootCrateModificationDate = self.collectionModificationDate;

            return YES;
        }
    }

    return NO;
}

- (NSString *)nameOfPlaylistSource:(id)source
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NSXMLElement *playlistElement = source;
    NSString *name = [[playlistElement attributeForName:@"NAME"] stringValue];
    return name;
}

- (NSString *)nameOfFolderEntrySource:(id)source
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NSXMLElement *playlistElement = source;
    NSString *name = [[playlistElement attributeForName:@"NAME"] stringValue];
    return name;
}

- (BOOL)folderEntrySourceIsAPlaylist:(id)source
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NSXMLElement *entryElement = source;
    return [[[entryElement attributeForName:@"TYPE"] stringValue] isEqualToString:@"PLAYLIST"];
}

- (BOOL)folderEntrySourceIsAFolder:(id)source
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NSXMLElement *entryElement = source;
    return [[[entryElement attributeForName:@"TYPE"] stringValue] isEqualToString:@"FOLDER"];
}

+ (NSString *)p_traktorPathToLocalPath:(NSString *)location
{
    // -- there seem to be some inconsistant occurance of a BOM (Byte Ordering Mark) character in file Trakor's
    // -- locations exports, in order to be able to match track entries and playlists keys, we need to filter those out.
    NSString *BOMString = [NSString stringWithFormat:@"%C", (unsigned short)0xfeff];
    location = [location stringByReplacingOccurrencesOfString:BOMString withString:@""];

    NSMutableArray *pathElements = [[location componentsSeparatedByString:@"/:"] mutableCopy];
    for (NSUInteger index = 0; index < pathElements.count; ++index) {
        pathElements[index] = [pathElements[index] stringByReplacingOccurrencesOfString:@"//" withString:@":"];
    }

    NSString *locationPath = [pathElements componentsJoinedByString:@"/"];
    NxAAssertNotNil(locationPath);

    return locationPath;
}

- (NSString *)locationPathForTrackSource:(id)source
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NSXMLElement *entryElement = source;

    NSXMLElement *locationElement = [entryElement elementForName:@"LOCATION"];
    NSString *directory = [[locationElement attributeForName:@"DIR"] stringValue];
    NSString *file = [[locationElement attributeForName:@"FILE"] stringValue];
    if (!locationElement  || !directory || !file) {
        // -- something went wrong in getting the location element, we can skip this track
        NxAWarning *warning = [NxAWarning warningWithDescription:@"Found a track in your Traktor collection with an invalid 'LOCATION' property."];
        [self.log addWarning:warning];
        return nil;
    }

    NSString *volume = [[locationElement attributeForName:@"VOLUME"] stringValue];
    NxAAssertNotNil(volume);

    NSString *location = nil;
    NSString *traktorLocation = nil;
    NSString *locationUsedInTraktorPlaylistEntries = nil;

    if (!volume.length) {
        traktorLocation = [NSString stringWithFormat:@"%@%@", directory, file];
        locationUsedInTraktorPlaylistEntries = [NSString stringWithFormat:@"%@%@", directory, file];
    }
    else {
        if ([volume isEqualToString:[NxAOSXAudioFile bootVolumeName]]) {
            traktorLocation = [NSString stringWithFormat:@"%@%@", directory, file];
            locationUsedInTraktorPlaylistEntries = [NSString stringWithFormat:@"%@%@%@", volume, directory, file];
        }
        else if ([[NxAOSXAudioFile otherVolumeNames] containsObject:volume]) {
            traktorLocation = [NSString stringWithFormat:@"/:Volumes/:%@%@%@", volume, directory, file];
            locationUsedInTraktorPlaylistEntries = [NSString stringWithFormat:@"%@%@%@", volume, directory, file];
        }
        else {
            NSString *testTraktorLocation = [NSString stringWithFormat:@"/:%@%@%@", volume, directory, file];
            NSString *testLocation = [NxATraktorParser p_traktorPathToLocalPath:testTraktorLocation];
            NSFileManager *fileManager = [NxAOSXAudioFile fileManager];

            if ([fileManager fileExistsAtPath:testLocation]) {
                traktorLocation = testTraktorLocation;
                location = testLocation;
                locationUsedInTraktorPlaylistEntries = [NSString stringWithFormat:@"%@%@%@", volume, directory, file];
            }
            else {
                testTraktorLocation = [NSString stringWithFormat:@"/:Users/:%@%@%@", volume, directory, file];
                testLocation = [NxATraktorParser p_traktorPathToLocalPath:testTraktorLocation];
                if ([fileManager fileExistsAtPath:[NxATraktorParser p_traktorPathToLocalPath:testTraktorLocation]]) {
                    traktorLocation = testTraktorLocation;
                    location = testLocation;
                    locationUsedInTraktorPlaylistEntries = [NSString stringWithFormat:@"%@%@%@", volume, directory, file];
                }
                else {
                    // -- something went wrong in getting the location element, we can skip this track
                    NxAWarning *warning = [NxAWarning warningWithDescription:[NSString stringWithFormat:@"Found a track entry in your Traktor collection with a missing file (%@).",
                                                                              [NSString stringWithFormat:@"/:Volumes/:%@%@%@", volume, directory, file]]];
                    [self.log addWarning:warning];
                    return nil;
                }
            }
        }

    }

    if (!location) {
        location = [NxATraktorParser p_traktorPathToLocalPath:traktorLocation];
    }

    NxAAssertNotNil(location);
    NxAAssertNotNil(locationUsedInTraktorPlaylistEntries);

    self.p_traktorPlaylistPathByTrackLocation[location] = locationUsedInTraktorPlaylistEntries;

    return location;
}

- (void)trackForLocation:(NSString *)location
                      is:(NxATrack *)track
{
    NxAAssertNotNil(location);
    NxAAssertNotNil(track);

    NSString *playlistPath = self.p_traktorPlaylistPathByTrackLocation[location];
    NxAAssertNotNil(playlistPath);

    self.p_trackByTraktorPlaylistPath[playlistPath] = track;
    self.p_traktorPlaylistPathByTrackID[track.objectID] = playlistPath;
}

- (NSDate *)modificationDateForTrackSource:(id)source
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NSXMLElement *entryElement = source;

    NSDate *trackModificationDate = [NxATrack parseTraktorModifiedDateFromEntryElement:entryElement
                                                                     loggingMessagesIn:self.log];
    return trackModificationDate;
}

- (NxAParsingResultFlags)parseTrackSource:(id)source
                       whichWasModifiedOn:(NSDate *)modificationDate
                                 forTrack:(NxATrack *)track
{
    NxAAssertNotNil(track);
    NxAAssertNotNil(modificationDate);
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NSXMLElement *entryElement = source;

#if NXADebugBreakAtSpecificTrack
    NSString *debugTitle = [[entryElement attributeForName:@"TITLE"] stringValue];
    NxALaunchDebuggerIf([debugTitle hasPrefix:NxADebugBreakAtTrackTitleStartsWith]);
#endif

    NxAParsingResultFlags result = NxAParsedObjectWasUpToDate;

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseStringFromXMLAttributeNamed:@"TITLE"
                                                                       inElement:entryElement
                                                              whichWasModifiedOn:modificationDate
                                                                   usingTagClass:[NxATrackTitle class]
                                                                       andParser:self],
                                         self.log, @"Title");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseMultipleStringsFromXMLAttributeNamed:@"ARTIST"
                                                                              seperatedBy:@", "
                                                                                     orBy:@","
                                                                                inElement:entryElement
                                                                       whichWasModifiedOn:modificationDate
                                                                            usingTagClass:[NxATrackArtistCredit class]
                                                                                andParser:self],
                                         self.log, @"Artists");

    // -- TODO: We should only create this is we need it
    NSXMLElement *infoElement = [entryElement elementForName:@"INFO"];
    if (self.parseOnlyToForceUpdateSavedValues) {
        NxAAssertNotNil(infoElement);
    }
    else if (!infoElement) {
        infoElement = [NSXMLElement elementWithName:@"INFO"];
        [entryElement addChild:infoElement];
    }

    result |= [self p_parseTraktorInfoElement:infoElement
                          traktorModifiedDate:modificationDate
                                     forTrack:track];
    if (self.log.hasAnError) {
        return result;
    }

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseTraktorKeyElement:@"MUSICAL_KEY"
                                                           fromElement:entryElement
                                                   elementModifiedDate:modificationDate
                                                                parser:self],
                                         self.log, @"Traktor Key");

    if (self.log.hasAnError) {
        return result;
    }

    NSXMLElement *albumElement = [entryElement elementForName:@"ALBUM"];

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseStringFromXMLAttributeNamed:@"TITLE"
                                                                       inElement:albumElement
                                                              whichWasModifiedOn:modificationDate
                                                                   usingTagClass:[NxATrackAlbum class]
                                                                       andParser:self],
                                         self.log, @"Album Name");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseIntegerFromXMLAttributeNamed:@"TRACK"
                                                                        inElement:albumElement
                                                               whichWasModifiedOn:modificationDate
                                                                    usingTagClass:[NxATrackNumber class]
                                                                        andParser:self],
                                         self.log, @"Album Track Number");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [track parseTraktorMarkerElementsIn:entryElement
                                                         elementModifiedDate:modificationDate
                                                                      parser:self],
                                         self.log, @"Markers");

    return result;
}

- (NxATrack *)trackForPlaylistEntrySource:(id)source
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NSXMLElement *entryElement = source;

    NSXMLElement *primaryKeyElement = [entryElement elementForName:@"PRIMARYKEY"];
    NxAAssertNotNil(primaryKeyElement);

    NSXMLNode *primaryKeyTypeAttribute = [primaryKeyElement attributeForName:@"TYPE"];
    if (primaryKeyTypeAttribute) {
        if ([primaryKeyTypeAttribute.stringValue isEqualToString:@"TRACK"]) {
            NSXMLNode *playlistKeyTypeAttribute = [primaryKeyElement attributeForName:@"KEY"];
            if (playlistKeyTypeAttribute) {
                NSString *playlistKey = playlistKeyTypeAttribute.stringValue;
                return self.p_trackByTraktorPlaylistPath[playlistKey];
            }
        }
    }

    return nil;
}

- (NSArray *)childrenOfCollectionSource:(id)source
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NxAAssertTrue([[source name] isEqualToString:@"COLLECTION"]);

    NSArray *trackSources = [source elementsForName:@"ENTRY"];
    NSMutableArray *tracksFound = [NSMutableArray arrayWithCapacity:trackSources.count];

    for (NSXMLElement *entryElement in trackSources) {
        if (![NxATraktorParser p_entryIsATrackEntry:entryElement]) {
            continue;
        }

        [tracksFound addObject:entryElement];
    }

    return tracksFound;
}

- (NSArray *)childrenOfPlaylistSource:(id)source
{
    NxAAssertNotNil(source);
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NSXMLElement *nodeElement = source;
    NxAAssertTrue([[[nodeElement attributeForName:@"TYPE"] stringValue] isEqualToString:@"PLAYLIST"]);

    NSXMLElement *playlistElement = [nodeElement elementForName:@"PLAYLIST"];
    if (!playlistElement) {
        return @[ ];
    }

    NSArray *children = [playlistElement elementsForName:@"ENTRY"];
    return children;
}

- (NSArray *)childrenOfFolderSource:(id)source
{
    NxAAssertNotNil(source);
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NSXMLElement *folderElement = source;
    NxAAssertTrue([[[folderElement attributeForName:@"TYPE"] stringValue] isEqualToString:@"FOLDER"]);

    NSXMLElement *subNodesElement = [folderElement elementForName:@"SUBNODES"];
    if (!subNodesElement) {
        return @[ ];
    }

    NSArray *subNodes = [subNodesElement elementsForName:@"NODE"];
    NSMutableArray *subNodesFound = [NSMutableArray arrayWithCapacity:subNodes.count];

    for (NSXMLElement *subNode in subNodes) {
        if (![NxATraktorParser p_folderEntryIsAnValidFolderEntry:subNode]) {
            continue;
        }

        [subNodesFound addObject:subNode];
    }

    return subNodesFound;
}

- (NSSet *)removeAndReturnValidChildrenFromCollectionSource:(id)source
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NxAAssertTrue([[source name] isEqualToString:@"COLLECTION"]);
    NSXMLElement *collectionElement = source;

    NSArray *allSources = [collectionElement removeAndReturnChildren];
    NSMutableSet *trackSources = [NSMutableSet set];
    for (NSXMLElement *entryElement in allSources) {
        if ([NxATraktorParser p_entryIsATrackEntry:entryElement]) {
            [trackSources addObject:entryElement];
        }
        else {
            [collectionElement addChild:entryElement];
        }
    }

    return trackSources;
}

- (NSSet *)removeAndReturnValidChildrenFromPlaylistSource:(id)source
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NSXMLElement *nodeElement = source;
    NSXMLElement *playlistElement = [nodeElement elementForName:@"PLAYLIST"];
    return [NSSet setWithArray:[playlistElement removeAndReturnChildren]];
}

- (NSSet *)removeAndReturnValidChildrenFromFolderSource:(id)source
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NSXMLElement *nodeElement = source;
    NSXMLElement *subNodesElement = [nodeElement elementForName:@"SUBNODES"];

    NSArray *allSources = [subNodesElement removeAndReturnChildren];
    NSMutableSet *folderEntrySources = [NSMutableSet set];
    for (NSXMLElement *entryElement in allSources) {
        if ([NxATraktorParser p_folderEntryIsAnValidFolderEntry:entryElement]) {
            [folderEntrySources addObject:entryElement];
        }
        else {
            [subNodesElement addChild:entryElement];
        }
    }

    return folderEntrySources;
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
    NSXMLElement *nodeElement = source;
    NSXMLElement *subNodesElement = [nodeElement elementForName:@"SUBNODES"];
    NxAAssertNotNil(subNodesElement);
    return subNodesElement.children.count;
}

- (void)updateTrackSource:(id)source
                 forTrack:(NxATrack *)track
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NSXMLElement *trackElement = source;

    [self.log startWarningContextWith:track];

    [track updateXMLDateAttribute:@"MODIFIED_DATE" forElement:trackElement tagClass:[NxATrackDateModified class] timeZone:@"Europe/Berlin" format:@"yyyy/M/d"];

    NxATrackDateModified *modifiedDate = [[NxATrackDateModified tagsIn:track] firstObject];
    NxAAssertNotNil(modifiedDate);

    NSString *stringDate = [modifiedDate stringValueWithTimeZone:@"Europe/Berlin"
                                                          format:@"HH:mm:ss"];
    NSArray *components = [stringDate componentsSeparatedByString:@":"];
    NxAAssertTrue(components.count == 3);
    NSInteger numberOfSecondsPastMidnight = ([components[0] integerValue] * 60 * 60) +
    ([components[1] integerValue] * 60) +
    [components[2] integerValue];
    NSString *modificationDateTime = [NSString stringWithFormat:@"%ld", numberOfSecondsPastMidnight];

    NSXMLNode *attribute = [trackElement attributeForName:@"MODIFIED_TIME"];
    if (attribute) {
        [attribute setStringValue:modificationDateTime];
    }
    else {
        [trackElement addAttributeWithName:@"MODIFIED_TIME" stringValue:modificationDateTime];
    }

    [track updateXMLStringAttribute:@"TITLE" forElement:trackElement tagClass:[NxATrackTitle class]];
    [track updateXMLMultipleStringAttribute:@"ARTIST" forElement:trackElement seperator:@", " tagClass:[NxATrackArtistCredit class]];

    [self p_updateTraktorAlbumElementInEntry:trackElement forTrack:track];

    NSXMLElement *modInfoElement = [trackElement elementForName:@"MODIFICATION_INFO"];
    if (!modInfoElement) {
        modInfoElement = [NSXMLElement elementWithName:@"MODIFICATION_INFO"];
        [trackElement addChild:modInfoElement];
    }

    NSXMLNode *authorTypeAttribute = [modInfoElement attributeForName:@"AUTHOR_TYPE"];
    if (!authorTypeAttribute) {
        [modInfoElement addAttributeWithName:@"AUTHOR_TYPE" stringValue:@"user"];
    }
    else {
        [authorTypeAttribute setStringValue:@"user"];
    }

    NSArray *keys = [track nonNilTagsWithType:[NxATrackKey tagType] andName:NxAKeyPropertyTraktorKeyName];
    if (keys.count > 1) {
        NxAWarning *warning = [NxAWarning warningWithDescription:NSLocalizedString(@"in your collection with more than one Traktor keys. Traktor only supports one.",
                                                                                   @"")];
        [self.log addWarning:warning inContextWithPerObjectWarningInfo:@""];
    }

    NxATrackKey *key = keys.firstObject;

    NSXMLElement *infoElement = [trackElement elementForName:@"INFO"];
    if (!infoElement) {
        // -- If we don't already have an info element, we add one.
        infoElement = [NSXMLElement elementWithName:@"INFO"];
        [trackElement addChild:infoElement];
    }

    [self p_updateTraktorInfoElement:infoElement forTrack:track];

    [key updateTraktorKeyElementInInfoElement:infoElement];

    NSArray *markers = [NxAMarkerProperty tagsIn:track];
    [NxAMarkerProperty updateTraktorMarkers:markers entryElement:trackElement log:self.log];

    [key updateTraktorKeyElementInEntry:trackElement];

    [self.log endWarningContext];
}

- (id)emptyTrackSourceFor:(NxATrack *)track
{
    NSXMLElement *entryElement = [NSXMLElement elementWithName:@"ENTRY"];

    NxAOSXAudioFile *localFile = (NxAOSXAudioFile *)track.fileForCurrentPlatform;
    NSString *location = localFile.url.path;
    NxAAssertTrue([location hasPrefix:@"/"]);

    NSMutableArray *pathElements = [[location componentsSeparatedByString: @"/"] mutableCopy];
    [pathElements removeObjectAtIndex:0];

    for (NSUInteger index = 0; index < pathElements.count; ++index) {
        pathElements[index] = [pathElements[index] stringByReplacingOccurrencesOfString:@":" withString:@"//"];
    }

    NSMutableArray *playlistPathElements = pathElements.mutableCopy;

    if ([playlistPathElements[0] isEqualToString:@"Volumes"]) {
        [playlistPathElements removeObjectAtIndex:0];
    }
    else {
        [playlistPathElements insertObject:[NxAOSXAudioFile bootVolumeName] atIndex:0];
    }

    NSString *playlistPath = [playlistPathElements componentsJoinedByString:@"/:"];

    self.p_traktorPlaylistPathByTrackID[track.objectID] = playlistPath;

    NSString *volume;
    if ([pathElements[0] isEqualToString:@"Volumes"]) {
        volume = pathElements[1];

        [pathElements removeObjectAtIndex:1];
        [pathElements removeObjectAtIndex:0];
    }
    else {
        volume = [NxAOSXAudioFile bootVolumeName];
    }

    NSString *file = pathElements.lastObject;
    [pathElements removeLastObject];

    NSString *dir = pathElements.count ? [NSString stringWithFormat:@"/:%@/:", [pathElements componentsJoinedByString:@"/:"]] : @"/:";

    NSXMLElement *locationElement = [NSXMLElement elementWithName:@"LOCATION"];

    [locationElement addAttributeWithName:@"DIR" stringValue:dir];
    [locationElement addAttributeWithName:@"FILE" stringValue:file];

    if ((volume != nil) && volume.length) {
        [locationElement addAttributeWithName:@"VOLUME" stringValue:volume];
        [locationElement addAttributeWithName:@"VOLUMEID" stringValue:volume];
    }
    else {
        [locationElement addAttributeWithName:@"VOLUME" stringValue:@""];
        [locationElement addAttributeWithName:@"VOLUMEID" stringValue:@""];
    }

    [entryElement addChild:locationElement];
    
    return entryElement;
}

- (id)emptyPlaylistSourceFor:(NxACrate *)playlist
{
    NSXMLElement *playlistElement = [NSXMLElement elementWithName:@"NODE"];
    [playlistElement addAttributeWithName:@"TYPE" stringValue:@"PLAYLIST"];
    [playlistElement addAttributeWithName:@"NAME" stringValue:playlist.name];

    NSXMLElement *playlistSubElement = [NSXMLElement elementWithName:@"PLAYLIST"];
    NxAAssertNotNilDebug(playlistSubElement);
    [playlistElement addChild:playlistSubElement];

    [playlistSubElement addAttributeWithName:@"ENTRIES" stringValue:@"0"];
    [playlistSubElement addAttributeWithName:@"TYPE" stringValue:@"LIST"];

    return playlistElement;
}

- (id)emptyFolderSourceFor:(NxACrate *)folder
{
    NSXMLElement *folderEntry = [NSXMLElement elementWithName:@"NODE"];
    [folderEntry addAttributeWithName:@"TYPE" stringValue:@"FOLDER"];
    [folderEntry addAttributeWithName:@"NAME" stringValue:folder.name];

    NSXMLElement *childElement = [NSXMLElement elementWithName:@"SUBNODES"];
    [folderEntry addChild:childElement];

    [childElement addAttributeWithName:@"COUNT" stringValue:@"0"];

    return folderEntry;
}

- (id)playlistEntrySourceFor:(NxATrack *)track
{
    NSString *location = self.p_traktorPlaylistPathByTrackID[track.objectID];
    if (!location) {
        NxAWarning *warning = [NxAWarning warningWithDescription:NSLocalizedString(@"in a Traktor playlist with an invalid track.",
                                                                                   @"")];
        [self.log addWarning:warning inContextWithPerObjectWarningInfo:@""];

        return nil;
    }

    NSXMLElement *entryElement = [NSXMLElement elementWithName:@"ENTRY"];

    NSXMLElement *keyElement = [NSXMLElement elementWithName:@"PRIMARYKEY"];
    [keyElement addAttributeWithName:@"TYPE" stringValue:@"TRACK"];
    
    NSXMLNode *keyNode = [NSXMLNode attributeWithName:@"KEY" stringValue:location];
    [keyElement addAttribute:keyNode];

    [entryElement addChild:keyElement];

    return entryElement;
}

- (void)addTrackSource:(id)source
             asChildOf:(id)collectionSource
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NxAAssertTrue([[source name] isEqualToString:@"ENTRY"]);
    NxAAssertTrue([collectionSource isKindOfClass:[NSXMLElement class]]);
    NxAAssertTrue([[collectionSource name] isEqualToString:@"COLLECTION"]);

    [collectionSource addChild:source];
}

- (void)addPlaylistEntrySource:(id)source
                     asChildOf:(id)playlistSource
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NxAAssertTrue([[source name] isEqualToString:@"ENTRY"]);
    NxAAssertTrue([playlistSource isKindOfClass:[NSXMLElement class]]);
    NxAAssertTrue([[playlistSource name] isEqualToString:@"NODE"]);

    NSXMLElement *playlistElement = [playlistSource elementForName:@"PLAYLIST"];
    NxAAssertNotNil(playlistElement);

    [playlistElement addChild:source];
}

- (void)addFolderEntrySource:(id)source
                   asChildOf:(id)folderSource
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NxAAssertTrue([[source name] isEqualToString:@"NODE"]);
    NxAAssertTrue([folderSource isKindOfClass:[NSXMLElement class]]);
    NxAAssertTrue([[folderSource name] isEqualToString:@"NODE"]);

    NSXMLElement *subNodesElement = [folderSource elementForName:@"SUBNODES"];
    NxAAssertNotNil(subNodesElement);

    [subNodesElement addChild:source];
}

- (void)setEntriesCountForCollectionSource:(id)source
                                        to:(NSNumber *)count
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NxAAssertTrue([[source name] isEqualToString:@"COLLECTION"]);
    NSXMLElement *collectionElement = source;

    NSXMLNode *attribute = [collectionElement attributeForName:@"ENTRIES"];
    NxAAssertNotNil(attribute);

    [attribute setStringValue:count.stringValue];
}

- (void)setEntriesCountForPlaylistSource:(id)source
                                      to:(NSNumber *)count
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NxAAssertTrue([[source name] isEqualToString:@"NODE"]);
    NSXMLElement *nodeElement = source;

    NSXMLElement *playlistElement = [nodeElement elementForName:@"PLAYLIST"];
    NxAAssertNotNil(playlistElement);

    NSXMLNode *attribute = [playlistElement attributeForName:@"ENTRIES"];
    NxAAssertNotNil(attribute);

    [attribute setStringValue:count.stringValue];
}

- (void)setEntriesCountForFolderSource:(id)source
                                    to:(NSNumber *)count
{
    NxAAssertTrue([source isKindOfClass:[NSXMLElement class]]);
    NxAAssertTrue([[source name] isEqualToString:@"NODE"]);
    NSXMLElement *nodeElement = source;

    NSXMLElement *subNodesElement = [nodeElement elementForName:@"SUBNODES"];
    NxAAssertNotNil(subNodesElement);

    NSXMLNode *attribute = [subNodesElement attributeForName:@"COUNT"];
    NxAAssertNotNil(attribute);

    [attribute setStringValue:count.stringValue];
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
    NSXMLElement *headElement = [rootElement elementForName:@"HEAD"];

    [headElement removeAttributeForName:@"COMPANY"];
    [headElement removeAttributeForName:@"PROGRAM"];

    [headElement addAttributeWithName:@"COMPANY" stringValue:@"www.nextaudiolabs.com"];
    NxAAppDelegate *appDelegate = (NxAAppDelegate *)[NSApp delegate];
    NSString *appVersionString = [NSString stringWithFormat:@"Rekord Buddy %@", appDelegate.appVersionAsString];
    [headElement addAttributeWithName:@"PROGRAM" stringValue:appVersionString];

    NSData *xmlData = [self.p_collectionFile XMLDataWithOptions:NSXMLNodePrettyPrint];
    if (xmlData == nil) {
        NSError *error = [NSError errorWithInternalCause:NSLocalizedString(@"Couldn't convert Traktor collection to NML document during output.",
                                                                           @"Tell the user we couldn't convert his collection for saving.")];
        [self.log addError:error];

        return;
    }

    NSError *systemError = nil;
    if(![xmlData writeToURL:self.collectionFileURL options:NSDataWritingAtomic error:&systemError]) {
        NSError *error = [NSError errorWithCode:NxACollectionParseErrorCode
                                    description:[NSString stringWithFormat:NSLocalizedString(@"Error writing to Traktor collection file '%@'.",
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
