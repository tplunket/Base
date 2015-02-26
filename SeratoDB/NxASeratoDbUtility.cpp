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
#include <sys/stat.h>
#include <string>
#include <sstream>
#include <vector>
#include <codecvt>
#include <locale>
#include <iostream>
#include <fstream>

using namespace NxA;
using namespace std;

#pragma mark Platform Defines

#ifndef _Windows
    #if defined(_WIN32) || defined(WIN32) || defined(__Win32__) || defined(__WIN32__) || defined(_WINDOWS)
        #define NXASeratoCompiledOnWindows
    #endif
#else
    #if defined(__Win32__) || defined (_WIN32)
        #define NXASeratoCompiledOnWindows
    #endif
#endif

#ifndef NXASeratoCompiledOnWindows
    #if defined(__APPLE__) && defined(__MACH__)
        #define NXASeratoCompiledOnOSX
    #else
        #error Unsupported platform.
    #endif
#endif

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

    void writeStringAsUTF16At(const char* characters, void* destination)
    {
        wstring_convert<codecvt_utf8_utf16<char16_t>,char16_t> convert;
        std::u16string u16 = convert.from_bytes(characters);

        const char16_t* result = u16.c_str();
        #ifdef __LITTLE_ENDIAN__
        result = p_convertEndiannessOfUTF16Characters(result, u16.length());
        #endif

        memcpy(destination, result, u16.length() * 2);

        #ifdef __LITTLE_ENDIAN__
        free((void*)result);
        #endif
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

    void writeBigEndianUInt32ValueAt(uint32_t value, void* ptr)
    {
        char* charsPtr = (char*)ptr;
        charsPtr[0] = (value >> 24) & 0xff;
        charsPtr[1] = (value >> 16) & 0xff;
        charsPtr[2] = (value >> 8) & 0xff;
        charsPtr[3] = value & 0xff;
    }

    void writeBigEndianUInt16ValueAt(uint16_t value, void* ptr)
    {
        char* charsPtr = (char*)ptr;
        charsPtr[0] = (value >> 8) & 0xff;
        charsPtr[1] = value & 0xff;
    }
    
    StringAutoPtr seratoFolderPathForFolder(const char* folderPath)
    {
        return joinPaths(folderPath, "_Serato_");
    }

    StringAutoPtr databaseFilePathForSeratoFolder(const char* seratoFolderPath)
    {
        return joinPaths(seratoFolderPath, "database V2");
    }

    StringAutoPtr crateOrderFilePathForSeratoFolder(const char* seratoFolderPath)
    {
        return joinPaths(seratoFolderPath, "neworder.pref");
    }

    StringAutoPtr crateFilePathForCrateNameInSeratoFolder(const char* crateName, const char* seratoFolderPath)
    {
        StringAutoPtr cratesFolderPath = joinPaths(seratoFolderPath, "Subcrates");
        StringAutoPtr crateFilePartialPath = joinPaths(cratesFolderPath->c_str(), crateName);
        return StringAutoPtr(new string(*crateFilePartialPath + ".crate"));
    }

    bool containsAValidSeratoFolder(const char* folderPath)
    {
        StringAutoPtr seratoFolderPath = seratoFolderPathForFolder(folderPath);
        StringAutoPtr databaseFilePath = databaseFilePathForSeratoFolder(seratoFolderPath->c_str());
        StringAutoPtr crateOrderFilePath = crateOrderFilePathForSeratoFolder(seratoFolderPath->c_str());
        return fileExistsAt(databaseFilePath->c_str()) && fileExistsAt(crateOrderFilePath->c_str());
    }

    StringAutoPtr joinPaths(const char* firstPath, const char* secondPath)
    {
        string* result = new string(firstPath);

        #ifdef NXASeratoCompiledOnOSX
        *result += "/";
        #else
        *result += "\";
        #endif

        *result += secondPath;

        return StringAutoPtr(result);
    }

    bool fileExistsAt(const char* filePath)
    {
        struct stat buf;
        return (stat(filePath, &buf) != -1);
    }

    size_t sizeOfFileAt(const char* filePath)
    {
        struct stat buf;
        if (stat(filePath, &buf) == -1) {
            return 0;
        }

        return buf.st_size;
    }
    
    time_t modificationDateInSecondsSince1970ForFile(const char* filePath)
    {
        struct stat buf;
        if (stat(filePath, &buf) == -1) {
            return 0;
        }

        return buf.st_mtimespec.tv_sec;
    }
    
    CharVectorAutoPtr readFileAt(const char* filePath)
    {
        size_t fileSize = sizeOfFileAt(filePath);
        if (fileSize) {
            char* fileData = (char*)malloc(fileSize);
            if (fileData) {
                fstream file(filePath, ios::in | ios::binary);
                file.read(fileData, fileSize);
                file.close();

                CharVector* result = new CharVector(fileData, fileData + fileSize);
                free(fileData);

                return CharVectorAutoPtr(result);
            }
        }

        return CharVectorAutoPtr(new CharVector);
    }
}
