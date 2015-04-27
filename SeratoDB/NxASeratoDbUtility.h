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

#pragma once

#include <string>
#include <vector>

namespace NxA {
    #pragma mark Containers
    typedef std::unique_ptr<const std::string> ConstStringPtr;
    typedef std::unique_ptr<std::string> StringPtr;
    typedef std::vector<StringPtr> StringVector;
    typedef std::unique_ptr<StringVector> StringVectorPtr;
    typedef std::vector<char> CharVector;
    typedef std::unique_ptr<CharVector> CharVectorPtr;
    typedef std::unique_ptr<const CharVector> ConstCharVectorPtr;
    typedef std::vector<CharVectorPtr> CharVectorPtrVector;

    #pragma mark Utility Functions
    ConstStringPtr convertUTF16ToStdString(const char16_t* characters, int numberOfCharacters);
    void writeStringAsUTF16At(const char* characters, void* destination);

    StringVectorPtr splitStringIntoOneStringForEachLine(const std::string& text);
    bool stringHasPrefix(const std::string& stringToTest, const std::string& prefix);
    bool stringHasPostfix(const std::string& stringToTest, const std::string& postfix);

    float bigEndianFloatValueAt(const void* ptr);
    uint32_t bigEndianUInt32ValueAt(const void* ptr);
    uint16_t bigEndianUInt16ValueAt(const void* ptr);
    void writeBigEndianFloatValueAt(float value, const void* ptr);
    void writeBigEndianUInt32ValueAt(uint32_t value, void* ptr);
    void writeBigEndianUInt16ValueAt(uint16_t value, void* ptr);

    bool containsAValidSeratoFolder(const char* folderPath);

    ConstStringPtr seratoFolderPathForFolder(const char* folderPath);
    ConstStringPtr databaseFilePathForSeratoFolder(const char* seratoFolderPath);
    ConstStringPtr crateOrderFilePathForSeratoFolder(const char* seratoFolderPath);
    ConstStringPtr subCratesDirectoryPathInSeratoFolder(const char* seratoFolderPath);
    ConstStringPtr smartCratesDirectoryPathInSeratoFolder(const char* seratoFolderPath);
    ConstStringPtr crateFilePathForCrateNameInSeratoFolder(const char* crateName, const char* seratoFolderPath);
    ConstStringPtr crateFilePathForSmartCrateNameInSeratoFolder(const char* crateName, const char* seratoFolderPath);

    ConstStringPtr joinPaths(const char* firstPath, const char* secondPath);
    ConstStringPtr removePrefixFromPath(const char* prefixPath, const char* fullPath);

    bool fileExistsAt(const char* filePath);
    size_t sizeOfFileAt(const char* filePath);
    time_t modificationDateInSecondsSince1970ForFile(const char* filePath);

    CharVectorPtr readFileAt(const char* filePath);
    void deleteFileAt(const char* filePath);
    void writeToFile(const char* filePath, const CharVector& content);
}
