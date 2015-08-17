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

//-----------------------------------------------------------------------------

/*
 This is part of the libb64 project, and has been placed in the public domain.
 For details, see http://sourceforge.net/projects/libb64
 */

    typedef enum
    {
        step_A, step_B, step_C
    } base64_encodestep;

    typedef struct
    {
        base64_encodestep step;
        char result;
        int stepcount;
    } base64_encodestate;

    typedef enum
    {
        step_a, step_b, step_c, step_d
    } base64_decodestep;

    typedef struct
    {
        base64_decodestep step;
        char plainchar;
    } base64_decodestate;

    const int CHARS_PER_LINE = 72;

    void base64_init_encodestate(base64_encodestate* state_in)
    {
        state_in->step = step_A;
        state_in->result = 0;
        state_in->stepcount = 0;
    }

    char base64_encode_value(char value_in)
    {
        static const char* encoding = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        if (value_in > 63) return '=';
        return encoding[(int)value_in];
    }

    int base64_encode_block(const char* plaintext_in, int length_in, char* code_out, base64_encodestate* state_in)
    {
        const char* plainchar = plaintext_in;
        const char* const plaintextend = plaintext_in + length_in;
        char* codechar = code_out;
        char result;
        char fragment;

        result = state_in->result;

        switch (state_in->step)
        {
                while (1)
                {
                case step_A:
                    if (plainchar == plaintextend)
                    {
                        state_in->result = result;
                        state_in->step = step_A;
                        return static_cast<int>(codechar - code_out);
                    }
                    fragment = *plainchar++;
                    result = (fragment & 0x0fc) >> 2;
                    *codechar++ = base64_encode_value(result);
                    result = (fragment & 0x003) << 4;
                case step_B:
                    if (plainchar == plaintextend)
                    {
                        state_in->result = result;
                        state_in->step = step_B;
                        return static_cast<int>(codechar - code_out);
                    }
                    fragment = *plainchar++;
                    result |= (fragment & 0x0f0) >> 4;
                    *codechar++ = base64_encode_value(result);
                    result = (fragment & 0x00f) << 2;
                case step_C:
                    if (plainchar == plaintextend)
                    {
                        state_in->result = result;
                        state_in->step = step_C;
                        return static_cast<int>(codechar - code_out);
                    }
                    fragment = *plainchar++;
                    result |= (fragment & 0x0c0) >> 6;
                    *codechar++ = base64_encode_value(result);
                    result  = (fragment & 0x03f) >> 0;
                    *codechar++ = base64_encode_value(result);

                    ++(state_in->stepcount);
                    if (state_in->stepcount == CHARS_PER_LINE/4)
                    {
                        *codechar++ = '\n';
                        state_in->stepcount = 0;
                    }
                }
        }
        /* control should not reach here */
        return static_cast<int>(codechar - code_out);
    }

    int base64_encode_blockend(char* code_out, base64_encodestate* state_in)
    {
        char* codechar = code_out;

        switch (state_in->step)
        {
            case step_B:
                *codechar++ = base64_encode_value(state_in->result);
                *codechar++ = '=';
                *codechar++ = '=';
                break;
            case step_C:
                *codechar++ = base64_encode_value(state_in->result);
                *codechar++ = '=';
                break;
            case step_A:
                break;
        }
        *codechar++ = '\n';

        return static_cast<int>(codechar - code_out);
    }

    int base64_decode_value(char value_in)
    {
        static const char decoding[] = {62,-1,-1,-1,63,52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-2,-1,-1,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,-1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51};
        static const char decoding_size = sizeof(decoding);
        value_in -= 43;
        if (value_in < 0 || value_in > decoding_size) return -1;
        return decoding[(int)value_in];
    }

    void base64_init_decodestate(base64_decodestate* state_in)
    {
        state_in->step = step_a;
        state_in->plainchar = 0;
    }

    int base64_decode_block(const char* code_in, const int length_in, char* plaintext_out, base64_decodestate* state_in)
    {
        const char* codechar = code_in;
        char* plainchar = plaintext_out;
        char fragment;

        *plainchar = state_in->plainchar;

        // -- Static analyzer trigger a false positive in the code below.
#ifndef __clang_analyzer__
        switch (state_in->step)
        {
                while (1)
                {
                case step_a:
                    do {
                        if (codechar == code_in+length_in)
                        {
                            state_in->step = step_a;
                            state_in->plainchar = *plainchar;
                            return static_cast<int>(plainchar - plaintext_out);
                        }
                        fragment = (char)base64_decode_value(*codechar++);
                    } while (fragment < 0);
                    *plainchar    = (fragment & 0x03f) << 2;
                case step_b:
                    do {
                        if (codechar == code_in+length_in)
                        {
                            state_in->step = step_b;
                            state_in->plainchar = *plainchar;
                            return static_cast<int>(plainchar - plaintext_out);
                        }
                        fragment = (char)base64_decode_value(*codechar++);
                    } while (fragment < 0);
                    *plainchar++ |= (fragment & 0x030) >> 4;
                    *plainchar    = (fragment & 0x00f) << 4;
                case step_c:
                    do {
                        if (codechar == code_in+length_in)
                        {
                            state_in->step = step_c;
                            state_in->plainchar = *plainchar;
                            return static_cast<int>(plainchar - plaintext_out);
                        }
                        fragment = (char)base64_decode_value(*codechar++);
                    } while (fragment < 0);
                    *plainchar++ |= (fragment & 0x03c) >> 2;
                    *plainchar    = (fragment & 0x003) << 6;
                case step_d:
                    do {
                        if (codechar == code_in+length_in)
                        {
                            state_in->step = step_d;
                            state_in->plainchar = *plainchar;
                            return static_cast<int>(plainchar - plaintext_out);
                        }
                        fragment = (char)base64_decode_value(*codechar++);
                    } while (fragment < 0);
                    *plainchar++   |= (fragment & 0x03f);
                }
        }
#endif
        /* control should not reach here */
        return static_cast<int>(plainchar - plaintext_out);
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

Blob::Pointer Blob::blobWithBase64String(const String& str)
{
    base64_decodestate decodeState;
    base64_init_decodestate(&decodeState);

    count lengthIn = str.length();
    char codeOut[lengthIn];
    int codeLength = base64_decode_block(str.toUTF8(), static_cast<int>(str.length()), codeOut, &decodeState);
    NXA_ASSERT_TRUE(codeLength <= sizeof(codeOut));

    auto newBlob = Blob::blobWithMemoryAndSize(reinterpret_cast<byte*>(codeOut), codeLength);
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

String::Pointer Blob::base64StringFor(const byte* memory, count size)
{
    base64_encodestate encodeState;
    base64_init_encodestate(&encodeState);

    char codeOut[size * 2];
    int codeLength = base64_encode_block(reinterpret_cast<const char*>(memory), static_cast<int>(size), codeOut, &encodeState);
    codeLength += base64_encode_blockend(codeOut + codeLength, &encodeState);
    NXA_ASSERT_TRUE(codeLength <= sizeof(codeOut));

    auto result = String::stringWith(codeOut, codeLength);
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

String::Pointer Blob::base64String(void)
{
    return Blob::base64StringFor(this->data(), this->size());
}

void Blob::append(const Blob& other)
{
    this->insert(this->end(), other.begin(), other.end());
}

void Blob::appendWithStringTermination(const character* other)
{
    this->insert(this->end(), other, other + ::strlen(other) + 1);
}

void Blob::appendWithoutStringTermination(const character* other)
{
    this->insert(this->end(), other, other + ::strlen(other));
}

void Blob::append(const character other)
{
    this->insert(this->end(), 1, other);
}
