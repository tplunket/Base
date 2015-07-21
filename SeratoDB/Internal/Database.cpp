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

#include "Internal/Database.hpp"

#include "TrackFiles/TrackFile.hpp"

// -- Generated internal implementation ommitted because this class does not use the default contructor.

using namespace NxA::Serato::Internal;

#pragma mark Constructors & Destructors

Database::Database(const String& path,
                   Serato::CrateOrderFile& usingCrateOrderFile) :
                   databaseFilePath(path.pointer()),
                   tracks(Serato::Track::Array::array()),
                   otherTags(Serato::Tag::ArrayOfConst::array()),
                   crateFilesToDelete(String::Array::array()),
                   crateOrderFile(usingCrateOrderFile.pointer()),
                   databaseIsValid(false) { }

#pragma mark Class Methods

#if PRINT_DEBUG_INFO
void Database::debugListCrate(Serato::Crate& crate,
                              const String& spacing)
{
    auto& crates = crate.crates();
    for (auto& subCrate : crates) {
        auto& crateName = subCrate->crateName();
        printf("%sCrate '%s'\n", spacing.toUTF8(), crateName.toUTF8());

        auto& crateTracks = subCrate->trackEntries();
        for (auto& trackEntry : crateTracks) {
            printf("%s   Track '%s'\n", spacing.toUTF8(), trackEntry->trackFilePath()->toUTF8());
        }

        String::Pointer newSpacing = String::stringWith(spacing);
        newSpacing->append("   ");

        Database::debugListCrate(subCrate, newSpacing);
    }
}
#endif

#pragma mark Instance Methods

void Database::storeTrackTag(Serato::ObjectTag& tag)
{
    try {
        this->tracks->append(Serato::Track::trackWithTagOnVolume(tag, String::stringWith("")));
    }
    catch (TrackFileError exception) {
        // TODO: This should be a logging call instead.
        printf("%s\n", exception.what());
    }
}

void Database::storeOtherTag(const Serato::Tag& tag)
{
    this->otherTags->append(tag);
}
