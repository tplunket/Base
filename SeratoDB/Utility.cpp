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

String::ConstPointer NxA::Serato::seratoFolderPathForFolder(String::ConstPointer const& folderPath)
{
    auto joinedPath = File::joinPaths(folderPath, String::stringWith("_Serato_"));
    return joinedPath;
}

String::ConstPointer NxA::Serato::databaseFilePathForSeratoFolder(String::ConstPointer const& seratoFolderPath)
{
    auto joinedPath = File::joinPaths(seratoFolderPath, String::stringWith("database V2"));
    return joinedPath;
}

String::ConstPointer NxA::Serato::crateOrderFilePathForSeratoFolder(String::ConstPointer const& seratoFolderPath)
{
    auto joinedPath = File::joinPaths(seratoFolderPath, String::stringWith("neworder.pref"));
    return joinedPath;
}

String::ConstPointer NxA::Serato::subCratesDirectoryPathInSeratoFolder(String::ConstPointer const& seratoFolderPath)
{
    auto joinedPath = File::joinPaths(seratoFolderPath, String::stringWith("Subcrates"));
    return joinedPath;
}

String::ConstPointer NxA::Serato::smartCratesDirectoryPathInSeratoFolder(String::ConstPointer const& seratoFolderPath)
{
    auto joinedPath = File::joinPaths(seratoFolderPath, String::stringWith("SmartCrates"));
    return joinedPath;
}

String::ConstPointer NxA::Serato::crateFilePathForCrateNameInSeratoFolder(String::ConstPointer const& crateName,
                                                                          String::ConstPointer const& seratoFolderPath)
{
    auto cratesFolderPath = subCratesDirectoryPathInSeratoFolder(seratoFolderPath);
    auto crateFilePartialPath = File::joinPaths(cratesFolderPath, crateName);
    crateFilePartialPath->append(".crate");

    return crateFilePartialPath;
}

String::ConstPointer NxA::Serato::crateFilePathForSmartCrateNameInSeratoFolder(String::ConstPointer const& crateName,
                                                                               String::ConstPointer const& seratoFolderPath)
{
    auto cratesFolderPath = subCratesDirectoryPathInSeratoFolder(seratoFolderPath);
    auto crateFilePartialPath = File::joinPaths(cratesFolderPath, crateName);
    crateFilePartialPath->append(".scrate");

    return crateFilePartialPath;
}

bool NxA::Serato::containsAValidSeratoFolder(String::ConstPointer const& folderPath)
{
    auto seratoFolderPath = seratoFolderPathForFolder(folderPath);
    auto databaseFilePath = databaseFilePathForSeratoFolder(seratoFolderPath);
    auto crateOrderFilePath = crateOrderFilePathForSeratoFolder(seratoFolderPath);
    return File::fileExistsAt(databaseFilePath) && File::fileExistsAt(crateOrderFilePath);
}
