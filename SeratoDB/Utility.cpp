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

#include <Base/File.hpp>

#include "Utility.hpp"

#include <cstdlib>
#include <sys/stat.h>
#include <string>
#include <sstream>
#include <vector>
#include <codecvt>
#include <locale>
#include <iostream>
#include <fstream>

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Utility Functions

String::Pointer NxA::Serato::seratoFolderPathForFolder(const String& folderPath)
{
    auto joinedPath = File::joinPaths(folderPath, String::stringWith("_Serato_"));
    return joinedPath;
}

String::Pointer NxA::Serato::databaseFilePathForSeratoFolder(const String& seratoFolderPath)
{
    auto joinedPath = File::joinPaths(seratoFolderPath, String::stringWith("database V2"));
    return joinedPath;
}

String::Pointer NxA::Serato::crateOrderFilePathForSeratoFolder(const String& seratoFolderPath)
{
    auto joinedPath = File::joinPaths(seratoFolderPath, String::stringWith("neworder.pref"));
    return joinedPath;
}

String::Pointer NxA::Serato::subCratesDirectoryPathInSeratoFolder(const String& seratoFolderPath)
{
    auto joinedPath = File::joinPaths(seratoFolderPath, String::stringWith("Subcrates"));
    return joinedPath;
}

String::Pointer NxA::Serato::smartCratesDirectoryPathInSeratoFolder(const String& seratoFolderPath)
{
    auto joinedPath = File::joinPaths(seratoFolderPath, String::stringWith("SmartCrates"));
    return joinedPath;
}

String::Pointer NxA::Serato::crateFilePathForCrateNameInSeratoFolder(const String& crateName,
                                                                          const String& seratoFolderPath)
{
    auto cratesFolderPath = subCratesDirectoryPathInSeratoFolder(seratoFolderPath);
    auto crateFilePartialPath = File::joinPaths(cratesFolderPath, crateName);
    crateFilePartialPath->append(".crate");

    return crateFilePartialPath;
}

String::Pointer NxA::Serato::crateFilePathForSmartCrateNameInSeratoFolder(const String& crateName,
                                                                               const String& seratoFolderPath)
{
    auto cratesFolderPath = subCratesDirectoryPathInSeratoFolder(seratoFolderPath);
    auto crateFilePartialPath = File::joinPaths(cratesFolderPath, crateName);
    crateFilePartialPath->append(".scrate");

    return crateFilePartialPath;
}

boolean NxA::Serato::containsAValidSeratoFolder(const String& folderPath)
{
    auto seratoFolderPath = seratoFolderPathForFolder(folderPath);
    auto databaseFilePath = databaseFilePathForSeratoFolder(seratoFolderPath);
    return File::fileExistsAt(databaseFilePath);
}
