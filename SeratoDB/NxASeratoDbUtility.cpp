//
//  NxASeratoDbUtility.cpp
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

#include "SeratoDB/NxASeratoDbUtility.h"

#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <codecvt>
#include <locale>

using namespace NxA;
using namespace std;

#pragma mark Utility Functions

namespace NxA {
    const char16_t* p_convertEndiannessOfUTF16Characters(const char16_t* characters, int numberOfCharacters)
    {
        int numberOfBytes = numberOfCharacters * 2;
        char* newCharacters = (char*)malloc(numberOfBytes);

        for (int i = 0; i < numberOfBytes; i += 2) {
            newCharacters[i] = ((char*)characters)[i+1];
            newCharacters[i+1] = ((char*)characters)[i];
        }

        return (char16_t*)newCharacters;
    }

    StringAutoPtr convertUTF16ToStdString(const char16_t* characters, int numberOfCharacters)
    {
        #ifdef __LITTLE_ENDIAN__
        characters = p_convertEndiannessOfUTF16Characters(characters, numberOfCharacters);
        #endif

        wstring_convert<codecvt_utf8_utf16<char16_t>,char16_t> convert;
        string* stdString = new string(convert.to_bytes(characters, characters + numberOfCharacters));

        #ifdef __LITTLE_ENDIAN__
        free((void*)characters);
        #endif

        return StringAutoPtr(stdString);
    }

    StringVectorAutoPtr splitStringIntoOneStringForEachLine(const string& text)
    {
        StringVector* results = new StringVector;
        stringstream stream(text);
        string line;

        while(getline(stream, line, '\n')) {
            const string* result = new string(line);
            results->push_back(StringAutoPtr(result));
        }

        return StringVectorAutoPtr(results);
    }

    uint32_t bigEndianUInt32ValueAt(const void* ptr)
    {
        const char* charsPtr = (const char*)ptr;
        return ((charsPtr[0] << 24) & 0xff000000) |
               ((charsPtr[1] << 16) & 0xff0000) |
               ((charsPtr[2] << 8) & 0xff00) |
                (charsPtr[3] & 0xff);
    }

    uint16_t bigEndianUInt16ValueAt(const void* ptr)
    {
        const char* charsPtr = (const char*)ptr;
        return ((charsPtr[0] << 8) & 0xff00) |
                (charsPtr[1] & 0xff);
    }
}

