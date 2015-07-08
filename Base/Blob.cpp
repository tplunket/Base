//
//  Copyright (c) 2015 Next Audio Labs, LLC. All rights reserved.
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

#pragma mark Murmur3 Methods

namespace NxA {
    //-----------------------------------------------------------------------------
    // MurmurHash3 was written by Austin Appleby, and is placed in the public
    // domain. The author hereby disclaims copyright to this source code.

    //-----------------------------------------------------------------------------
    // Platform-specific functions and macros

#ifdef __GNUC__
#define FORCE_INLINE __attribute__((always_inline)) inline
#else
#define FORCE_INLINE inline
#endif

    static FORCE_INLINE uint64_t rotl64 ( uint64_t x, int8_t r )
    {
        return (x << r) | (x >> (64 - r));
    }

#define	ROTL32(x,y)	rotl32(x,y)
#define ROTL64(x,y)	rotl64(x,y)

#define BIG_CONSTANT(x) (x##LLU)

    //-----------------------------------------------------------------------------
    // Block read - if your platform needs to do endian-swapping or can only
    // handle aligned reads, do the conversion here

#define getblock(p, i) (p[i])

    //-----------------------------------------------------------------------------
    // Finalization mix - force all bits of a hash block to avalanche

    static FORCE_INLINE uint64_t fmix64 ( uint64_t k )
    {
        k ^= k >> 33;
        k *= BIG_CONSTANT(0xff51afd7ed558ccd);
        k ^= k >> 33;
        k *= BIG_CONSTANT(0xc4ceb9fe1a85ec53);
        k ^= k >> 33;

        return k;
    }

    //-----------------------------------------------------------------------------

    void MurmurHash3_x64_128 ( const void * key, const int len,
                              const uint32_t seed, void * out )
    {
        const uint8_t * data = (const uint8_t*)key;
        const int nblocks = len / 16;
        int i;

        uint64_t h1 = seed;
        uint64_t h2 = seed;

        uint64_t c1 = BIG_CONSTANT(0x87c37b91114253d5);
        uint64_t c2 = BIG_CONSTANT(0x4cf5ad432745937f);

        //----------
        // body

        const uint64_t * blocks = (const uint64_t *)(data);

        for(i = 0; i < nblocks; i++)
        {
            uint64_t k1 = getblock(blocks,i*2+0);
            uint64_t k2 = getblock(blocks,i*2+1);

            k1 *= c1; k1  = ROTL64(k1,31); k1 *= c2; h1 ^= k1;

            h1 = ROTL64(h1,27); h1 += h2; h1 = h1*5+0x52dce729;

            k2 *= c2; k2  = ROTL64(k2,33); k2 *= c1; h2 ^= k2;

            h2 = ROTL64(h2,31); h2 += h1; h2 = h2*5+0x38495ab5;
        }

        //----------
        // tail

        const uint8_t * tail = (const uint8_t*)(data + nblocks*16);

        uint64_t k1 = 0;
        uint64_t k2 = 0;

        switch(len & 15)
        {
            case 15: k2 ^= (uint64_t)(tail[14]) << 48;
            case 14: k2 ^= (uint64_t)(tail[13]) << 40;
            case 13: k2 ^= (uint64_t)(tail[12]) << 32;
            case 12: k2 ^= (uint64_t)(tail[11]) << 24;
            case 11: k2 ^= (uint64_t)(tail[10]) << 16;
            case 10: k2 ^= (uint64_t)(tail[ 9]) << 8;
            case  9: k2 ^= (uint64_t)(tail[ 8]) << 0;
                k2 *= c2; k2  = ROTL64(k2,33); k2 *= c1; h2 ^= k2;

            case  8: k1 ^= (uint64_t)(tail[ 7]) << 56;
            case  7: k1 ^= (uint64_t)(tail[ 6]) << 48;
            case  6: k1 ^= (uint64_t)(tail[ 5]) << 40;
            case  5: k1 ^= (uint64_t)(tail[ 4]) << 32;
            case  4: k1 ^= (uint64_t)(tail[ 3]) << 24;
            case  3: k1 ^= (uint64_t)(tail[ 2]) << 16;
            case  2: k1 ^= (uint64_t)(tail[ 1]) << 8;
            case  1: k1 ^= (uint64_t)(tail[ 0]) << 0;
                k1 *= c1; k1  = ROTL64(k1,31); k1 *= c2; h1 ^= k1;
        };

        //----------
        // finalization

        h1 ^= len; h2 ^= len;

        h1 += h2;
        h2 += h1;

        h1 = fmix64(h1);
        h2 = fmix64(h2);

        h1 += h2;
        h2 += h1;
        
        ((uint64_t*)out)[0] = h1;
        ((uint64_t*)out)[1] = h2;
    }
}

//-----------------------------------------------------------------------------

#include "Base/Blob.hpp"

using namespace NxA;

#pragma mark mark Factory Methods

Blob::Pointer Blob::blob(void)
{
    return Blob::makeShared();
}

Blob::Pointer Blob::blobWithCapacity(count size)
{
    auto newBlob = Blob::makeShared();
    newBlob->resize(size);
    newBlob->fillWithZeros();

    return newBlob;
}

Blob::Pointer Blob::blobWithMemoryAndSize(const byte* other, count size)
{
    auto newBlob = Blob::makeShared();
    std::vector<byte>& blobAsVector = *newBlob;
    blobAsVector = std::vector<byte>(other, other + size);

    return newBlob;
}

Blob::Pointer Blob::blobWith(const Blob& other)
{
    auto newBlob = Blob::makeShared();
    std::vector<byte>& blobAsVector = *newBlob;
    const std::vector<byte>& otherBlobAsVector = other;
    blobAsVector = otherBlobAsVector;

    return newBlob;
}

#pragma mark Class Methods

Blob::Pointer Blob::hashFor(const byte* memory, count size)
{
    auto result = NxA::Blob::blobWithCapacity(16);

    MurmurHash3_x64_128(memory, static_cast<int>(size), 0x23232323, result->data());

    return result;
}

#pragma mark Operators

const byte& Blob::operator[] (integer index) const
{
    NXA_ASSERT_TRUE(index >= 0 && index < this->size());
    return this->std::vector<byte>::operator[](index);
}

bool Blob::operator==(const Blob& other) const
{
    if (this == &other) {
        return true;
    }

    const std::vector<byte>& blobAsVector = *this;
    const std::vector<byte>& otherBlobAsVector = other;
    return blobAsVector == otherBlobAsVector;
}

#pragma mark Instance Methods

count Blob::size(void) const
{
    return this->std::vector<byte>::size();
}

const byte* Blob::data(void) const
{
    NXA_ASSERT_TRUE(this->size() > 0);
    return this->std::vector<byte>::data();
}

void Blob::fillWithZeros(void)
{
    std::memset(this->data(), 0, this->size());
}

Blob::Pointer Blob::hash(void)
{
    return Blob::hashFor(this->data(), this->size());
}

void Blob::append(const Blob& other)
{
    this->insert(this->end(), other.begin(), other.end());
}

void Blob::append(const character* other)
{
    this->insert(this->end(), other, other + ::strlen(other) + 1);
}

void Blob::append(const character other)
{
    this->insert(this->end(), 1, other);
}
