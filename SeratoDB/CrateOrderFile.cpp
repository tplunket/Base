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

#include "SeratoDB/CrateOrderFile.hpp"
#include "SeratoDB/Internal/CrateOrderFile.hpp"
#include "SeratoDB/Utility.hpp"
#include "SeratoDB/Crate.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, CrateOrderFile, Object);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

CrateOrderFile::Pointer CrateOrderFile::fileWithSeratoFolderInRootFolder(String::Pointer const& seratoFolderPath,
                                                                         String::Pointer const& rootFolderPath)
{
    auto rootCrate = Crate::crateWithNameInFolderOnVolume(String::string(),
                                                          String::string(),
                                                          String::string());
    auto internalObject = Internal::CrateOrderFile::Pointer(std::make_shared<Internal::CrateOrderFile>(crateOrderFilePathForSeratoFolder(seratoFolderPath),
                                                                                                       rootCrate));
    auto newCrateOrderFile = CrateOrderFile::makeSharedWithInternal(NxA::Internal::Object::Pointer::dynamicCastFrom(internalObject));

    auto subCratesDirectory = subCratesDirectoryPathInSeratoFolder(seratoFolderPath);
    auto subCratesFound = Internal::CrateOrderFile::cratesInSubCratesDirectory(subCratesDirectory);
    auto cratesInOrder = Internal::CrateOrderFile::readCratesNamesInCrateOrderFile(newCrateOrderFile->internal->crateOrderFilePath);

    Internal::CrateOrderFile::addCratesNamesAtTheStartOfUnlessAlreadyThere(cratesInOrder, subCratesFound);

    auto it = cratesInOrder->begin();
    auto crates = newCrateOrderFile->internal->childrenCratesOfCrateNamedUsingNameList(String::string(),
                                                                                       it,
                                                                                       cratesInOrder->end(),
                                                                                       seratoFolderPath,
                                                                                       rootFolderPath);
    for (auto& crate : *crates) {
        Crate::addCrateAsChildOfCrate(crate, newCrateOrderFile->internal->rootCrate);
    }

    newCrateOrderFile->internal->rootCrate->resetModificationFlags();

    return newCrateOrderFile;
}

#pragma mark Instance Methods

Crate::Pointer const& CrateOrderFile::rootCrate(void) const
{
    return internal->rootCrate;
}

timestamp CrateOrderFile::modificationDateInSecondsSince1970(void) const
{
    return File::modificationDateInSecondsSince1970ForFile(internal->crateOrderFilePath);
}

void CrateOrderFile::saveIfModified(void) const
{
    internal->rootCrate->saveIfModifiedAndRecurseToChildren();

    if (!internal->rootCrate->childrenCratesWereModified()) {
        return;
    }

    auto result = String::string();
    result->append("[begin record]\n");
    internal->rootCrate->addFullCrateNameWithPrefixAndRecurseToChildren(result, "[crate]");
    for (auto& crateName : *internal->unknownCratesNames) {
        result->append("[crate]");
        result->append(crateName);
        result->append("\n");
    }
    result->append("[end record]\n");

    File::writeBlobToFileAt(result->toUTF16(), internal->crateOrderFilePath);
}
