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

// -- Generated internal implementation ommitted because this class does not use the default contructor.

using namespace NxA;
using namespace NxA::Serato::Internal;

#pragma mark Constants

static String::Pointer emptyString = String::string();

#pragma mark Constructors & Destructors

Track::Track(Serato::Tag& tag,
             const String& rootFolderPath) :
             trackTag(tag.pointer()),
             rootFolder(rootFolderPath.pointer()),
             wasModified(false),
             cueMarkers(Serato::CueMarker::ArrayOfConst::array()),
             loopMarkers(Serato::LoopMarker::ArrayOfConst::array()),
             gridMarkers(Serato::GridMarker::ArrayOfConst::array()) { }

#pragma mark Class Methods

#if PRINT_DEBUG_INFO
void Track::debugPrint(const String& text, const String& name)
{
    printf("%s '%s'\n", name.toUTF8(), text.toUTF8());
}

void Track::debugPrintUint(uinteger32 value, const String& name)
{
    printf("%s '%d'\n", name.toUTF8(), value);
}

void Track::debugPrintTimeFromMilliseconds(uinteger32 value, const String& name)
{
    uinteger32 minutes = value / 60000;
    uinteger32 milliseconds = value % 1000;
    uinteger32 seconds = (value - milliseconds - (minutes * 60000)) / 1000;

    milliseconds *= 60;
    milliseconds /= 1000;

    printf("%s '%02d:%02d.%02d'\n", name.toUTF8(), minutes, seconds, milliseconds);
}

void Track::debugPrintDate(timestamp value, const String& name)
{
    char* stringVersion = ctime(&value);
    printf("%s %s", name.toUTF8(), stringVersion);
}

void Track::debugPrintComparaison(const Serato::Track& track, const Serato::TrackFile& trackFile)
{
    printf("----------------------------------------\n");
    Track::debugPrintUint(static_cast<uinteger32>(track.size()), String::stringWith("size"));
    Track::debugPrintUint((uint32_t)trackFile.size(), String::stringWith("size"));
    Track::debugPrintDate(track.dateModifiedInSecondsSinceJanuary1st1970(), String::stringWith("datemodified"));
    Track::debugPrintDate(track.dateAddedInSecondsSinceJanuary1st1970(), String::stringWith("dateadded"));

    Track::debugPrint(track.title(), String::stringWith("title"));
    Track::debugPrint(trackFile.title(), String::stringWith("title"));
    Track::debugPrint(track.artist(), String::stringWith("artist"));
    Track::debugPrint(trackFile.artist(), String::stringWith("artist"));
    Track::debugPrint(track.album(), String::stringWith("album"));
    Track::debugPrint(trackFile.album(), String::stringWith("album"));
    Track::debugPrint(track.comments(), String::stringWith("comments"));
    Track::debugPrint(trackFile.comments(), String::stringWith("comments"));
    Track::debugPrint(track.genre(), String::stringWith("genre"));
    Track::debugPrint(trackFile.genre(), String::stringWith("genre"));
    Track::debugPrint(track.grouping(), String::stringWith("grouping"));
    Track::debugPrint(trackFile.grouping(), String::stringWith("grouping"));
    Track::debugPrint(track.recordLabel(), String::stringWith("recordlabel"));
    if (trackFile.hasRecordLabel()) {
        Track::debugPrint(trackFile.recordLabel(), String::stringWith("recordLabel"));
    }
    else {
        printf("No record label.\n");
    }
    Track::debugPrint(track.remixer(), String::stringWith("remixer"));
    if (trackFile.hasRecordLabel()) {
        Track::debugPrint(trackFile.remixer(), String::stringWith("remixer"));
    }
    else {
        printf("No remixer.\n");
    }
    Track::debugPrint(track.composer(), String::stringWith("composer"));
    Track::debugPrint(trackFile.composer(), String::stringWith("composer"));
    Track::debugPrint(track.key(), String::stringWith("key"));
    if (trackFile.hasKey()) {
        Track::debugPrint(trackFile.key(), String::stringWith("key"));
    }
    else {
        printf("No key.\n");
    }
    Track::debugPrint(track.year(), String::stringWith("year"));
    Track::debugPrint(trackFile.yearReleased(), String::stringWith("year"));
    Track::debugPrint(track.length(), String::stringWith("length"));
    Track::debugPrintTimeFromMilliseconds(trackFile.lengthInMilliseconds(), String::stringWith("length"));
    Track::debugPrint(track.bitRate(), String::stringWith("bitrate"));
    Track::debugPrintUint(trackFile.bitRateInKiloBitsPerSecond(), String::stringWith("bitrate"));
    Track::debugPrint(track.sampleRate(), String::stringWith("samplerate"));
    Track::debugPrintUint(trackFile.sampleRateInSamplesPerSecond(), String::stringWith("samplerate"));
    Track::debugPrint(track.bpm(), String::stringWith("bpm"));
    Track::debugPrint(trackFile.bpm(), String::stringWith("bpm"));
    Track::debugPrintUint((uinteger32)track.trackNumber(), String::stringWith("tracknumber"));
    Track::debugPrintUint((uinteger32)trackFile.trackNumber(), String::stringWith("tracknumber"));

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
    auto& trackObjectTag = dynamic_cast<const Serato::ObjectTag&>(*this->trackTag);
    if (trackObjectTag.hasSubTagForIdentifier(identifier)) {
        auto& textTag = dynamic_cast<const Serato::TextTag&>(trackObjectTag.subTagForIdentifier(identifier));
        return textTag.value();
    }

    return emptyString;
}

const String& Track::pathForSubTagForIdentifier(uinteger32 identifier) const
{
    auto& trackObjectTag = dynamic_cast<const Serato::ObjectTag&>(*this->trackTag);
    if (trackObjectTag.hasSubTagForIdentifier(identifier)) {
        auto& pathTag = dynamic_cast<const Serato::PathTag&>(trackObjectTag.subTagForIdentifier(identifier));
        return pathTag.value();
    }

    return emptyString;
}

uinteger32 Track::uint32ForSubTagForIdentifier(uinteger32 identifier) const
{
    auto& trackObjectTag = dynamic_cast<const Serato::ObjectTag&>(*this->trackTag);
    if (trackObjectTag.hasSubTagForIdentifier(identifier)) {
        auto& uintTag = dynamic_cast<const Serato::UInteger32Tag&>(trackObjectTag.subTagForIdentifier(identifier));
        return uintTag.value();
    }

    return 0;
}

void Track::setStringForSubTagForIdentifier(const String& value, uinteger32 identifier)
{
    auto& trackObjectTag = dynamic_cast<Serato::ObjectTag&>(*this->trackTag);
    if (!trackObjectTag.hasSubTagForIdentifier(identifier)) {
        auto textTag = Serato::TextTag::tagWithIdentifierAndValue(identifier, value);
        trackObjectTag.addSubTag(*textTag);
    }
    else {
        auto& textTag = dynamic_cast<Serato::TextTag&>(trackObjectTag.subTagForIdentifier(identifier));
        textTag.setValue(value);
    }

    this->wasModified = true;
}

void Track::setPathForSubTagForIdentifier(const String& value, uinteger32 identifier)
{
    auto& trackObjectTag = dynamic_cast<Serato::ObjectTag&>(*this->trackTag);
    if (!trackObjectTag.hasSubTagForIdentifier(identifier)) {
        auto pathTag = Serato::PathTag::tagWithIdentifierAndValue(identifier, value);
        trackObjectTag.addSubTag(*pathTag);
    }
    else {
        auto& pathTag = dynamic_cast<Serato::PathTag&>(trackObjectTag.subTagForIdentifier(identifier));
        pathTag.setValue(value);
    }

    this->wasModified = true;
}

void Track::setUInt32ForSubTagForIdentifier(uinteger32 value, uinteger32 identifier)
{
    auto& trackObjectTag = dynamic_cast<Serato::ObjectTag&>(*this->trackTag);
    if (!trackObjectTag.hasSubTagForIdentifier(identifier)) {
        auto uinteger32Tag = Serato::UInteger32Tag::tagWithIdentifierAndValue(identifier, value);
        trackObjectTag.addSubTag(*uinteger32Tag);
    }
    else {
        auto& uinteger32Tag = dynamic_cast<Serato::UInteger32Tag&>(trackObjectTag.subTagForIdentifier(identifier));
        uinteger32Tag.setValue(value);
    }

    this->wasModified = true;
}

void Track::readMarkersFrom(const Serato::TrackFile& trackFile)
{
    auto& allCueMarkers = trackFile.cueMarkers();
    for (auto& marker : allCueMarkers) {
        this->cueMarkers->append(Serato::CueMarker::markerWith(marker));
    }

    auto& allLoopMarkers = trackFile.loopMarkers();
    for (auto& marker : allLoopMarkers) {
        this->loopMarkers->append(Serato::LoopMarker::markerWith(marker));
    }

    auto& allGridMarkers = trackFile.gridMarkers();
    for (auto& marker : allGridMarkers) {
        this->gridMarkers->append(Serato::GridMarker::markerWith(marker));
    }
}
