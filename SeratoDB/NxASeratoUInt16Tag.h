//
//  NxASeratoUInt16Tag.h
//  SeratoDB
//
//  Created by Didier Malenfant on 2/27/15.
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

#ifndef __SeratoDB__NxASeratoUInt16Tag__
#define __SeratoDB__NxASeratoUInt16Tag__

#include <SeratoDB/NxASeratoTag.h>
#include <SeratoDB/NxASeratoDbUtility.h>

namespace NxA {
    #pragma mark Class Declaration
    class SeratoUInt16Tag : public SeratoTag
    {
    private:
        #pragma mark Private Instance Variables
        uint16_t p_value;

    public:
        #pragma mark Constructors
        explicit SeratoUInt16Tag(const void* tagAddress) :
                                 SeratoTag(SeratoTag::identifierForTagAt(tagAddress)),
                                 p_value(bigEndianUInt16ValueAt(SeratoTag::p_dataForTagAt(tagAddress))) { }
        explicit SeratoUInt16Tag(const uint32_t& identifier, const uint16_t& value) :
                                 SeratoTag(identifier),
                                 p_value(value) { }

        #pragma mark Instance Methods
        const uint16_t& value(void) const;
        uint16_t& value(void);

        void addTo(CharVector& destination) const;
    };
}

#endif /* defined(__SeratoDB__NxASeratoUInt16Tag__) */
