//
//  Copyright (c) 2015 Next Audio Labs, LLC. All rights reserved.
//
//  This file contains confidential and proprietary information of Serato
//  Inc. LLP ("Serato"). No use is permitted without express written
//  permission of Serato. If you are not a party to a Confidentiality/
//  Non-Disclosure Agreement with Serato, please immediately delete this
//  file as well as all copies in your possession. For further information,
//  please refer to the modified MIT license provided with this library,
//  or email licensing@serato.com.
//

#include "CrateOrderFile.hpp"
#include "Utility.hpp"

#include <Base/File.hpp>
#include <Base/String.hpp>
#include <Base/Types.hpp>

#include <string>
#include <errno.h>
#include <dirent.h>

using namespace NxA;
using namespace NxA::Serato;
using namespace std;

#pragma mark Utility Functions

static String::Pointer p_crateNameIfValidCrateOrEmptyIfNot(String::ConstPointer const& name)
{
    String::Pointer result = String::string();

    if (name->hasPrefix("[crate]")) {
        result = name->subString(7, name->length() - 7);
    }

    return result;
}

static bool p_filenameIsAValidCrateName(String::ConstPointer const& fileName)
{
    return !fileName->hasPrefix(".") && fileName->hasPostfix(".crate");
}

static String::Pointer p_crateNameFromFilename(String::ConstPointer const& fileName)
{
    return fileName->subString(0, fileName->length() - 6);
}

static StringVectorPtr p_cratesInSubCratesDirectory(const string& directory)
{
    StringVectorPtr crateNamesFound(make_unique<StringVector>());

    DIR *pdir;
    struct dirent *pent;

    pdir = opendir(directory.c_str()); //"." refers to the current dir
    if (pdir){
        errno = 0;
        while ((pent = readdir(pdir))){
            auto fileName = String::stringWithUTF8(pent->d_name);
            if (::p_filenameIsAValidCrateName(fileName)) {
                crateNamesFound->push_back(make_unique<string>(p_crateNameFromFilename(fileName)->toUTF8()));
            }
        }
    }

    if (!errno){
        closedir(pdir);
    }

    return move(crateNamesFound);
}

static StringVectorPtr p_readCratesNamesInCrateOrderFile(String::ConstPointer const& crateOrderFilePath)
{
    StringVectorPtr cratesInOrder(make_unique<StringVector>());

    const Blob::Pointer crateOrderFile = File::readFileAt(crateOrderFilePath);
    if (crateOrderFile->size()) {
        auto textAsString = String::stringWithUTF16(crateOrderFile);
        auto lines = textAsString->splitBySeperator('\n');
        for (auto& crateLine : *lines) {
            auto fullCrateName = p_crateNameIfValidCrateOrEmptyIfNot(crateLine);
            if (fullCrateName->isEmpty()) {
                continue;
            }

            cratesInOrder->push_back(make_unique<string>(fullCrateName->toUTF8()));
        }
    }

    return move(cratesInOrder);
}

static void p_addCratesNamesAtTheStartOfUnlessAlreadyThere(StringVector& cratesToAddTo, const StringVector& cratesToAdd)
{
    auto insertionPosition = cratesToAddTo.begin();
    for (auto& crateName : cratesToAdd) {
        bool alreadyHaveThisCrate = false;

        for (auto& otherCrateName : cratesToAddTo) {
            if (*crateName == *otherCrateName) {
                alreadyHaveThisCrate = true;
                break;
            }
        }

        if (!alreadyHaveThisCrate) {
            printf("added: %s\n", crateName->c_str());
            insertionPosition = cratesToAddTo.insert(insertionPosition, make_unique<string>(*crateName)) + 1;
        }
    }
}

#pragma mark Constructors

CrateOrderFile::CrateOrderFile(const char* seratoFolderPath, const char* rootFolderPath, Database& database) :
                               p_crateOrderFilePath(crateOrderFilePathForSeratoFolder(seratoFolderPath)),
                               p_unknownCrates(make_unique<StringVector>()),
                               p_rootCrate(make_unique<Crate>("", "", "", database))
{
    auto subCratesDirectory = subCratesDirectoryPathInSeratoFolder(seratoFolderPath);
    StringVectorPtr subCratesFound(::p_cratesInSubCratesDirectory(subCratesDirectory->toUTF8()));
    StringVectorPtr cratesInOrder(::p_readCratesNamesInCrateOrderFile(this->p_crateOrderFilePath));

    ::p_addCratesNamesAtTheStartOfUnlessAlreadyThere(*cratesInOrder, *subCratesFound);

    StringVector::iterator it = cratesInOrder->begin();
    CrateVectorPtr crates = this->p_childrenCratesOfCrateNamedUsingNameList("", it, cratesInOrder->end(), seratoFolderPath, rootFolderPath, database);
    for (auto& crateName : *crates) {
        this->p_rootCrate->addChildCrate(move(crateName));
    }

    this->p_rootCrate->resetModificationFlags();
}

#pragma mark Instance Methods

CrateVectorPtr CrateOrderFile::p_childrenCratesOfCrateNamedUsingNameList(const string& name,
                                                                         StringVector::iterator& it,
                                                                         const StringVector::iterator& end,
                                                                         const char* seratoFolderPath,
                                                                         const char* rootFolderPath,
                                                                         Database& database)
{
    CrateVectorPtr cratesFound = make_unique<CrateVector>();

    while (it != end) {
        auto fullCrateName = String::stringWithUTF8(it->get()->c_str());
        if (name.length() && !fullCrateName->hasPrefix(name.c_str())) {
            break;
        }

        if (Crate::isASmartCrateName(fullCrateName->toUTF8(), seratoFolderPath)) {
            this->p_unknownCrates->push_back(make_unique<string>(fullCrateName->toUTF8()));
            ++it;
            continue;
        }

        CratePtr newCrate = make_unique<Crate>(fullCrateName->toUTF8(), seratoFolderPath, rootFolderPath, database);

        if (Crate::isAValidCrateName(fullCrateName->toUTF8(), seratoFolderPath)) {
            newCrate->loadFromFile();
        }

        ++it;

        auto crateNameWithSeperator = String::stringWithString(fullCrateName);
        crateNameWithSeperator->append("%%");

        CrateVectorPtr childCrates = p_childrenCratesOfCrateNamedUsingNameList(std::string(crateNameWithSeperator->toUTF8()),
                                                                               it,
                                                                               end,
                                                                               seratoFolderPath,
                                                                               rootFolderPath,
                                                                               database);
        for (auto& crate : *childCrates) {
            newCrate->addChildCrate(move(crate));
        }

        newCrate->resetModificationFlags();

        cratesFound->push_back(move(newCrate));
    }
    
    return move(cratesFound);
}

const Crate* CrateOrderFile::rootCrate(void) const
{
    return this->p_rootCrate.get();
}

timestamp CrateOrderFile::modificationDateInSecondsSince1970(void) const
{
    return File::modificationDateInSecondsSince1970ForFile(this->p_crateOrderFilePath);
}

void CrateOrderFile::saveIfModified(void) const
{
    this->p_rootCrate->saveIfModifiedAndRecurseToChildren();

    if (!this->p_rootCrate->childrenCratesWereModified()) {
        return;
    }

    auto result = String::string();
    result->append("[begin record]\n");
    this->p_rootCrate->addFullCrateNameWithPrefixAndRecurseToChildren(result, "[crate]");
    for (auto& crateName : *this->p_unknownCrates) {
        result->append("[crate]");
        result->append(crateName.get()->c_str());
        result->append("\n");
    }
    result->append("[end record]\n");

    File::writeToFile(this->p_crateOrderFilePath, result->toUTF16());
}
