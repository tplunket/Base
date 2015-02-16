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

static string* p_crateNameIfValidCrateOrEmptyStringIfNot(const string* name)
{
    if (name->find("[crate]") != 0) {
        return new string;
    }

    return new string(name->substr(7, name->length()));
}

#pragma mark Constructors

SeratoCrateOrderFile::SeratoCrateOrderFile(const char* seratoFolderPath)
{
    StringAutoPtr crateOrderFilePath = crateOrderFilePathForSeratoFolder(seratoFolderPath);
    CharVectorAutoPtr crateOrderFile = readFileAt(crateOrderFilePath->c_str());

    const void* startOfFile = crateOrderFile->data();
    size_t lengthInBytes = crateOrderFile->size();

    int numberOfCharacters = (int)lengthInBytes / 2;
    const char16_t* textToRead = (const char16_t*)startOfFile;
    StringAutoPtr textAString = convertUTF16ToStdString(textToRead, numberOfCharacters);

    StringVectorAutoPtr lines(splitStringIntoOneStringForEachLine(*textAString));
    for(StringVector::iterator it = lines->begin(); it != lines->end(); ++it) {
        const string* crateName = p_crateNameIfValidCrateOrEmptyStringIfNot(it->get());
        if (*crateName == "") {
            continue;
        }

        this->p_crateNames.push_back(StringAutoPtr(crateName));
    }
}

#pragma mark Instance Methods

const StringVector& SeratoCrateOrderFile::crateNamesInOrder(void)
{
    return this->p_crateNames;
}
