//
//  NxASeratoDatabaseParser.h
//  SeratoDB
//
//  Created by Didier Malenfant on 1/30/15.
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

#ifndef __SeratoDB__NxASeratoDatabaseParser__
#define __SeratoDB__NxASeratoDatabaseParser__

#include "SeratoDB/NxASeratoDatabaseTag.h"

#include <stdio.h>
#include <string>

#pragma mark Class Declaration

namespace NxA {
    class SeratoDatabaseParser
    {
    private:
        SeratoDatabaseTagVector* p_tags;

    public:
        #pragma mark Constructors
        SeratoDatabaseParser(const void* startOfFile, unsigned long lengthInBytes);

        #pragma mark Destructor
        ~SeratoDatabaseParser();

        #pragma mark Instance Methods
        const std::string* versionAsString(void) const;
    };
}

#endif /* defined(__SeratoDB__NxASeratoDatabaseParser__) */
