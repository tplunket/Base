//
//  NxASeratoDbUtility.cpp
//  SeratoDB
//
//  Created by Didier Malenfant on 2/4/15.
//  Copyright (c) 2015 Next Audio Labs, LLC. All rights reserved.
//

#include "NxASeratoDbUtility.h"

#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <codecvt>
#include <locale>

using namespace NxA;

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

        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
        std::string* stdString = new std::string(convert.to_bytes(characters, characters + numberOfCharacters));

        #ifdef __LITTLE_ENDIAN__
        free((void*)characters);
        #endif

        return StringAutoPtr(stdString);
    }

    StringVectorAutoPtr splitStringIntoOneStringForEachLine(const std::string* text)
    {
        StringVector* results = new StringVector;
        if (text != NULL) {
            std::stringstream stream(*text);
            std::string line;

            while(std::getline(stream, line, '\n')) {
                const std::string* result = new std::string(line);
                results->push_back(StringAutoPtr(result));
            }
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
