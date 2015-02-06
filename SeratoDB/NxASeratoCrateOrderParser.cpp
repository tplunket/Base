//
//  NxASeratoCrateOrderParser.cpp
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

#include "SeratoDB/NxASeratoCrateOrderParser.h"

using namespace NxA;

#pragma mark Utility Functions

static std::string* p_crateNameIfValidCrateOrEmptyStringIfNot(const std::string* name)
{
    if (name->find("[crate]") != 0) {
        return new std::string;
    }

    return new std::string(name->substr(7, name->length()));
}

#pragma mark Constructors

SeratoCrateOrderParser::SeratoCrateOrderParser(const void* startOfFile, unsigned long lengthInBytes)
{
    int numberOfCharacters = (int)lengthInBytes / 2;
    const char16_t* textToRead = (const char16_t*)startOfFile;
    StringAutoPtr textAString = convertUTF16ToStdString(textToRead, numberOfCharacters);

    StringVectorAutoPtr lines(splitStringIntoOneStringForEachLine(textAString.get()));
    for(StringVector::iterator it = lines->begin(); it != lines->end(); ++it) {
        const std::string* crateName = p_crateNameIfValidCrateOrEmptyStringIfNot(it->get());
        if (*crateName == "") {
            continue;
        }

        this->p_crateNames.push_back(StringAutoPtr(crateName));
    }
}

#pragma mark Instance Methods

const StringVector& SeratoCrateOrderParser::crateNamesInOrder(void)
{
    return this->p_crateNames;
}
