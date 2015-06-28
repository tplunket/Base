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

#include "Internal/CrateOrderFile.hpp"

#include <string>
#include <errno.h>
#include <dirent.h>

// -- Generated internal implementation ommitted because this class does not use the default contructor.

using namespace NxA;
using namespace NxA::Serato::Internal;

#pragma mark Constructors & Destructors

CrateOrderFile::CrateOrderFile(const String& path,
                               Serato::Crate& root) :
                               crateOrderFilePath(path.pointerToConst()),
                               rootCrate(root.pointer()),
                               unknownCratesNames(String::ArrayOfConst::array()) { }

#pragma mark Class Methods

String::Pointer CrateOrderFile::crateNameIfValidCrateOrEmptyIfNot(const String& name)
{
    auto result = String::string();

    if (name.hasPrefix("[crate]")) {
        result = name.subString(7, name.length() - 7);
    }

    return result;
}

bool CrateOrderFile::filenameIsAValidCrateName(const String& fileName)
{
    return !fileName.hasPrefix(".") && fileName.hasPostfix(".crate");
}

String::Pointer CrateOrderFile::crateNameFromFilename(const String& fileName)
{
    return fileName.subString(0, fileName.length() - 6);
}

String::ArrayOfConst::Pointer CrateOrderFile::cratesInSubCratesDirectory(const String& directory)
{
    auto crateNamesFound = String::ArrayOfConst::array();

    DIR *pdir;
    struct dirent *pent;

    pdir = opendir(directory.toUTF8()); //"." refers to the current dir
    if (pdir){
        errno = 0;
        while ((pent = readdir(pdir))){
            auto fileName = String::stringWith(pent->d_name);
            if (CrateOrderFile::filenameIsAValidCrateName(fileName)) {
                crateNamesFound->append(CrateOrderFile::crateNameFromFilename(fileName));
            }
        }
    }

    if (!errno){
        closedir(pdir);
    }

    return crateNamesFound;
}

String::ArrayOfConst::Pointer CrateOrderFile::readCratesNamesInCrateOrderFile(const String& crateOrderFilePath)
{
    auto cratesInOrder = String::ArrayOfConst::array();

    auto crateOrderFile = File::readFileAt(crateOrderFilePath);
    if (crateOrderFile->size()) {
        auto textAsString = String::stringWithUTF16(crateOrderFile);
        auto lines = textAsString->splitBySeperator('\n');
        for (auto& crateLine : *lines) {
            auto fullCrateName = CrateOrderFile::crateNameIfValidCrateOrEmptyIfNot(crateLine);
            if (fullCrateName->isEmpty()) {
                continue;
            }

            cratesInOrder->append(fullCrateName);
        }
    }

    return cratesInOrder;
}

void CrateOrderFile::addCratesNamesAtTheStartOfUnlessAlreadyThere(String::ArrayOfConst& cratesToAddTo,
                                                                  const String::ArrayOfConst& cratesToAdd)
{
    auto insertionPosition = cratesToAddTo.begin();
    for (auto& crateName : cratesToAdd) {
        bool alreadyHaveThisCrate = false;

        for (auto& otherCrateName : cratesToAddTo) {
            if (crateName->isEqualTo(otherCrateName)) {
                alreadyHaveThisCrate = true;
                break;
            }
        }

        if (!alreadyHaveThisCrate) {
            printf("added: %s\n", crateName->toUTF8());
            cratesToAddTo.insertAt(*crateName, insertionPosition);
            insertionPosition += 1;
        }
    }
}

#pragma mark Instance Methods

Serato::Crate::Array::Pointer CrateOrderFile::childrenCratesOfCrateNamedUsingNameList(const String& name,
                                                                                      String::ArrayOfConst::iterator& it,
                                                                                      const String::ArrayOfConst::iterator& end,
                                                                                      const String& seratoFolderPath,
                                                                                      const String& rootFolderPath)
{
    auto cratesFound = Serato::Crate::Array::array();

    while (it != end) {
        auto fullCrateName = *it;
        if (name.length() && !fullCrateName->hasPrefix(name.toUTF8())) {
            break;
        }

        if (Serato::Crate::isASmartCrateName(fullCrateName, seratoFolderPath)) {
            this->unknownCratesNames->append(fullCrateName);
            ++it;
            continue;
        }

        auto newCrate = Serato::Crate::crateWithNameInFolderOnVolume(fullCrateName,
                                                                     seratoFolderPath,
                                                                     rootFolderPath);

        if (Serato::Crate::isAValidCrateName(fullCrateName, seratoFolderPath)) {
            newCrate->loadFromFile();
        }

        ++it;

        auto crateNameWithSeperator = String::stringWith(fullCrateName);
        crateNameWithSeperator->append("%%");

        auto childCrates = CrateOrderFile::childrenCratesOfCrateNamedUsingNameList(crateNameWithSeperator,
                                                                                   it,
                                                                                   end,
                                                                                   seratoFolderPath,
                                                                                   rootFolderPath);
        for (auto& crate : *childCrates) {
            Serato::Crate::addCrateAsChildOfCrate(crate, newCrate);
        }

        newCrate->resetModificationFlags();

        cratesFound->append(newCrate);
    }
    
    return cratesFound;
}
