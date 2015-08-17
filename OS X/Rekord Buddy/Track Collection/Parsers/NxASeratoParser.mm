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

#import "Track Collection/Parsers/Internal/NxASeratoParser.h"
#import "Track Collection/AudioFiles/OSX/NxAOSXAudioFile.h"
#import "NxATrack+NxAOSXAudioFile.h"
#import "NxATrack+SeratoParsing.h"
#import "NxAProgressViewController.h"
#import "NSFileManager+NxAUtility.h"
#import "NSManagedObjectContext+NxAUtility.h"
#import "Track Collection/Crates/NxACrate.h"
#import "NxAAppDelegate.h"
#import "NxACrate+NxAWarningContextObject.h"

#include "SeratoDB/Crate.hpp"

#include <memory.h>

using namespace NxA;
using namespace std;

#pragma mark Debug Defines

// -- This flag can be used to stop in the debugger while reading a specific track.
#define NXADebugBreakAtSpecificTrack                            0
#define NxADebugBreakAtTrackTitleStartsWith                     @""

#pragma mark Implementation

@implementation NxASeratoDatabaseSource

+ (NxASeratoDatabaseSource *)sourceWithSeratoDatabasePointer:(NxA::Serato::Database&)other
{
    NxASeratoDatabaseSource *source = [[NxASeratoDatabaseSource alloc] init];

    source->value = new NxA::Serato::Database::Pointer(other.pointer());
    NxAAssertNotNil(source->value);

    return source;
}

- (NxA::Serato::Database&)getSeratoDatabase
{
    return *(*self->value);
}

- (void)dealloc
{
    delete self->value;
    self->value = nullptr;
}

- (id)copyWithZone:(NSZone *)zone
{
    NxASeratoDatabaseSource *newOne = [[NxASeratoDatabaseSource allocWithZone:zone] init];

    newOne->value = new NxA::Serato::Database::Pointer(*self->value);
    NxAAssertNotNil(newOne->value);

    return newOne;
}

- (BOOL)isEqual:(id)anObject
{
    return [anObject isKindOfClass:[NxASeratoDatabaseSource class]] &&
            self.hash == [(NxASeratoDatabaseSource*)anObject hash];
}

- (NSUInteger)hash
{
    return (NSUInteger)&(self.getSeratoDatabase);
}

@end

@implementation NxASeratoCrateSource

+ (NxASeratoCrateSource *)sourceWithSeratoCratePointer:(NxA::Serato::Crate&)other
{
    NxASeratoCrateSource *source = [[NxASeratoCrateSource alloc] init];

    source->value = new NxA::Serato::Crate::Pointer(other.pointer());
    NxAAssertNotNil(source->value);

    return source;
}

- (NxA::Serato::Crate&)getSeratoCrate
{
    return *(*self->value);
}

- (void)dealloc
{
    delete self->value;
    self->value = nullptr;
}

- (id)copyWithZone:(NSZone *)zone
{
    NxASeratoCrateSource *newOne = [[NxASeratoCrateSource allocWithZone:zone] init];

    newOne->value = new NxA::Serato::Crate::Pointer(*self->value);
    NxAAssertNotNil(newOne->value);

    return newOne;
}

- (BOOL)isEqual:(id)anObject
{
    return [anObject isKindOfClass:[NxASeratoCrateSource class]] &&
            self.hash == [(NxASeratoCrateSource*)anObject hash];
}

- (NSUInteger)hash
{
    return (NSUInteger)&(self.getSeratoCrate);
}

@end

@implementation NxASeratoTrackSource

+ (NxASeratoTrackSource *)sourceWithSeratoTrackPointer:(NxA::Serato::Track&)other
{
    NxASeratoTrackSource *source = [[NxASeratoTrackSource alloc] init];

    source->value = new NxA::Serato::Track::Pointer(other.pointer());
    NxAAssertNotNil(source->value);

    return source;
}

- (NxA::Serato::Track&)getSeratoTrack
{
    return *(*self->value);
}

- (void)dealloc
{
    delete self->value;
    self->value = nullptr;
}

- (id)copyWithZone:(NSZone *)zone
{
    NxASeratoTrackSource *newOne = [[NxASeratoTrackSource allocWithZone:zone] init];

    newOne->value = new NxA::Serato::Track::Pointer(*self->value);
    NxAAssertNotNil(newOne->value);

    return newOne;
}

- (BOOL)isEqual:(id)anObject
{
    return [anObject isKindOfClass:[NxASeratoTrackSource class]] &&
            self.hash == [(NxASeratoTrackSource*)anObject hash];
}

- (NSUInteger)hash
{
    return (NSUInteger)&(self.getSeratoTrack);
}

@end

@implementation NxASeratoTrackEntrySource

+ (NxASeratoTrackEntrySource *)sourceWithSeratoTrackEntryPointer:(NxA::Serato::TrackEntry&)other
{
    NxASeratoTrackEntrySource *source = [[NxASeratoTrackEntrySource alloc] init];

    source->value = new NxA::Serato::TrackEntry::Pointer(other.pointer());
    NxAAssertNotNil(source->value);

    return source;
}

- (NxA::Serato::TrackEntry&)getSeratoTrackEntry
{
    return *(*self->value);
}

- (void)dealloc
{
    delete self->value;
    self->value = nullptr;
}

- (id)copyWithZone:(NSZone *)zone
{
    NxASeratoTrackEntrySource *newOne = [[NxASeratoTrackEntrySource allocWithZone:zone] init];

    newOne->value = new NxA::Serato::TrackEntry::Pointer(*self->value);
    NxAAssertNotNil(newOne->value);

    return newOne;
}

- (BOOL)isEqual:(id)anObject
{
    return [anObject isKindOfClass:[NxASeratoTrackEntrySource class]] &&
            self.hash == [(NxASeratoTrackEntrySource*)anObject hash];
}

- (NSUInteger)hash
{
    return (NSUInteger)&(self.getSeratoTrackEntry);
}

@end

@implementation NxASeratoParser

#pragma mark Class Methods

+ (NSURL *)p_musicFolderURL
{
    NSURL *musicFolderURL = [NSURL fileURLWithPath:[NSString stringWithFormat:@"/Users/%@/Music", NSUserName()]];
    NxAAssertNotNil(musicFolderURL);
    return musicFolderURL;
}

+ (BOOL)seratoIsAvailable
{
    NSString* path = [[NxASeratoParser p_musicFolderURL] path];
    if (Serato::Database::containsAValidSeratoFolder(String::stringWith(path.UTF8String))) {
        return YES;
    }
    else {
        return NO;
    }
}

+ (NSString *)p_versionAsStringForDatabaseInSeratoFolderIn:(NSString *)seratoFolderParentDirectoryPath
{
    auto seratoFolderPath = Serato::Database::seratoFolderPathForFolder(String::stringWith(seratoFolderParentDirectoryPath.UTF8String));
    auto versionString = Serato::Database::versionAsStringForDatabaseIn(String::stringWith(seratoFolderPath));
    return [NSString stringWithUTF8String:versionString->toUTF8()];
}

+ (NSString *)currentSeratoDatabaseVersion
{
    NSString *musicFolderPath = [[NxASeratoParser p_musicFolderURL] path];
    NSString *versionString = [self p_versionAsStringForDatabaseInSeratoFolderIn:musicFolderPath];

    NSArray *components = [versionString componentsSeparatedByString:@"/"];
    return components[0];
}

#pragma mark Accessors

- (NSMutableDictionary *)p_tracksBySeratoTrackLocation
{
    if (!_p_tracksBySeratoTrackLocation) {
        _p_tracksBySeratoTrackLocation = [NSMutableDictionary dictionary];
    }

    return _p_tracksBySeratoTrackLocation;
}

#pragma mark Instance Methods

- (instancetype)initPrivate
{
    return [super init];
}

- (instancetype)init
{
    ALog(@"We only create this type of objects via our factory methods.");
    return nil;
}

- (void)p_trackForSeratoFileLocation:(NSString *)location
                                  is:(NxATrack *)track
{
    self.p_tracksBySeratoTrackLocation[location] = track;
}

- (NxATrack *)p_trackForSeratoFileLocation:(NSString *)location
{
    return self.p_tracksBySeratoTrackLocation[location];
}

#pragma mark NxAParserImplementation Protocol

@synthesize parseOnlyToForceUpdateSavedValues;

+ (instancetype)implementationWithContext:(NSManagedObjectContext *)context
                        loggingMessagesIn:(NxAMessageLog *)log
{
    NxASeratoParser *implementation = [[NxASeratoParser alloc] initPrivate];

    implementation.p_context = context;
    implementation.p_log = log;
    implementation.parseOnlyToForceUpdateSavedValues = NO;

    return implementation;
}

+ (NSString *)nameOfTheApplication
{
    return @"Serato";
}

+ (NSString *)versionPrefix
{
    return @"SER";
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
    return [NxASeratoCrateSource sourceWithSeratoCratePointer:(*self->p_database)->rootCrate()];
}

- (id)collectionSource
{
    return [NxASeratoDatabaseSource sourceWithSeratoDatabasePointer:(*self->p_database)];
}

- (NSDate *)collectionModificationDate
{
    time_t modificationDate = (*self->p_database)->databaseModificationDateInSecondsSince1970();
    return [NSDate dateWithTimeIntervalSince1970:modificationDate];
}

- (NSDate *)rootCrateModificationDate
{
    time_t modificationDate = (*self->p_database)->rootCrateModificationDateInSecondsSince1970();
    return [NSDate dateWithTimeIntervalSince1970:modificationDate];
}

- (BOOL)openCollection
{
    NSString *musicFolderPath = [[NxASeratoParser p_musicFolderURL] path];
    auto seratoFolderPath = Serato::Database::seratoFolderPathForFolder(String::stringWith(musicFolderPath.UTF8String));
    self->p_database = new Serato::Database::Pointer(Serato::Database::databaseWithFileAt(seratoFolderPath));

    return YES;
}

- (NSString *)nameOfPlaylistSource:(id)source
{
    NxAAssertNotNil(source);
    NxAAssertTrue([source isKindOfClass:[NxASeratoCrateSource class]]);

    auto& crate = [(NxASeratoCrateSource *)source getSeratoCrate];
    return [NSString stringWithUTF8String:crate.crateName().toUTF8()];
}

- (NSString *)nameOfFolderEntrySource:(id)source
{
    NxAAssertNotNil(source);
    NxAAssertTrue([source isKindOfClass:[NxASeratoCrateSource class]]);

    auto& crate = [(NxASeratoCrateSource *)source getSeratoCrate];
    return [NSString stringWithUTF8String:crate.crateName().toUTF8()];
}

- (BOOL)folderEntrySourceIsAPlaylist:(id)source
{
    NxAAssertNotNil(source);
    NxAAssertTrue([source isKindOfClass:[NxASeratoCrateSource class]]);

    auto& crate = [(NxASeratoCrateSource *)source getSeratoCrate];
    BOOL containsCrates = (crate.crates().length() != 0);
    BOOL containsTracks = (crate.trackEntries().length() != 0);
    return !containsCrates || containsTracks;
}

- (BOOL)folderEntrySourceIsAFolder:(id)source
{
    NxAAssertNotNil(source);
    NxAAssertTrue([source isKindOfClass:[NxASeratoCrateSource class]]);

    auto& crate = [(NxASeratoCrateSource *)source getSeratoCrate];
    BOOL containsCrates = (crate.crates().length() != 0);
    BOOL containsTracks = (crate.trackEntries().length() != 0);
    return containsCrates || !containsTracks;
}

- (NSString *)locationPathForTrackSource:(id)source
{
    NxAAssertNotNil(source);
    NxAAssertTrue([source isKindOfClass:[NxASeratoTrackSource class]]);

    auto& track = [(NxASeratoTrackSource *)source getSeratoTrack];
    NSString *path = [NSString stringWithUTF8String:track.trackFilePath()->toUTF8()];
    NxAAssertNotNil(path);

    return path;
}

- (void)trackForLocation:(NSString *)location
                      is:(NxATrack *)track
{
    NxAAssertNotNil(location);
    NxAAssertNotNil(track);

    // -- For now we don't need to do anything with this.
}

- (NSDate *)modificationDateForTrackSource:(id)source
{
    NxAAssertTrue([source isKindOfClass:[NxASeratoTrackSource class]]);

    auto& track = [(NxASeratoTrackSource *)source getSeratoTrack];

    NSDate *trackModificationDate = [NSDate dateWithTimeIntervalSince1970:track.dateModifiedInSecondsSinceJanuary1st1970()];
    return trackModificationDate;
}

- (NxAParsingResultFlags)parseTrackSource:(id)source
                       whichWasModifiedOn:(NSDate *)modificationDate
                                 forTrack:(NxATrack *)track
{
    NxAAssertNotNil(modificationDate);
    NxAAssertNotNil(track);
    NxAAssertTrue([source isKindOfClass:[NxASeratoTrackSource class]]);

    auto& seratoTrack = [(NxASeratoTrackSource *)source getSeratoTrack];

#if NXADebugBreakAtSpecificTrack
    NSString *debugTitle = [NSString stringWithUTF8String:seratoTrack->title()->c_str()];
    NxALaunchDebuggerIf([debugTitle hasPrefix:NxADebugBreakAtTrackTitleStartsWith]);
#endif

    NxAParsingResultFlags result = [track parseSeratoPropertiesFromTrack:seratoTrack
                                                          lastModifiedOn:modificationDate
                                                andUpdateTagsUsingParser:self];

    if (!self.parseOnlyToForceUpdateSavedValues) {
        NSString *trackLocation = [NSString stringWithUTF8String:seratoTrack.trackFilePath()->toUTF8()];
        [self p_trackForSeratoFileLocation:trackLocation is:track];
    }

    return result;
}

- (NxATrack *)trackForPlaylistEntrySource:(id)source
{
    NxAAssertNotNil(source);
    NxAAssertTrue([source isKindOfClass:[NxASeratoTrackEntrySource class]]);

    auto& trackEntry = [(NxASeratoTrackEntrySource *)source getSeratoTrackEntry];
    NSString *trackLocation = [NSString stringWithUTF8String:trackEntry.trackFilePath()->toUTF8()];

    return [self p_trackForSeratoFileLocation:trackLocation];
}

- (NSArray *)childrenOfCollectionSource:(id)source
{
    NxAAssertNotNil(source);
    NxAAssertTrue([source isKindOfClass:[NxASeratoDatabaseSource class]]);

    const Serato::Database& database = [(NxASeratoDatabaseSource *)source getSeratoDatabase];
    auto& tracks = database.tracks();
    NSMutableArray *result = [NSMutableArray arrayWithCapacity:tracks.length()];

    for(auto& trackSource : tracks) {
        NxASeratoTrackSource *value = [NxASeratoTrackSource sourceWithSeratoTrackPointer:trackSource];
        [result addObject:value];
    }

    return result;
}

- (NSArray *)childrenOfPlaylistSource:(id)source
{
    NxAAssertNotNil(source);
    NxAAssertTrue([source isKindOfClass:[NxASeratoCrateSource class]]);

    auto& seratoCrate = [(NxASeratoCrateSource *)source getSeratoCrate];
    auto& trackEntries = seratoCrate.trackEntries();
    NSMutableArray *result = [NSMutableArray arrayWithCapacity:trackEntries.length()];

    for(auto& trackEntry : trackEntries) {
        NxASeratoTrackEntrySource *value = [NxASeratoTrackEntrySource sourceWithSeratoTrackEntryPointer:*trackEntry];
        [result addObject:value];
    }

    return result;
}

- (NSArray *)childrenOfFolderSource:(id)source
{
    NxAAssertNotNil(source);
    NxAAssertTrue([source isKindOfClass:[NxASeratoCrateSource class]]);

    auto& seratoCrate = [(NxASeratoCrateSource *)source getSeratoCrate];
    auto& crates = seratoCrate.crates();
    NSMutableArray *result = [NSMutableArray arrayWithCapacity:crates.length()];

    for(auto& crate : crates) {
        NxASeratoCrateSource *value = [NxASeratoCrateSource sourceWithSeratoCratePointer:crate];
        [result addObject:value];
    }

    return result;
}

- (NSSet *)removeAndReturnValidChildrenFromCollectionSource:(id)source
{
    NxAAssertNotNil(source);
    NxAAssertTrue([source isKindOfClass:[NxASeratoDatabaseSource class]]);

    auto& database = [(NxASeratoDatabaseSource *)source getSeratoDatabase];
    auto tracks = database.removeAndReturnTracks();
    NSMutableSet *result = [NSMutableSet setWithCapacity:tracks->length()];

    for(auto& track : *tracks) {
        NxASeratoTrackSource *value = [NxASeratoTrackSource sourceWithSeratoTrackPointer:track];
        [result addObject:value];
    }

    return result;
}

- (NSSet *)removeAndReturnValidChildrenFromPlaylistSource:(id)source
{
    NxAAssertNotNil(source);
    NxAAssertTrue([source isKindOfClass:[NxASeratoCrateSource class]]);

    auto& seratoCrate = [(NxASeratoCrateSource *)source getSeratoCrate];
    auto trackEntries = seratoCrate.removeAndReturnTrackEntries();
    NSMutableSet *result = [NSMutableSet setWithCapacity:trackEntries->length()];

    for(auto& trackEntry : *trackEntries) {
        NxASeratoTrackEntrySource *value = [NxASeratoTrackEntrySource sourceWithSeratoTrackEntryPointer:*trackEntry];
        [result addObject:value];
    }

    return result;
}

- (NSSet *)removeAndReturnValidChildrenFromFolderSource:(id)source
{
    NxAAssertNotNil(source);
    NxAAssertTrue([source isKindOfClass:[NxASeratoCrateSource class]]);

    auto& seratoCrate = [(NxASeratoCrateSource *)source getSeratoCrate];
    auto crates = seratoCrate.removeAndReturnChildrenCrates();
    NSMutableSet *result = [NSMutableSet setWithCapacity:crates->length()];

    for(auto& crate : *crates) {
        NxASeratoCrateSource *value = [NxASeratoCrateSource sourceWithSeratoCratePointer:crate];
        [result addObject:value];
    }

    return result;
}

- (NSUInteger)numberOfChildrenLeftInCollectionSource:(id)source
{
    NxAAssertNotNil(source);

    // -- In Serato databases, we currently don't leave anything behind after removing items from a collection source.
    return 0;
}

- (NSUInteger)numberOfChildrenLeftInFolderSource:(id)source
{
    NxAAssertNotNil(source);

    // -- In Serato databases, we currently don't leave anything behind after removing items from a folder source.
    return 0;
}

- (void)updateTrackSource:(id)source
                 forTrack:(NxATrack *)track
{
    NxAAssertNotNil(source);
    NxAAssertNotNil(track);
    NxAAssertTrue([source isKindOfClass:[NxASeratoTrackSource class]]);

    auto& trackSource = [(NxASeratoTrackSource*)source getSeratoTrack];
    [track updateSeratoTrack:trackSource];
}

- (id)emptyTrackSourceFor:(NxATrack *)track
{
    NxAAssertNotNil(track);

    NxAOSXAudioFile *localFile = (NxAOSXAudioFile *)track.fileForCurrentPlatform;

    // TODO: This should deal with files on external drives.
    auto newTrack = Serato::Track::trackWithFileAtOnVolume(String::stringWith(localFile.url.path.UTF8String), String::stringWith("/"));
    return [NxASeratoTrackSource sourceWithSeratoTrackPointer:newTrack];
}

- (id)emptyPlaylistSourceFor:(NxACrate *)playlist
{
    NxAAssertNotNil(playlist);

    NSString *fullName = [playlist.description stringByReplacingOccurrencesOfString:@"->" withString:@"%%"];

    // TODO: This should deal with files on external drives.
    auto newCrate = Serato::Crate::crateWithNameInFolderOnVolume(String::stringWith(fullName.UTF8String),
                                                                 String::stringWith([[NSString stringWithFormat:@"/Users/%@/Music/_Serato_", NSUserName()] UTF8String]),
                                                                 String::stringWith("/"));
    return [NxASeratoCrateSource sourceWithSeratoCratePointer:newCrate];
}

- (id)emptyFolderSourceFor:(NxACrate *)folder
{
    NxAAssertNotNil(folder);

    NSString *fullName = [folder.description stringByReplacingOccurrencesOfString:@"->" withString:@"%%"];

    // TODO: This should deal with files on external drives.
    auto newCrate = Serato::Crate::crateWithNameInFolderOnVolume(String::stringWith(fullName.UTF8String),
                                                                 String::stringWith([[NSString stringWithFormat:@"/Users/%@/Music/_Serato_", NSUserName()] UTF8String]),
                                                                 String::stringWith("/"));
    return [NxASeratoCrateSource sourceWithSeratoCratePointer:newCrate];
}

- (id)playlistEntrySourceFor:(NxATrack *)track
{
    NxAAssertNotNil(track);

    NxAOSXAudioFile *localFile = (NxAOSXAudioFile *)track.fileForCurrentPlatform;

    // TODO: This should deal with files on external drives.
    auto newTrackEntry = Serato::TrackEntry::entryWithTrackFileAtOnVolume(String::stringWith(localFile.url.path.UTF8String), String::stringWith("/"));
    return [NxASeratoTrackEntrySource sourceWithSeratoTrackEntryPointer:newTrackEntry];
}

- (void)addTrackSource:(id)source
             asChildOf:(id)collectionSource
{
    NxAAssertNotNil(source);
    NxAAssertNotNil(collectionSource);
    NxAAssertTrue([collectionSource isKindOfClass:[NxASeratoDatabaseSource class]]);
    NxAAssertTrue([source isKindOfClass:[NxASeratoTrackSource class]]);

    auto& database = [(NxASeratoDatabaseSource *)collectionSource getSeratoDatabase];
    auto& track = [(NxASeratoTrackSource *)source getSeratoTrack];
    database.addTrack(track);

    NSLog(@"Added track '%s' to collection.", track.trackFilePath()->toUTF8());
}

- (void)addPlaylistEntrySource:(id)source
                     asChildOf:(id)playlistSource
{
    NxAAssertNotNil(source);
    NxAAssertNotNil(playlistSource);
    NxAAssertTrue([playlistSource isKindOfClass:[NxASeratoCrateSource class]]);
    NxAAssertTrue([source isKindOfClass:[NxASeratoTrackEntrySource class]]);

    auto& seratoCrate = [(NxASeratoCrateSource *)playlistSource getSeratoCrate];
    auto& trackEntry = [(NxASeratoTrackEntrySource *)source getSeratoTrackEntry];
    seratoCrate.addTrackEntry(trackEntry);

    NSLog(@"Adding track entry for '%s' to crate '%s'.", trackEntry.trackFilePath()->toUTF8(), seratoCrate.crateFullName().toUTF8());
}

- (void)addFolderEntrySource:(id)source
                   asChildOf:(id)folderSource
{
    NxAAssertNotNil(source);
    NxAAssertNotNil(folderSource);
    NxAAssertTrue([folderSource isKindOfClass:[NxASeratoCrateSource class]]);
    NxAAssertTrue([source isKindOfClass:[NxASeratoCrateSource class]]);

    auto& seratoCrate = [(NxASeratoCrateSource *)folderSource getSeratoCrate];
    auto& childCrate = [(NxASeratoCrateSource *)source getSeratoCrate];
    seratoCrate.addCrate(childCrate);

    NSLog(@"Added folder '%s' to crate '%s'.", childCrate.crateFullName().toUTF8(), seratoCrate.crateFullName().toUTF8());
}

- (void)setEntriesCountForCollectionSource:(id)source
                                        to:(NSNumber *)count
{
    NxAAssertNotNil(source);
    NxAAssertNotNil(count);

    // -- In Serato databases, we don't need to set any counts in a collection source.
}

- (void)setEntriesCountForPlaylistSource:(id)source
                                      to:(NSNumber *)count
{
    NxAAssertNotNil(source);
    NxAAssertNotNil(count);

    // -- In Serato databases, we don't need to set any counts in a playlist source.
}

- (void)setEntriesCountForFolderSource:(id)source
                                    to:(NSNumber *)count
{
    NxAAssertNotNil(source);
    NxAAssertNotNil(count);

    // -- In Serato databases, we don't need to set any counts in a folder source.
}

- (void)deleteCollectionEntrySources:(NSSet *)objects
{
    NxAAssertNotNil(objects);

    for(NxASeratoTrackSource *value in objects) {
        auto& track = [value getSeratoTrack];
        NSLog(@"Deleting Serato track '%s'.", track.trackFilePath()->toUTF8());

        (*self->p_database)->removeTrack(track);
    }
}

- (void)deletePlaylistEntrySources:(NSSet *)objects
{
    NxAAssertNotNil(objects);

    for(NxASeratoTrackEntrySource *value in objects) {
        auto& trackEntry = [value getSeratoTrackEntry];
        if (trackEntry.hasParentCrate()) {
            NSLog(@"Deleting Serato track entry for '%s' from crate '%s'.", trackEntry.trackFilePath()->toUTF8(), trackEntry.parentCrate().crateFullName().toUTF8());
        }
        else {
            NSLog(@"Deleting orphan Serato track entry for '%s'.", trackEntry.trackFilePath()->toUTF8());
        }

        (*self->p_database)->removeTrackEntry(trackEntry);
    }
}

- (void)deleteFolderEntrySources:(NSSet *)objects
{
    NxAAssertNotNil(objects);

    for(NxASeratoCrateSource *value in objects) {
        auto& crate = [value getSeratoCrate];
        NSLog(@"Deleting Serato crate '%s'.", crate.crateFullName().toUTF8());

        (*self->p_database)->removeCrate(crate);
    }
}

- (void)saveCollection
{
    (*self->p_database)->saveIfModified();
}

- (void)dealloc
{
    delete self->p_database;
    self->p_database = nullptr;
}

@end
