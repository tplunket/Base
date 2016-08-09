//
//  Copyright (c) 2015-2016 Next Audio Labs, LLC. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy of
//  this software and associated documentation files (the "Software"), to deal in the
//  Software without restriction, including without limitation the rights to use, copy,
//  modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
//  and to permit persons to whom the Software is furnished to do so, subject to the
//  following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
//  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
//  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#pragma once

// -- This should be included after our own includes but some names clash with its contents.
#include <vector>

#include "Base/Internal/Object.hpp"
#include "Base/Assert.hpp"
#include "Base/Types.hpp"

namespace NxA {

#pragma mark Forward Declarations

class String;
void MurmurHash3_x64_128(const void*, const int, const uint32_t, void*);

#pragma mark Class

struct MutableBlobInternal : public Object::Internal, public std::vector<byte>
{
    #pragma mark Constructors/Destructors
    MutableBlobInternal() : std::vector<byte>() { }
    MutableBlobInternal(const std::vector<byte>& other) : std::vector<byte>{ other } { }
    MutableBlobInternal(std::vector<byte>&& other) : std::vector<byte>{ std::move(other) } { }
    virtual ~MutableBlobInternal() = default;

    #pragma mark Factory Methods
    static std::shared_ptr<MutableBlobInternal> blobWithCapacity(count size)
    {
        return std::make_shared<MutableBlobInternal>(std::vector<byte>(size));
    }
    static std::shared_ptr<MutableBlobInternal> blobWithMemoryAndSize(const byte* other, count size)
    {
        return std::make_shared<MutableBlobInternal>(std::vector<byte>(other, other + size));
    }
    static std::shared_ptr<MutableBlobInternal> blobWithBase64String(const String&);
    static std::shared_ptr<MutableBlobInternal> blobWithStringWithTerminator(const String&);
    static std::shared_ptr<MutableBlobInternal> blobWithStringWithoutTerminator(const String&);

    #pragma mark Class Methods
    static std::shared_ptr<MutableBlobInternal> hashFor(const byte* memory, count size)
    {
        auto result = std::vector<byte>(16);

        MurmurHash3_x64_128(memory, static_cast<int>(size), 0x23232323, result.data());

        return std::make_shared<MutableBlobInternal>(std::move(result));
    }
    static String base64StringFor(const byte* memory, count size);

    #pragma mark Operators
    bool operator==(const MutableBlobInternal& other) const
    {
        if (this->size() != other.size()) {
            return false;
        }

        for (integer index = 0; index < this->size(); ++index) {
            if (this->std::vector<byte>::operator[](index) != other.std::vector<byte>::operator[](index)) {
                return false;
            }
        }

        return true;
    }
    byte& operator[] (integer index)
    {
        NXA_ASSERT_TRUE(index >= 0 && index < this->size());
        return this->std::vector<byte>::operator[](index);
    }

    #pragma mark Instance Methods
    count size() const
    {
        return this->std::vector<byte>::size();
    }

    byte* data()
    {
        // Returns pointer to the underlying array serving as element storage. The pointer is such that range [data(); data() + size()) is always a valid range, even if the container is empty (data() is not dereferenceable in that case).
        return this->std::vector<byte>::data();
    }

    void fillWithZeros()
    {
        std::memset(this->data(), 0, this->size());
    }

    std::shared_ptr<MutableBlobInternal> hash()
    {
        return MutableBlobInternal::hashFor(this->data(), this->size());
    }

    String base64String();

    void append(MutableBlobInternal other)
    {
        this->insert(this->end(), other.begin(), other.end());
    }

    void appendWithStringTermination(const character* other)
    {
        this->insert(this->end(), other, other + ::strlen(other) + 1);
    }

    void appendWithoutStringTermination(const character* other)
    {
        this->insert(this->end(), other, other + ::strlen(other));
    }

    void append(const character other)
    {
        this->insert(this->end(), 1, other);
    }

    void removeAll()
    {
        this->clear();
    }

    void padToAlignment(integer32 alignment)
    {
        count paddingSize = (((this->size() + alignment - 1) / alignment) * alignment) - this->size();
        if (paddingSize > 0) {
            auto padding = std::vector<byte>(paddingSize);
            this->std::vector<byte>::insert(this->end(), padding.begin(), padding.end());
        }
    }

    #pragma mark Overriden Object::Internal Instance Methods
    uinteger32 classHash() const override
    {
        NXA_ALOG("Illegal call.");
        return 0;
    }
    const character* className() const override
    {
        NXA_ALOG("Illegal call.");
        return nullptr;
    }
};

}
