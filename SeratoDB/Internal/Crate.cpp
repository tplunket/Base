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

#include "Internal/Crate.hpp"
#include "SeratoDB/Database.hpp"

// -- Generated internal implementation ommitted because this class does not use the default contructor.

using namespace NxA::Serato::Internal;

#pragma mark Constructors & Destructors

Crate::Crate(const String& fullName,
             const String& volumePath,
             const String& fromSeratoFolderPath) :
             crateName(String::string()),
             crateFullName(fullName.pointer()),
             rootVolumePath(volumePath.pointer()),
             seratoFolderPath(fromSeratoFolderPath.pointer()),
             tracksWereModified(true),
             cratesWereModified(false),
             childrenCrates(Serato::Crate::Array::array()),
             trackEntries(Serato::TrackEntry::Array::array()),
             otherTags(Serato::Tag::ArrayOfConst::array()) { }

#pragma mark Class Methods

NxA::String::Pointer Crate::smartCratesDirectoryPathInSeratoFolder(const String& seratoFolderPath)
{
    auto joinedPath = File::joinPaths(seratoFolderPath, String::stringWith("SmartCrates"));
    return joinedPath;
}

NxA::String::Pointer Crate::crateFilePathForCrateNameInSeratoFolder(const String& crateName,
                                                                    const String& seratoFolderPath)
{
    auto cratesFolderPath = NxA::Serato::Crate::subCratesDirectoryPathInSeratoFolder(seratoFolderPath);
    auto crateFilePartialPath = File::joinPaths(cratesFolderPath, crateName);
    crateFilePartialPath->append(".crate");

    return crateFilePartialPath;
}

NxA::String::Pointer Crate::crateFilePathForSmartCrateNameInSeratoFolder(const String& crateName,
                                                                         const String& seratoFolderPath)
{
    auto cratesFolderPath = NxA::Serato::Crate::subCratesDirectoryPathInSeratoFolder(seratoFolderPath);
    auto crateFilePartialPath = File::joinPaths(cratesFolderPath, crateName);
    crateFilePartialPath->append(".scrate");

    return crateFilePartialPath;
}

#pragma mark Instance Methods

void Crate::markCratesAsModified(void)
{
    this->cratesWereModified = true;

    if (this->parentCrate.isValid()) {
        auto parent = this->parentCrate.pointer();
        parent->internal->markCratesAsModified();
    }
}

NxA::String::Pointer Crate::crateFilePath(void)
{
    return Crate::crateFilePathForCrateNameInSeratoFolder(crateFullName,
                                                          seratoFolderPath);
}
