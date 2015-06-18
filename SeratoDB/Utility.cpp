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

String::ConstPointer NxA::Serato::seratoFolderPathForFolder(const char* folderPath)
{
    auto joinedPath = File::joinPaths(String::stringWithUTF8(folderPath),
                                      String::stringWithUTF8("_Serato_"));
    return joinedPath;
}

String::ConstPointer NxA::Serato::databaseFilePathForSeratoFolder(const char* seratoFolderPath)
{
    auto joinedPath = File::joinPaths(String::stringWithUTF8(seratoFolderPath),
                                      String::stringWithUTF8("database V2"));
    return joinedPath;
}

String::ConstPointer NxA::Serato::crateOrderFilePathForSeratoFolder(const char* seratoFolderPath)
{
    auto joinedPath = File::joinPaths(String::stringWithUTF8(seratoFolderPath),
                                      String::stringWithUTF8("neworder.pref"));
    return joinedPath;
}

String::ConstPointer NxA::Serato::subCratesDirectoryPathInSeratoFolder(const char* seratoFolderPath)
{
    auto joinedPath = File::joinPaths(String::stringWithUTF8(seratoFolderPath),
                                      String::stringWithUTF8("Subcrates"));
    return joinedPath;
}

String::ConstPointer NxA::Serato::smartCratesDirectoryPathInSeratoFolder(const char* seratoFolderPath)
{
    auto joinedPath = File::joinPaths(String::stringWithUTF8(seratoFolderPath),
                                      String::stringWithUTF8("SmartCrates"));
    return joinedPath;
}

String::ConstPointer NxA::Serato::crateFilePathForCrateNameInSeratoFolder(const char* crateName, const char* seratoFolderPath)
{
    auto cratesFolderPath = subCratesDirectoryPathInSeratoFolder(seratoFolderPath);
    auto crateFilePartialPath = File::joinPaths(cratesFolderPath, String::stringWithUTF8(crateName));
    crateFilePartialPath->append(".crate");

    return crateFilePartialPath;
}

String::ConstPointer NxA::Serato::crateFilePathForSmartCrateNameInSeratoFolder(const char* crateName, const char* seratoFolderPath)
{
    auto cratesFolderPath = subCratesDirectoryPathInSeratoFolder(seratoFolderPath);
    auto crateFilePartialPath = File::joinPaths(cratesFolderPath, String::stringWithUTF8(crateName));
    crateFilePartialPath->append(".scrate");

    return crateFilePartialPath;
}

bool NxA::Serato::containsAValidSeratoFolder(const char* folderPath)
{
    auto seratoFolderPath = seratoFolderPathForFolder(folderPath);
    auto databaseFilePath = databaseFilePathForSeratoFolder(seratoFolderPath->toUTF8());
    auto crateOrderFilePath = crateOrderFilePathForSeratoFolder(seratoFolderPath->toUTF8());
    return File::fileExistsAt(databaseFilePath) && File::fileExistsAt(crateOrderFilePath);
}
