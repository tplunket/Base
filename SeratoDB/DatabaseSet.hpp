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

#include <Base/Base.hpp>
#include <SeratoDB/Database.hpp>

namespace NxA { namespace Serato {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(DatabaseSet);

    class Crate;
    class Database;

    class DatabaseSet : public Object {
        NXA_GENERATED_OPERATOR_EQUAL_DECLARATION_IN_NAMESPACE_FOR_CLASS(NxA::Serato, DatabaseSet);
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(NxA::Serato, DatabaseSet);

    public:
        #pragma mark Factory Methods
        static DatabaseSet::Pointer databaseSet(void);

        #pragma mark Instance Methods
        Crate& rootCrate(void) const;
        NxA::ArrayContainer<Database>& databases(void) const;

        Database::Pointer addDatabaseWithFileAndVolume(const String& seratoFilePath,
                                                       const String& volume);
        void removeDatabase(Database& db);

        void saveIfModified(String::Array& simpleWarningLog) const;
    };
} }
