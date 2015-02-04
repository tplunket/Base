//
//  NxASeratoTag.h
//  SeratoDB
//
//  Created by Didier Malenfant on 2/3/15.
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

#ifndef __SeratoDB__NxASeratoTag__
#define __SeratoDB__NxASeratoTag__

#include <string>
#include <map>
#include <vector>

namespace NxA {
    #pragma mark Forward declarations
    class SeratoTag;

    #pragma mark Containers
    typedef std::map<uint32_t, SeratoTag*> SeratoIdentifierToTagMap;
    typedef std::vector<SeratoTag*> SeratoTagVector;
    typedef std::vector<unsigned char> SeratoTagData;

    #pragma mark Class Declaration
    class SeratoTag
    {
    private:
        #pragma mark Private Instance Variables
        uint32_t p_identifier;

        size_t p_dataSizeInBytes;

        SeratoTagData* p_data;

        const SeratoTag* p_parentTag;

        SeratoIdentifierToTagMap* p_childrenTagsByIdentifier;

    public:
        #pragma mark Constructors
        SeratoTag(const void* tagAddress);

        #pragma mark Destructor
        virtual ~SeratoTag();
        
        SeratoTag(const void* tagAddress, SeratoTag* parentTag) :
            SeratoTag(tagAddress) {
                this->p_parentTag = parentTag;
            };

        #pragma mark Class Methods
        static SeratoTagVector* parseTagsIn(const void* firstTagAddress, size_t sizeFromFirstTagInBytes);

        static void deleteTagsIn(SeratoTagVector* tags);

        #pragma mark Instance Methods
        SeratoTag* subTagWithIdentifier(uint32_t identifier) const;

        uint32_t identifier(void) const;

        bool dataAsBoolean(void) const;
        uint16_t dataAsUInt16(void) const;
        uint32_t dataAsUInt32(void) const;
        const std::string* dataAsString(void) const;
        const std::string* dataAsPath(void) const;
        const void* data(void) const;

        size_t dataSizeInBytes(void) const;
    };
}

#endif /* defined(__SeratoDB__NxASeratoTag__) */
