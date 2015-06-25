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
    String::ConstPointer seratoFolderPathForFolder(String::ConstPointer const& folderPath);
    String::ConstPointer databaseFilePathForSeratoFolder(String::ConstPointer const& seratoFolderPath);
    String::ConstPointer crateOrderFilePathForSeratoFolder(String::ConstPointer const& seratoFolderPath);
    String::ConstPointer subCratesDirectoryPathInSeratoFolder(String::ConstPointer const& seratoFolderPath);
    String::ConstPointer smartCratesDirectoryPathInSeratoFolder(String::ConstPointer const& seratoFolderPath);
    String::ConstPointer crateFilePathForCrateNameInSeratoFolder(String::ConstPointer const& crateName,
                                                                 String::ConstPointer const& seratoFolderPath);
    String::ConstPointer crateFilePathForSmartCrateNameInSeratoFolder(String::ConstPointer const& crateName,
                                                                      String::ConstPointer const& seratoFolderPath);
    bool containsAValidSeratoFolder(String::ConstPointer const& folderPath);
} }
