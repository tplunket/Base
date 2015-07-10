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
    #pragma mark Utility Functions
    String::Pointer seratoFolderPathForFolder(const String& folderPath);
    String::Pointer databaseFilePathForSeratoFolder(const String& seratoFolderPath);
    String::Pointer crateOrderFilePathForSeratoFolder(const String& seratoFolderPath);
    String::Pointer subCratesDirectoryPathInSeratoFolder(const String& seratoFolderPath);
    String::Pointer smartCratesDirectoryPathInSeratoFolder(const String& seratoFolderPath);
    String::Pointer crateFilePathForCrateNameInSeratoFolder(const String& crateName,
                                                                 const String& seratoFolderPath);
    String::Pointer crateFilePathForSmartCrateNameInSeratoFolder(const String& crateName,
                                                                      const String& seratoFolderPath);
    boolean containsAValidSeratoFolder(const String& folderPath);
} }
