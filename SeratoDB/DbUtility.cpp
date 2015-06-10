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

#include "SeratoDB/DbUtility.hpp"

#include <stdlib.h>
#include <sys/stat.h>
#include <string>
#include <sstream>
#include <vector>
#include <codecvt>
#include <locale>
#include <iostream>
#include <fstream>

using namespace NxA::Serato;
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

static const char16_t* p_convertEndiannessOfUTF16Characters(const char16_t* characters, size_t numberOfCharacters)
{
    size_t numberOfBytes = numberOfCharacters * 2;
    char* newCharacters = (char*)malloc(numberOfBytes);

    for (int i = 0; i < numberOfBytes; i += 2) {
        newCharacters[i] = ((char*)characters)[i+1];
        newCharacters[i+1] = ((char*)characters)[i];
    }

    return (char16_t*)newCharacters;
}

ConstStringPtr NxA::Serato::convertUTF16ToStdString(const char16_t* characters, int numberOfCharacters)
{
#ifdef __LITTLE_ENDIAN__
    characters = p_convertEndiannessOfUTF16Characters(characters, numberOfCharacters);
#endif

    wstring_convert<codecvt_utf8_utf16<char16_t>,char16_t> convert;
    ConstStringPtr stdString = make_unique<string>(convert.to_bytes(characters, characters + numberOfCharacters));

#ifdef __LITTLE_ENDIAN__
    free((void*)characters);
#endif

    return move(stdString);
}

void NxA::Serato::writeStringAsUTF16At(const char* characters, void* destination)
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

StringVectorPtr NxA::Serato::splitStringIntoOneStringForEachLine(const string& text)
{
    StringVectorPtr results = make_unique<StringVector>();
    stringstream stream(text);
    string line;

    while(getline(stream, line, '\n')) {
        results->push_back(make_unique<string>(line));
    }

    return move(results);
}

bool NxA::Serato::stringHasPrefix(const string& stringToTest, const string& prefix)
{
    return stringToTest.find(prefix) == 0;
}

bool NxA::Serato::stringHasPostfix(const string& stringToTest, const string& postfix)
{
    return stringToTest.rfind(postfix) == (stringToTest.length() - postfix.length());
}

float NxA::Serato::bigEndianFloatValueAt(const void* ptr)
{
    const char* charsPtr = (const char*)ptr;
    uint32_t bigEndianVersion = ((charsPtr[0] << 24) & 0xff000000) |
    ((charsPtr[1] << 16) & 0xff0000) |
    ((charsPtr[2] << 8) & 0xff00) |
    (charsPtr[3] & 0xff);
    return *(float *)&bigEndianVersion;
}

uint32_t NxA::Serato::bigEndianUInt32ValueAt(const void* ptr)
{
    const char* charsPtr = (const char*)ptr;
    return ((charsPtr[0] << 24) & 0xff000000) |
    ((charsPtr[1] << 16) & 0xff0000) |
    ((charsPtr[2] << 8) & 0xff00) |
    (charsPtr[3] & 0xff);
}

uint16_t NxA::Serato::bigEndianUInt16ValueAt(const void* ptr)
{
    const char* charsPtr = (const char*)ptr;
    return ((charsPtr[0] << 8) & 0xff00) |
    (charsPtr[1] & 0xff);
}

void NxA::Serato::writeBigEndianFloatValueAt(float value, const void* ptr)
{
    char* charsPtr = (char*)ptr;
    const char* valuePtr = (const char*)&value;
    charsPtr[0] = valuePtr[3];
    charsPtr[1] = valuePtr[2];
    charsPtr[2] = valuePtr[1];
    charsPtr[3] = valuePtr[0];
}

void NxA::Serato::writeBigEndianUInt32ValueAt(uint32_t value, void* ptr)
{
    char* charsPtr = (char*)ptr;
    charsPtr[0] = (value >> 24) & 0xff;
    charsPtr[1] = (value >> 16) & 0xff;
    charsPtr[2] = (value >> 8) & 0xff;
    charsPtr[3] = value & 0xff;
}

void NxA::Serato::writeBigEndianUInt16ValueAt(uint16_t value, void* ptr)
{
    char* charsPtr = (char*)ptr;
    charsPtr[0] = (value >> 8) & 0xff;
    charsPtr[1] = value & 0xff;
}

ConstStringPtr NxA::Serato::seratoFolderPathForFolder(const char* folderPath)
{
    return joinPaths(folderPath, "_Serato_");
}

ConstStringPtr NxA::Serato::databaseFilePathForSeratoFolder(const char* seratoFolderPath)
{
    return joinPaths(seratoFolderPath, "database V2");
}

ConstStringPtr NxA::Serato::crateOrderFilePathForSeratoFolder(const char* seratoFolderPath)
{
    return joinPaths(seratoFolderPath, "neworder.pref");
}

ConstStringPtr NxA::Serato::subCratesDirectoryPathInSeratoFolder(const char* seratoFolderPath)
{
    return joinPaths(seratoFolderPath, "Subcrates");
}

ConstStringPtr NxA::Serato::smartCratesDirectoryPathInSeratoFolder(const char* seratoFolderPath)
{
    return joinPaths(seratoFolderPath, "SmartCrates");
}

ConstStringPtr NxA::Serato::crateFilePathForCrateNameInSeratoFolder(const char* crateName, const char* seratoFolderPath)
{
    ConstStringPtr cratesFolderPath = NxA::Serato::subCratesDirectoryPathInSeratoFolder(seratoFolderPath);
    ConstStringPtr crateFilePartialPath = joinPaths(cratesFolderPath->c_str(), crateName);
    return make_unique<string>(*crateFilePartialPath + ".crate");
}

ConstStringPtr NxA::Serato::crateFilePathForSmartCrateNameInSeratoFolder(const char* crateName, const char* seratoFolderPath)
{
    ConstStringPtr cratesFolderPath = NxA::Serato::smartCratesDirectoryPathInSeratoFolder(seratoFolderPath);
    ConstStringPtr crateFilePartialPath = joinPaths(cratesFolderPath->c_str(), crateName);
    return make_unique<string>(*crateFilePartialPath + ".scrate");
}

bool NxA::Serato::containsAValidSeratoFolder(const char* folderPath)
{
    ConstStringPtr seratoFolderPath = seratoFolderPathForFolder(folderPath);
    ConstStringPtr databaseFilePath = databaseFilePathForSeratoFolder(seratoFolderPath->c_str());
    ConstStringPtr crateOrderFilePath = NxA::Serato::crateOrderFilePathForSeratoFolder(seratoFolderPath->c_str());
    return fileExistsAt(databaseFilePath->c_str()) && fileExistsAt(crateOrderFilePath->c_str());
}

ConstStringPtr NxA::Serato::joinPaths(const char* firstPath, const char* secondPath)
{
    StringPtr result = make_unique<string>(firstPath);

#ifdef NXASeratoCompiledOnOSX
    *result += "/";
#else
    *result += "\";
#endif

    *result += secondPath;

    return move(result);
}

ConstStringPtr NxA::Serato::removePrefixFromPath(const char* prefixPath, const char* fullPath)
{
    string path(fullPath);

    size_t lengthToCrop = strlen(prefixPath);
    path = path.substr(lengthToCrop);

    return make_unique<string>(path);
}

bool NxA::Serato::fileExistsAt(const char* filePath)
{
    struct stat buf;
    return (stat(filePath, &buf) != -1);
}

size_t NxA::Serato::sizeOfFileAt(const char* filePath)
{
    struct stat buf;
    if (stat(filePath, &buf) == -1) {
        return 0;
    }

    return buf.st_size;
}

time_t NxA::Serato::modificationDateInSecondsSince1970ForFile(const char* filePath)
{
    struct stat buf;
    if (stat(filePath, &buf) == -1) {
        return 0;
    }

    return buf.st_mtimespec.tv_sec;
}

CharVectorPtr NxA::Serato::readFileAt(const char* filePath)
{
    size_t fileSize = NxA::Serato::sizeOfFileAt(filePath);
    if (fileSize) {
        char* fileData = (char*)malloc(fileSize);
        if (fileData) {
            fstream file(filePath, ios::in | ios::binary);
            file.read(fileData, fileSize);
            file.close();

            CharVectorPtr result = make_unique<CharVector>(fileData, fileData + fileSize);
            free(fileData);

            return move(result);
        }
    }

    return make_unique<CharVector>();
}

void NxA::Serato::deleteFileAt(const char* filePath)
{
    ::remove(filePath);
}

void NxA::Serato::writeToFile(const char* filePath, const CharVector& content)
{
    fstream file(filePath, ios::out | ios::binary);
    file.write(content.data(), content.size());
    file.close();
}
