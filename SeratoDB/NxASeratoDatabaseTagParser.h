//
//  NxASeratoDatabaseTagParser.h
//  SeratoDB
//
//  Created by Didier Malenfant on 1/31/15.
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

#ifndef __SeratoDB__NxASeratoDatabaseTagParser__
#define __SeratoDB__NxASeratoDatabaseTagParser__

#include <stdio.h>
#include <string>

#pragma mark Class Declaration

namespace NxA {
    class SeratoDatabaseTagParser
    {
    private:
        #pragma mark Private Instance Variables
        const void* p_currentTagAddress;

        const void* p_endOfTagsAddress;

        size_t p_sizeOfDataInBytes() const;

    public:
        #pragma mark Constructors
        SeratoDatabaseTagParser(const void* firstTagAddress, size_t sizeOfDataFromFirstTagInBytes) :
                                p_currentTagAddress(firstTagAddress),
                                p_endOfTagsAddress((unsigned char*)firstTagAddress + sizeOfDataFromFirstTagInBytes) { };
        SeratoDatabaseTagParser(const SeratoDatabaseTagParser* tag) :
                                p_currentTagAddress(tag->p_currentTagAddress),
                                p_endOfTagsAddress(tag->p_endOfTagsAddress) { };

        #pragma mark Instance Methods
        bool hasParsedAllTags(void) const;

        void goToTagWithIdentifier(uint32_t identifier);

        void goToNextTag(void);

        void goToDataAsTag(void);

        void goToEnd(void);
        
        const void* address(void) const;

        uint32_t identifier(void) const;

        const std::string* dataAsString(void) const;

        bool dataAsBoolean(void) const;

        uint16_t dataAsUInt16(void) const;

        uint32_t dataAsUInt32(void) const;

        const std::string* dataAsPath(void) const;

        const void* dataAsBlob(void) const;
    };
}

#endif /* defined(__SeratoDB__NxASeratoDatabaseTagParser__) */
