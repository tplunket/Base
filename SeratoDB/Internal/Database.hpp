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

#include "SeratoDB/Database.hpp"
#include "SeratoDB/Track.hpp"
#include "SeratoDB/Crate.hpp"
#include "SeratoDB/CrateOrderFile.hpp"
#include "Tags/Tag.hpp"
#include "Tags/ObjectTag.hpp"

#include <Base/Base.hpp>
#include <Base/Internal/Object.hpp>

namespace NxA { namespace Serato { namespace Internal {
    struct Database : public NxA::Internal::Object {
        NXA_GENERATED_INTERNAL_DECLARATIONS_WITHOUT_CONSTRUCTORS_FOR(NxA::Serato, Database);

        #pragma mark Constructors & Destructors
        Database(const String& path,
                 const String& volume,
                 Serato::CrateOrderFile& usingCrateOrderFile);

        #pragma mark Class Methods
#if NXA_PRINT_DEBUG_INFO
        static void debugListCrate(Serato::Crate& crate,
                                   const String& spacing);
#endif

        #pragma mark Instance Variables
        String::PointerToConst databaseFilePath;
        String::PointerToConst databaseVolume;
        Serato::Track::Array::Pointer tracks;
        Serato::Tag::ArrayOfConst::Pointer otherTags;

        String::Array::Pointer crateFilesToDelete;

        Serato::CrateOrderFile::Pointer crateOrderFile;

        boolean databaseIsValid;

        #pragma mark Instance Methods
        void storeTrackTag(Serato::ObjectTag& tag);
        void storeOtherTag(const Serato::Tag& tag);
    };
} } }
