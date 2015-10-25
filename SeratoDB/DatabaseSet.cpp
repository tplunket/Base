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

#include "SeratoDB/DatabaseSet.hpp"
#include "SeratoDb/Internal/DatabaseSet.hpp"
#include "SeratoDB/Database.hpp"
#include "Tags/DatabaseV2Tags.hpp"
#include "Tags/TagFactory.hpp"
#include "Tags/VersionTag.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, DatabaseSet, Object);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

DatabaseSet::Pointer DatabaseSet::databaseSet(void)
{
    auto newDatabaseSet = DatabaseSet::makeShared();
    return newDatabaseSet;
}

#pragma mark Instance Methods

Crate& DatabaseSet::rootCrate(void) const
{
    return internal->rootCrate;
}

Database::Array& DatabaseSet::databases(void) const
{
    return internal->databases;
}

Database::Pointer DatabaseSet::addDatabaseWithFileAndVolume(const String& seratoFilePath,
                                                            const String& volume)
{
    auto db = Serato::Database::databaseWithFileAndVolume(seratoFilePath, volume);
    internal->databases->append(db);
    return db;
}

void DatabaseSet::removeDatabase(Database& db)
{
    internal->databases->remove(db);
}

void DatabaseSet::saveIfModified(void) const
{
    auto& dbs = this->databases();
    for (auto iter = dbs.begin(); iter != dbs.end(); ++iter) {
        (*iter)->saveIfModified();
    }
}
