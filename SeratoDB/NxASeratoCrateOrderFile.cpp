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

static StringAutoPtr p_crateNameIfValidCrateOrNullIfNot(const string& name)
{
    string* result = NULL;

    if (p_stringHasPrefix(name, "[crate]")) {
        result = new string(name.substr(7, name.length() - 7));
    }

    return StringAutoPtr(result);
}

#pragma mark Constructors

SeratoCrateOrderFile::SeratoCrateOrderFile(const char* seratoFolderPath, const char* rootFolderPath) :
        p_crateOrderFilePath(crateOrderFilePathForSeratoFolder(seratoFolderPath)),
        p_unknownCrates(StringVectorAutoPtr(new StringVector)),
        p_rootCrate(SeratoCrateAutoPtr(new SeratoCrate("", "", "")))
{
    CharVectorAutoPtr crateOrderFile = readFileAt(this->p_crateOrderFilePath->c_str());
    if (!crateOrderFile->size()) {
        return;
    }

    const char16_t* textToRead = (const char16_t*)crateOrderFile->data();
    int numberOfCharacters = (int)crateOrderFile->size() / 2;
    StringAutoPtr textAString = convertUTF16ToStdString(textToRead, numberOfCharacters);

    StringVectorAutoPtr lines(splitStringIntoOneStringForEachLine(*textAString));
    StringVector::iterator it = lines->begin();

    SeratoCrateVectorAutoPtr crates = this->p_childrenCratesOfCrateNamedUsingNameList("", it, lines->end(), seratoFolderPath, rootFolderPath);
    for (SeratoCrateVector::iterator it = crates->begin(); it != crates->end(); ++it) {
        this->p_rootCrate->addChildCrate(*it);
    }

    this->p_rootCrate->setAsNotModified();
}

#pragma mark Instance Methods

SeratoCrateVectorAutoPtr SeratoCrateOrderFile::p_childrenCratesOfCrateNamedUsingNameList(const string& name,
                                                                                         StringVector::iterator& it,
                                                                                         const StringVector::iterator& end,
                                                                                         const char* seratoFolderPath,
                                                                                         const char* rootFolderPath)
{
    SeratoCrateVector* cratesFound = new SeratoCrateVector;

    while (it != end) {
        const StringAutoPtr fullCrateName = p_crateNameIfValidCrateOrNullIfNot(*(it->get()));
        if (!fullCrateName.get()) {
            ++it;
            continue;
        }

        if (name.length() && !p_stringHasPrefix(*fullCrateName, name)) {
            break;
        }

        if (!SeratoCrate::isAValidCrateName(fullCrateName->c_str(), seratoFolderPath)) {
            this->p_unknownCrates->push_back(StringAutoPtr(new string(*(fullCrateName.get()))));
            ++it;
            continue;
        }

        SeratoCrate* newCrate = new SeratoCrate(fullCrateName->c_str(), seratoFolderPath, rootFolderPath);
        newCrate->loadFromFile();

        cratesFound->push_back(SeratoCrateAutoPtr(newCrate));

        ++it;

        string crateNameWithSeperator = *fullCrateName;
        crateNameWithSeperator += "%%";

        SeratoCrateVectorAutoPtr childCrates = p_childrenCratesOfCrateNamedUsingNameList(crateNameWithSeperator,
                                                                                         it,
                                                                                         end,
                                                                                         seratoFolderPath,
                                                                                         rootFolderPath);
        for (SeratoCrateVector::iterator childrenIt = childCrates->begin(); childrenIt != childCrates->end(); ++childrenIt) {
            newCrate->addChildCrate(*childrenIt);
        }

        newCrate->setAsNotModified();
    }
    
    return SeratoCrateVectorAutoPtr(cratesFound);
}

const SeratoCrate* SeratoCrateOrderFile::rootCrate(void) const
{
    return this->p_rootCrate.get();
}

const void SeratoCrateOrderFile::saveFile(void) const
{
    string result;
    result += "[begin record]\n";
    this->p_rootCrate->addFullCrateNameWithPrefixAndRecurseToChildren(result, "[crate]");
    for (StringVector::iterator it = this->p_unknownCrates->begin(); it != this->p_unknownCrates->end(); ++it) {
        result += "[crate]";
        result += *(it->get());
    }
    result += "[end record]\n";

    CharVector content(result.c_str(), result.c_str() + result.length());
    writeToFile(this->p_crateOrderFilePath->c_str(), content);
}
