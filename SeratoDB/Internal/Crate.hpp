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
#include "Tags/ObjectTag.hpp"

#include <Base/Base.hpp>
#include <Base/Internal/Object.hpp>

namespace NxA { namespace Serato { namespace Internal {
    struct Crate : public NxA::Internal::Object {
        NXA_GENERATED_INTERNAL_DECLARATIONS_WITHOUT_CONSTRUCTORS_FOR(NxA::Serato, Crate);

        #pragma mark Constructors & Destructors
        Crate(const String& fullName, const String& volumePath, const String& fromSeratoFolderPath);

        #pragma mark Class Methods
        static String::Pointer smartCratesDirectoryPathInSeratoFolder(const String& seratoFolderPath);
        static String::Pointer crateFilePathForCrateNameInSeratoFolder(const String& crateName,
                                                                       const String& seratoFolderPath);
        static String::Pointer crateFilePathForSmartCrateNameInSeratoFolder(const String& crateName,
                                                                            const String& seratoFolderPath);

        #pragma mark Instance Variables
        String::PointerToConst crateName;
        String::PointerToConst crateFullName;
        String::PointerToConst rootVolumePath;
        String::PointerToConst seratoFolderPath;

        boolean tracksWereModified;
        boolean cratesWereModified;

        Serato::Crate::WeakPointer parentCrate;
        Serato::Crate::Array::Pointer childrenCrates;
        Serato::TrackEntry::Array::Pointer trackEntries;

        Serato::Tag::ArrayOfConst::Pointer otherTags;

        #pragma mark Instance Methods
        void markCratesAsModified(void);

        String::Pointer crateFilePath(void);
    };
} } }
