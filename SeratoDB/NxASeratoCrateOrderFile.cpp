//
//  NxASeratoCrateOrderFile.cpp
//  SeratoDB
//
//  Created by Didier Malenfant on 2/4/15.
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

#include "SeratoDB/NxASeratoCrateOrderFile.h"
#include "SeratoDB/NxASeratoDbUtility.h"

#include <string>
#include <errno.h>
#include <dirent.h>

using namespace NxA;
using namespace std;

#pragma mark Utility Functions

static ConstStringPtr p_crateNameIfValidCrateOrNullIfNot(const string& name)
{
    ConstStringPtr result;

    if (stringHasPrefix(name, "[crate]")) {
        result = make_unique<string>(name.substr(7, name.length() - 7));
    }

    return move(result);
}

static bool p_filenameIsAValidCrateName(const string& fileName)
{
    return !stringHasPrefix(fileName, ".") && stringHasPostfix(fileName, ".crate");
}

static string p_crateNameFromFilename(const string& filename)
{
    string crateName(filename);
    crateName.resize(filename.length() - 6);
    return crateName;
}

static StringVectorPtr p_cratesInSubCratesDirectory(const string& directory)
{
    StringVectorPtr crateNamesFound(make_unique<StringVector>());

    DIR *pdir;
    struct dirent *pent;

    pdir=opendir(directory.c_str()); //"." refers to the current dir
    if (pdir){
        errno=0;
        while ((pent=readdir(pdir))){
            string fileName(pent->d_name);
            if (::p_filenameIsAValidCrateName(fileName)) {
                crateNamesFound->push_back(make_unique<string>(p_crateNameFromFilename(fileName)));
            }
        }
    }

    if (!errno){
        closedir(pdir);
    }

    return move(crateNamesFound);
}

static StringVectorPtr p_readCratesNamesInCrateOrderFile(const string& crateOrderFilePath)
{
    StringVectorPtr cratesInOrder(make_unique<StringVector>());

    CharVectorPtr crateOrderFile = readFileAt(crateOrderFilePath.c_str());
    if (crateOrderFile->size()) {
        const char16_t* textToRead = (const char16_t*)crateOrderFile->data();
        int numberOfCharacters = (int)crateOrderFile->size() / 2;
        ConstStringPtr textAString = convertUTF16ToStdString(textToRead, numberOfCharacters);

        StringVectorPtr lines(splitStringIntoOneStringForEachLine(*textAString));
        for (auto& crateLine : *lines) {
            const ConstStringPtr fullCrateName = p_crateNameIfValidCrateOrNullIfNot(*crateLine);
            if (!fullCrateName.get()) {
                continue;
            }

            cratesInOrder->push_back(make_unique<string>(*fullCrateName));
        }
    }

    return move(cratesInOrder);
}

static void p_addCratesNamesAtTheStartOfUnlessAlreadyThere(StringVector& cratesToAddTo, const StringVector& cratesToAdd)
{
    StringVector::iterator insertionPosition = cratesToAddTo.begin();
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

SeratoCrateOrderFile::SeratoCrateOrderFile(const char* seratoFolderPath, const char* rootFolderPath) :
                                           p_crateOrderFilePath(crateOrderFilePathForSeratoFolder(seratoFolderPath)),
                                           p_unknownCrates(make_unique<StringVector>()),
                                           p_rootCrate(make_unique<SeratoCrate>("", "", ""))
{
    ConstStringPtr subcratesDirectory = subcratesDirectoryPathInSeratoFolder(seratoFolderPath);
    StringVectorPtr subCratesFound(::p_cratesInSubCratesDirectory(subcratesDirectory->c_str()));
    StringVectorPtr cratesInOrder(::p_readCratesNamesInCrateOrderFile(*(this->p_crateOrderFilePath)));

    ::p_addCratesNamesAtTheStartOfUnlessAlreadyThere(*cratesInOrder, *subCratesFound);

    StringVector::iterator it = cratesInOrder->begin();
    SeratoCrateVectorPtr crates = this->p_childrenCratesOfCrateNamedUsingNameList("", it, cratesInOrder->end(), seratoFolderPath, rootFolderPath);
    for (auto& crateName : *crates) {
        this->p_rootCrate->addChildCrate(move(crateName));
    }

    this->p_rootCrate->resetModificationFlags();
}

#pragma mark Instance Methods

SeratoCrateVectorPtr SeratoCrateOrderFile::p_childrenCratesOfCrateNamedUsingNameList(const string& name,
                                                                                     StringVector::iterator& it,
                                                                                     const StringVector::iterator& end,
                                                                                     const char* seratoFolderPath,
                                                                                     const char* rootFolderPath)
{
    SeratoCrateVectorPtr cratesFound = make_unique<SeratoCrateVector>();

    while (it != end) {
        string fullCrateName = *(it->get());
        if (name.length() && !stringHasPrefix(fullCrateName, name)) {
            break;
        }

        if (!SeratoCrate::isAValidCrateName(fullCrateName.c_str(), seratoFolderPath)) {
            this->p_unknownCrates->push_back(make_unique<string>(fullCrateName));
            ++it;
            continue;
        }

        SeratoCratePtr newCrate = make_unique<SeratoCrate>(fullCrateName.c_str(), seratoFolderPath, rootFolderPath);
        newCrate->loadFromFile();

        ++it;

        if (!newCrate->isAValidCrate()) {
            continue;
        }

        string crateNameWithSeperator = fullCrateName;
        crateNameWithSeperator += "%%";

        SeratoCrateVectorPtr childCrates = p_childrenCratesOfCrateNamedUsingNameList(crateNameWithSeperator,
                                                                                     it,
                                                                                     end,
                                                                                     seratoFolderPath,
                                                                                     rootFolderPath);
        for (auto& crate : *childCrates) {
            newCrate->addChildCrate(move(crate));
        }

        newCrate->resetModificationFlags();

        cratesFound->push_back(move(newCrate));
    }
    
    return move(cratesFound);
}

const SeratoCrate* SeratoCrateOrderFile::rootCrate(void) const
{
    return this->p_rootCrate.get();
}

void SeratoCrateOrderFile::saveIfModified(void) const
{
    this->p_rootCrate->saveIfModifiedAndRecurseToChildren();

    if (!this->p_rootCrate->childrenCratesWereModified()) {
        return;
    }

    string result;
    result += "[begin record]\n";
    this->p_rootCrate->addFullCrateNameWithPrefixAndRecurseToChildren(result, "[crate]");
    for (auto& crateName : *this->p_unknownCrates) {
        result += "[crate]";
        result += *(crateName.get());
        result += "\n";
    }
    result += "[end record]\n";

    CharVectorPtr content = make_unique<CharVector>(result.size() * 2, 0);
    writeStringAsUTF16At(result.c_str(), content->data());
    writeToFile(this->p_crateOrderFilePath->c_str(), *content);
}
