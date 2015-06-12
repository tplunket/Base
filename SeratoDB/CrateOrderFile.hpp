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

#include <SeratoDB/Utility.hpp>
#include <SeratoDB/Crate.hpp>

namespace NxA { namespace Serato {
    #pragma mark Forward declarations
    class CrateOrderFile;
    class Database;

    #pragma mark Containers
    typedef std::unique_ptr<const CrateOrderFile> CrateOrderFilePtr;

    #pragma mark Class Declaration
    class CrateOrderFile
    {
    private:
        #pragma mark Private Instance Variables
        ConstStringPtr p_crateOrderFilePath;

        CratePtr p_rootCrate;

        StringVectorPtr p_unknownCrates;

        #pragma mark Private Instance Methods
        CrateVectorPtr p_childrenCratesOfCrateNamedUsingNameList(const std::string& name,
                                                                 StringVector::iterator& it,
                                                                 const StringVector::iterator& end,
                                                                 const char* seratoFolderPath,
                                                                 const char* rootFolderPath,
                                                                 Database& database);

    public:
        #pragma mark Constructors
        explicit CrateOrderFile(const char* seratoFolderPath,
                                const char* rootFolderPath,
                                Database& database);

        #pragma mark Instance Methods
        const Crate* rootCrate(void) const;

        time_t modificationDateInSecondsSince1970(void) const;

        void saveIfModified(void) const;
    };
} }
