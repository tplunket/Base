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

#include "Base64.hpp"

#include <stdlib.h>

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Utility Functions

static int base64_decode_value(char value_in)
{
    static const char decoding[] = { 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -2, -1,
                                     -1, -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17,
                                     18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28, 29, 30, 31,
                                     32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51 };
    static const char decoding_size = sizeof(decoding);

    value_in -= 43;

    if (value_in < 0 || value_in >= decoding_size) {
        return -1;
    }

    return decoding[(int)value_in];
}

static const char* base64_decode_loop(const char* codechar, const char* code_in_end, char& fragment)
{
    do {
        if (codechar == code_in_end) {
            return NULL;
        }

        fragment = (char)base64_decode_value(*codechar++);
    }
    while (fragment < 0);

    return codechar;
}

static const char* base64_encoding = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static inline char base64_encode_value(char value_in)
{
    if (value_in > 63) {
        return '=';
    }
    return base64_encoding[(int)value_in];
}

#pragma mark Class Methods

Blob::Pointer Base64::decodeBlock(const void* code_in, count length_in)
{
    if (length_in) {
        const char* codechar = reinterpret_cast<const char*>(code_in);
        const char* code_in_end = reinterpret_cast<const char*>(code_in) + length_in;

        auto buffer = Blob::blobWithCapacity(length_in);
        char* plaintext_out = reinterpret_cast<char*>(buffer->data());
        char* plaintext_out_end = plaintext_out + length_in;
        char* plainchar = plaintext_out;
        char fragment;

        *plainchar = 0;

        while (plainchar < plaintext_out_end) {
            codechar = base64_decode_loop(codechar, code_in_end, fragment);
            if (!codechar) {
                return Blob::blobWithCharPointer(plaintext_out, plainchar - plaintext_out);
            }

            *plainchar = (fragment & 0x03f) << 2;

            codechar = base64_decode_loop(codechar, code_in_end, fragment);
            if (!codechar) {
                return Blob::blobWithCharPointer(plaintext_out, plainchar - plaintext_out);
            }

            *plainchar++ |= (fragment & 0x030) >> 4;
            if (plainchar >= plaintext_out_end) {
                break;
            }

            *plainchar = (fragment & 0x00f) << 4;

            codechar = base64_decode_loop(codechar, code_in_end, fragment);
            if (!codechar) {
                return Blob::blobWithCharPointer(plaintext_out, plainchar - plaintext_out);
            }

            *plainchar++ |= (fragment & 0x03c) >> 2;
            if (plainchar >= plaintext_out_end) {
                break;
            }

            *plainchar = (fragment & 0x003) << 6;
            
            codechar = base64_decode_loop(codechar, code_in_end, fragment);
            if (!codechar) {
                return Blob::blobWithCharPointer(plaintext_out, plainchar - plaintext_out);
            }
            
            *plainchar++ |= (fragment & 0x03f);
        }
    }

    return Blob::blob();
}

Blob::Pointer Base64::encodeBlock(const void* plaintext_in, count length_in)
{
    const char* plainchar = reinterpret_cast<const char*>(plaintext_in);
    const char* const plaintextend = reinterpret_cast<const char*>(plaintext_in) + length_in;
    char* code_out = (char*)malloc(length_in * 2);
    char* code_out_end = code_out + (length_in * 2);
    char* codechar = code_out;
    char charToOutput = 0;
    char fragment;
    auto result = Blob::blob();

    while (1) {
        if (plainchar == plaintextend) {
            *codechar++ = '\n';
            result = Blob::blobWithCharPointer(code_out, codechar - code_out);
            break;
        }

        fragment = *plainchar++;
        charToOutput = (fragment & 0x0fc) >> 2;
        *codechar++ = base64_encode_value(charToOutput);
        charToOutput = (fragment & 0x003) << 4;
        if (codechar >= code_out_end) {
            break;
        }

        if (plainchar == plaintextend) {
            if ((codechar + 4) >= code_out_end) {
                break;
            }

            *codechar++ = base64_encode_value(charToOutput);
            *codechar++ = '=';
            *codechar++ = '=';
            *codechar++ = '\n';

            result = Blob::blobWithCharPointer(code_out, codechar - code_out);
            break;
        }

        fragment = *plainchar++;
        charToOutput |= (fragment & 0x0f0) >> 4;
        *codechar++ = base64_encode_value(charToOutput);
        charToOutput = (fragment & 0x00f) << 2;
        if (codechar >= code_out_end) {
            break;
        }

        if (plainchar == plaintextend) {
            if ((codechar + 3) >= code_out_end) {
                break;
            }

            *codechar++ = base64_encode_value(charToOutput);
            *codechar++ = '=';
            *codechar++ = '\n';

            result = Blob::blobWithCharPointer(code_out, codechar - code_out);
            break;
        }

        fragment = *plainchar++;
        charToOutput |= (fragment & 0x0c0) >> 6;
        *codechar++ = base64_encode_value(charToOutput);
        charToOutput  = (fragment & 0x03f) >> 0;
        if (codechar >= code_out_end) {
            break;
        }

        *codechar++ = base64_encode_value(charToOutput);
        if (codechar >= code_out_end) {
            break;
        }
    }

    free(code_out);

    return result;
}
