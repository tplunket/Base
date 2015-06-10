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

#include <SeratoDB/DbUtility.hpp>

#include <string>

namespace NxA { namespace Serato {
    #pragma mark Forward declarations
    class Tag;

    #pragma mark Containers
    typedef std::unique_ptr<const Tag> ConstTagPtr;
    typedef std::unique_ptr<Tag> TagPtr;
    typedef std::vector<TagPtr> TagVector;
    typedef std::vector<ConstTagPtr> ConstTagVector;
    typedef std::unique_ptr<TagVector> TagVectorPtr;
    typedef std::unique_ptr<ConstTagVector> ConstTagVectorPtr;

    #pragma mark Class Declaration
    class Tag
    {
    private:
        #pragma mark Private Constructors/Destructors
        Tag() { };

    protected:
        #pragma mark Protected Constructors/Destructors
        explicit Tag(const uint32_t& identifier) : p_identifier(identifier) { }
        explicit Tag(const void* tagAddress) : p_identifier(Tag::identifierForTagAt(tagAddress)) { }

        #pragma mark Protected Class Methods
        static void p_setIdentifierForTagAt(const uint32_t& identifier, const void* tagAddress);
        static size_t p_dataSizeInBytesForTagAt(const void* tagAddress);
        static void p_setDataSizeForTagAt(const size_t& dataSizeInBytes, const void* tagAddress);
        static const void* p_dataForTagAt(const void* tagAddress);
        static void* p_dataForTagAt(void* tagAddress);
        static size_t p_memoryNeededWithDataOfSize(const size_t& dataSizeInBytes);

        #pragma mark Protected Instance Variables
        uint32_t p_identifier;

    public:
        #pragma mark Public Constructors/Destructors
        virtual ~Tag() { };

        #pragma mark Class Methods
        static uint32_t identifierForTagAt(const void* tagAddress);
        static const void* nextTagAfterBinaryRepresentationAt(const void* tagAddress);

        #pragma mark Instance Methods
        const uint32_t& identifier(void) const;

        virtual void addTo(CharVector& destination) const = 0;
    };
} }
