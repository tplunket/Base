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

#include "SeratoDb/CrateOrderFile.hpp"
#include "SeratoDb/Crate.hpp"
#include "SeratoDb/Database.hpp"

#include <Base/Base.hpp>
#include <Base/Internal/Object.hpp>

namespace NxA { namespace Serato { namespace Internal {
    struct CrateOrderFile : public NxA::Internal::Object {
        NXA_GENERATED_INTERNAL_DECLARATIONS_WITHOUT_CONSTRUCTOR_FOR(NxA::Serato, CrateOrderFile);

        #pragma mark Constructors & Desctructors
        CrateOrderFile(String::ConstPointer const& path,
                       Serato::Crate::Pointer const& root);

        #pragma mark Class Methods
        static String::Pointer crateNameIfValidCrateOrEmptyIfNot(String::ConstPointer const& name);
        static bool filenameIsAValidCrateName(String::ConstPointer const& fileName);
        static String::Pointer crateNameFromFilename(String::ConstPointer const& fileName);
        static String::Array::Pointer cratesInSubCratesDirectory(String::ConstPointer const& directory);
        static String::Array::Pointer readCratesNamesInCrateOrderFile(String::ConstPointer const& crateOrderFilePath);
        static void addCratesNamesAtTheStartOfUnlessAlreadyThere(String::Array::Pointer const& cratesToAddTo,
                                                                 String::Array::ConstPointer const& cratesToAdd);

        #pragma mark Instance Variables
        String::ConstPointer crateOrderFilePath;

        Serato::Crate::Pointer rootCrate;

        String::Array::Pointer unknownCratesNames;

        #pragma mark Instance Methods
        Serato::Crate::Array::Pointer childrenCratesOfCrateNamedUsingNameList(String::ConstPointer const& name,
                                                                              String::Array::iterator& it,
                                                                              const String::Array::iterator& end,
                                                                              String::ConstPointer const& seratoFolderPath,
                                                                              String::ConstPointer const& rootFolderPath);
    };
} } }
