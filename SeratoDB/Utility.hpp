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

#include <Base/String.hpp>

#include <string>
#include <vector>

namespace NxA { namespace Serato {
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
    bool containsAValidSeratoFolder(const char* folderPath);

    String::ConstPointer seratoFolderPathForFolder(const char* folderPath);
    String::ConstPointer databaseFilePathForSeratoFolder(const char* seratoFolderPath);
    String::ConstPointer crateOrderFilePathForSeratoFolder(const char* seratoFolderPath);
    String::ConstPointer subCratesDirectoryPathInSeratoFolder(const char* seratoFolderPath);
    String::ConstPointer smartCratesDirectoryPathInSeratoFolder(const char* seratoFolderPath);
    String::ConstPointer crateFilePathForCrateNameInSeratoFolder(const char* crateName, const char* seratoFolderPath);
    String::ConstPointer crateFilePathForSmartCrateNameInSeratoFolder(const char* crateName, const char* seratoFolderPath);
} }
