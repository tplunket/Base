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

#pragma once

#include <SeratoDB/Track.hpp>
#include <Tags/ObjectTag.hpp>
#include <TrackFiles/TrackFile.hpp>
#include <Markers/CueMarker.hpp>
#include <Markers/LoopMarker.hpp>
#include <Markers/GridMarker.hpp>

#include <Base/Base.hpp>
#include <Base/Internal/Object.hpp>

namespace NxA { namespace Serato { namespace Internal {
    struct Track : public NxA::Internal::Object {
        NXA_GENERATED_INTERNAL_DECLARATIONS_WITHOUT_CONSTRUCTORS_FOR(NxA::Serato, Track);

        #pragma mark Constructors & Destructors
        Track(Serato::ObjectTag& tag, const String& rootFolderPath);

        #pragma mark Class Methods
#if NXA_PRINT_DEBUG_INFO
        static void debugPrintString(const String& text, const String& name);
        static void debugPrintUint(uinteger32 value, const String& name);
        static void debugPrintTimeFromMilliseconds(uinteger32 value, const String& name);
        static void debugPrintDate(timestamp value, const String& name);
        static void debugPrint(const Serato::Track& track, const Serato::TrackFile& trackFile);
#endif

        #pragma mark Instance Variables
        Serato::ObjectTag::Pointer trackTag;
        String::PointerToConst rootFolder;

        boolean needsToUpdateTrackFile;
        boolean needsToUpdateDatabaseFile;
        boolean trackFileWasRead;

        Serato::CueMarker::ArrayOfConst::Pointer cueMarkers;
        Serato::LoopMarker::ArrayOfConst::Pointer loopMarkers;
        Serato::GridMarker::ArrayOfConst::Pointer gridMarkers;

        #pragma mark Instance Methods
        const String& stringForSubTagForIdentifier(uinteger32 identifier) const;
        const String& pathForSubTagForIdentifier(uinteger32 identifier) const;
        uinteger32 uint32ForSubTagForIdentifier(uinteger32 identifier) const;

        void setStringForSubTagForIdentifier(const String& value, uinteger32 identifier);
        void setPathForSubTagForIdentifier(const String& value, uinteger32 identifier);
        void setUInt32ForSubTagForIdentifier(uinteger32 value, uinteger32 identifier);

        String::Pointer trackFilePath(void) const;
        void readTrackFile(void);
        void readMarkersFrom(const Serato::TrackFile& trackFile);
    };
} } }
