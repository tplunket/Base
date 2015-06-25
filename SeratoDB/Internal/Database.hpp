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

#include <Base/Base.hpp>
#include <Base/Internal/Object.hpp>

#define PRINT_DEBUG_INFO        1

namespace NxA { namespace Serato { namespace Internal {
    struct Database : public NxA::Internal::Object {
        NXA_GENERATED_INTERNAL_DECLARATIONS_WITHOUT_CONSTRUCTOR_FOR(NxA::Serato, Database);

        #pragma mark Constructors & Destructors
        Database(String::ConstPointer const& path,
                 Serato::CrateOrderFile::Pointer usingCrateOrderFile);

        #pragma mark Class Methods
#if PRINT_DEBUG_INFO
        static void debugListCrate(Serato::Crate::ConstPointer const& crate,
                                   String::ConstPointer const& spacing);
#endif

        #pragma mark Instance Variables
        String::ConstPointer databaseFilePath;
        Serato::Track::Array::Pointer tracks;
        Serato::Tag::ConstArray::Pointer otherTags;

        String::ConstArray::Pointer crateFilesToDelete;

        Serato::CrateOrderFile::Pointer crateOrderFile;

        bool databaseIsValid;

        #pragma mark Instance Methods
        void storeTrackTag(Serato::Tag::Pointer const& tag);
        void storeOtherTag(Serato::Tag::Pointer const& tag);
    };
} } }
