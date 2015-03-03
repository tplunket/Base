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

#include <string>

using namespace NxA;
using namespace std;

#pragma mark Utility Functions

static bool p_stringHasPrefix(const string& stringToTest, const string& prefix)
{
    return stringToTest.find(prefix) == 0;
}

static ConstStringPtr p_crateNameIfValidCrateOrNullIfNot(const string& name)
{
    ConstStringPtr result;

    if (p_stringHasPrefix(name, "[crate]")) {
        result = make_unique<string>(name.substr(7, name.length() - 7));
    }

    return move(result);
}

#pragma mark Constructors

SeratoCrateOrderFile::SeratoCrateOrderFile(const char* seratoFolderPath, const char* rootFolderPath) :
                                           p_crateOrderFilePath(crateOrderFilePathForSeratoFolder(seratoFolderPath)),
                                           p_unknownCrates(make_unique<StringVector>()),
                                           p_rootCrate(make_unique<SeratoCrate>("", "", ""))
{
    CharVectorPtr crateOrderFile = readFileAt(this->p_crateOrderFilePath->c_str());
    if (!crateOrderFile->size()) {
        return;
    }

    const char16_t* textToRead = (const char16_t*)crateOrderFile->data();
    int numberOfCharacters = (int)crateOrderFile->size() / 2;
    ConstStringPtr textAString = convertUTF16ToStdString(textToRead, numberOfCharacters);

    StringVectorPtr lines(splitStringIntoOneStringForEachLine(*textAString));
    StringVector::iterator it = lines->begin();

    SeratoCrateVectorPtr crates = this->p_childrenCratesOfCrateNamedUsingNameList("", it, lines->end(), seratoFolderPath, rootFolderPath);
    for (auto& crate : *crates) {
        this->p_rootCrate->addChildCrate(move(crate));
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
        const ConstStringPtr fullCrateName = p_crateNameIfValidCrateOrNullIfNot(*(it->get()));
        if (!fullCrateName.get()) {
            ++it;
            continue;
        }

        if (name.length() && !p_stringHasPrefix(*fullCrateName, name)) {
            break;
        }

        if (!SeratoCrate::isAValidCrateName(fullCrateName->c_str(), seratoFolderPath)) {
            this->p_unknownCrates->push_back(make_unique<string>(*(fullCrateName.get())));
            ++it;
            continue;
        }

        SeratoCratePtr newCrate = make_unique<SeratoCrate>(fullCrateName->c_str(), seratoFolderPath, rootFolderPath);
        newCrate->loadFromFile();

        cratesFound->push_back(move(newCrate));

        ++it;

        string crateNameWithSeperator = *fullCrateName;
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
    }
    result += "[end record]\n";

    CharVector content(result.c_str(), result.c_str() + result.length());
    writeToFile(this->p_crateOrderFilePath->c_str(), content);
}
