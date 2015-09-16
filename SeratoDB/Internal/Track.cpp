//
//  Copyright (c) 2015 Next Audio Labs, LLC. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy of
//  this software and associated documentation files (the "Software"), to deal in the
//  Software without restriction, including without limitation the rights to use, copy,
//  modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
//  and to permit persons to whom the Software is furnished to do so, subject to the
//  following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
//  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
//  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include "Internal/Track.hpp"
#include "TrackFiles/TrackFileFactory.hpp"
#include "Tags/ObjectTag.hpp"
#include "Tags/TextTag.hpp"
#include "Tags/PathTag.hpp"
#include "Tags/UInteger32Tag.hpp"
#include "Tags/DatabaseV2Tags.hpp"

// -- Generated internal implementation ommitted because this class does not use the default contructor.

using namespace NxA;
using namespace NxA::Serato::Internal;

#pragma mark Constants

static String::Pointer emptyString = String::string();

#pragma mark Constructors & Destructors

Track::Track(Serato::ObjectTag& tag, const String& rootFolderPath) :
             trackTag(tag.pointer()),
             rootFolder(rootFolderPath.pointer()),
             needsToUpdateDatabaseFile(false),
             trackFilemarkersWereRead(false),
             cueMarkers(Serato::CueMarker::ArrayOfConst::array()),
             loopMarkers(Serato::LoopMarker::ArrayOfConst::array()),
             gridMarkers(Serato::GridMarker::ArrayOfConst::array()),
             lastMarkersModificationDate(0)
{
}

#pragma mark Class Methods

#if NXA_PRINT_DEBUG_INFO
void Track::debugPrintString(const String& text, const String& name)
{
    printf("%s '%s'\n", name.toUTF8(), text.toUTF8());
}

void Track::debugPrintUint(uinteger32 value, const String& name)
{
    printf("%s '%d'\n", name.toUTF8(), value);
}

void Track::debugPrintDate(timestamp value, const String& name)
{
    char* stringVersion = ctime(&value);
    printf("%s %s", name.toUTF8(), stringVersion);
}

void Track::debugPrint(const Serato::Track& track, const Serato::TrackFile& trackFile)
{
    Track::debugPrintString(track.size(), String::stringWith("size"));
    Track::debugPrintDate(track.dateModifiedInSecondsSinceJanuary1st1970(), String::stringWith("datemodified"));
    Track::debugPrintDate(track.dateAddedInSecondsSinceJanuary1st1970(), String::stringWith("dateadded"));

    Track::debugPrintString(track.title(), String::stringWith("title"));
    Track::debugPrintString(track.artist(), String::stringWith("artist"));
    Track::debugPrintString(track.album(), String::stringWith("album"));
    Track::debugPrintString(track.comments(), String::stringWith("comments"));
    Track::debugPrintString(track.genre(), String::stringWith("genre"));
    Track::debugPrintString(track.grouping(), String::stringWith("grouping"));
    Track::debugPrintString(track.recordLabel(), String::stringWith("recordlabel"));
    Track::debugPrintString(track.remixer(), String::stringWith("remixer"));
    Track::debugPrintString(track.composer(), String::stringWith("composer"));
    Track::debugPrintString(track.key(), String::stringWith("key"));
    Track::debugPrintString(track.year(), String::stringWith("year"));
    Track::debugPrintString(track.length(), String::stringWith("length"));
    Track::debugPrintString(track.bitRate(), String::stringWith("bitrate"));
    Track::debugPrintString(track.sampleRate(), String::stringWith("samplerate"));
    Track::debugPrintString(track.bpm(), String::stringWith("bpm"));
    Track::debugPrintUint((uinteger32)track.trackNumber(), String::stringWith("tracknumber"));
    Track::debugPrintUint((uinteger32)track.discNumber(), String::stringWith("discnumber"));

    printf("Found %ld cue markers, %ld grid markers and %ld loop markers.\n",
           trackFile.cueMarkers().length(),
           trackFile.gridMarkers().length(),
           trackFile.loopMarkers().length());
}
#endif

#pragma mark Instance Methods

const String& Track::stringForSubTagForIdentifier(uinteger32 identifier) const
{
    auto& trackObjectTag = *this->trackTag;
    if (trackObjectTag.hasSubTagForIdentifier(identifier)) {
        auto& textTag = dynamic_cast<const Serato::TextTag&>(trackObjectTag.subTagForIdentifier(identifier));
        return textTag.value();
    }

    return emptyString;
}

const String& Track::pathForSubTagForIdentifier(uinteger32 identifier) const
{
    auto& trackObjectTag = *this->trackTag;
    if (trackObjectTag.hasSubTagForIdentifier(identifier)) {
        auto& pathTag = dynamic_cast<const Serato::PathTag&>(trackObjectTag.subTagForIdentifier(identifier));
        return pathTag.value();
    }

    return emptyString;
}

uinteger32 Track::uint32ForSubTagForIdentifier(uinteger32 identifier) const
{
    auto& trackObjectTag = *this->trackTag;
    if (trackObjectTag.hasSubTagForIdentifier(identifier)) {
        auto& uintTag = dynamic_cast<const Serato::UInteger32Tag&>(trackObjectTag.subTagForIdentifier(identifier));
        return uintTag.value();
    }

    return 0;
}

void Track::setStringForSubTagForIdentifier(const String& value, uinteger32 identifier)
{
    auto& trackObjectTag = *this->trackTag;
    if (!trackObjectTag.hasSubTagForIdentifier(identifier)) {
        auto textTag = Serato::TextTag::tagWithIdentifierAndValue(identifier, value);
        trackObjectTag.setSubTag(*textTag);
    }
    else {
        auto& textTag = dynamic_cast<Serato::TextTag&>(trackObjectTag.subTagForIdentifier(identifier));
        textTag.setValue(value);
    }

    this->needsToUpdateDatabaseFile = true;
}

void Track::setPathForSubTagForIdentifier(const String& value, uinteger32 identifier)
{
    auto& trackObjectTag = dynamic_cast<Serato::ObjectTag&>(*this->trackTag);
    if (!trackObjectTag.hasSubTagForIdentifier(identifier)) {
        auto pathTag = Serato::PathTag::tagWithIdentifierAndValue(identifier, value);
        trackObjectTag.setSubTag(*pathTag);
    }
    else {
        auto& pathTag = dynamic_cast<Serato::PathTag&>(trackObjectTag.subTagForIdentifier(identifier));
        pathTag.setValue(value);
    }

    this->needsToUpdateDatabaseFile = true;
}

void Track::setUInt32ForSubTagForIdentifier(uinteger32 value, uinteger32 identifier)
{
    auto& trackObjectTag = *this->trackTag;
    if (!trackObjectTag.hasSubTagForIdentifier(identifier)) {
        auto uinteger32Tag = Serato::UInteger32Tag::tagWithIdentifierAndValue(identifier, value);
        trackObjectTag.setSubTag(*uinteger32Tag);
    }
    else {
        auto& uinteger32Tag = dynamic_cast<Serato::UInteger32Tag&>(trackObjectTag.subTagForIdentifier(identifier));
        uinteger32Tag.setValue(value);
    }

    this->needsToUpdateDatabaseFile = true;
}

String::Pointer Track::trackFilePath(void) const
{
    auto& pathFromRootFolder = this->pathForSubTagForIdentifier(trackFilePathTagIdentifier);
    return File::joinPaths(this->rootFolder, pathFromRootFolder);
}

void Track::readMarkers(void)
{
    auto trackFile = TrackFileFactory::trackFileForPath(this->trackFilePath());

    auto& allCueMarkers = trackFile->cueMarkers();
    for (auto& marker : allCueMarkers) {
#if NXA_PRINT_DEBUG_INFO
        printf("Found cue marker at %f and index %d.\n", (float)marker->positionInMilliseconds()/1000, marker->index());
#endif
        this->cueMarkers->append(Serato::CueMarker::markerWith(marker));
    }

    auto& allLoopMarkers = trackFile->loopMarkers();
    for (auto& marker : allLoopMarkers) {
#if NXA_PRINT_DEBUG_INFO
        printf("Found loop marker at %f and index %d.\n", (float)marker->startPositionInMilliseconds()/1000, marker->index());
#endif
        this->loopMarkers->append(Serato::LoopMarker::markerWith(marker));
    }

    auto& allGridMarkers = trackFile->gridMarkers();
    for (auto& marker : allGridMarkers) {
#if NXA_PRINT_DEBUG_INFO
        printf("Found grid marker at %f and bpm %f.\n", marker->positionInSeconds(), marker->beatsPerMinute());
#endif
        this->gridMarkers->append(Serato::GridMarker::markerWith(marker));
    }

    this->lastMarkersModificationDate = File::modificationDateInSecondsSince1970ForFile(this->trackFilePath());

    this->trackFilemarkersWereRead = true;
}

void Track::saveToTrackFile(void) const
{
    auto trackFile = TrackFileFactory::trackFileForPath(this->trackFilePath());

    auto newCueMarkers = Serato::CueMarker::Array::array();
    for (auto& marker : *(this->cueMarkers)) {
        auto markerCopy = Serato::CueMarker::markerWith(marker);
        newCueMarkers->append(markerCopy);
    }
    trackFile->setCueMarkers(newCueMarkers);

    auto newLoopMarkers = Serato::LoopMarker::Array::array();
    for (auto& marker : *(this->loopMarkers)) {
        auto markerCopy = Serato::LoopMarker::markerWith(marker);
        newLoopMarkers->append(markerCopy);
    }
    trackFile->setLoopMarkers(newLoopMarkers);

    auto newGridMarkers = Serato::GridMarker::Array::array();
    for (auto& marker : *(this->gridMarkers)) {
        auto markerCopy = Serato::GridMarker::markerWith(marker);
        newGridMarkers->append(markerCopy);
    }
    trackFile->setGridMarkers(newGridMarkers);

    trackFile->saveChangesIfAny();
}

