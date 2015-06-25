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

#include "SeratoDB/Crate.hpp"
#include "SeratoDB/TrackEntry.hpp"
#include "Tags/Tag.hpp"

#include <Base/Base.hpp>
#include <Base/Internal/Object.hpp>

namespace NxA { namespace Serato { namespace Internal {
    struct Crate : public NxA::Internal::Object {
        NXA_GENERATED_INTERNAL_DECLARATIONS_WITHOUT_CONSTRUCTOR_FOR(NxA::Serato, Crate);

        #pragma mark Constructors & Destructors
        Crate(String::ConstPointer const& fullName,
              String::ConstPointer const& volumePath,
              String::ConstPointer const& filePath);

        #pragma mark Instance Variables
        String::ConstPointer crateName;
        String::ConstPointer crateFullName;
        String::ConstPointer rootVolumePath;
        String::ConstPointer crateFilePath;

        bool tracksWereModified;
        bool cratesWereModified;

        Serato::Crate::WeakPointer parentCrate;
        Serato::Crate::Array::Pointer childrenCrates;
        Serato::TrackEntry::Array::Pointer trackEntries;

        Serato::Tag::ConstArray::Pointer otherTags;

        #pragma mark Instance Methods
        void addTrackEntry(Serato::TrackEntry::Pointer const& trackEntry);

        void storeTrackTag(Serato::Tag::ConstPointer const& tag);
        void storeOtherTag(Serato::Tag::Pointer const& tag);

        void markCratesAsModified(void);
    };
} } }
