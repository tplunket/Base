//
//  NxASeratoBase64.cpp
//  SeratoDB
//
//  Created by Didier Malenfant on 2/11/15.
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

#include "SeratoDB/NxASeratoBase64.h"

#include <stdlib.h>

using namespace NxA;
using namespace std;

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

static UnsignedCharVectorAutoPtr vectorAutoPtrFromUnsignedCharsBetween(const char* start, const char* end)
{
    UnsignedCharVector* out = new UnsignedCharVector(start, end);
    return UnsignedCharVectorAutoPtr(out);
}

#pragma mark Class Methods

UnsignedCharVectorAutoPtr SeratoBase64::decodeBlock(const char* code_in, size_t length_in)
{
    if (!length_in) {
        return UnsignedCharVectorAutoPtr(NULL);
    }

    const char* codechar = code_in;
    const char* code_in_end = code_in + length_in;

    UnsignedCharVector buffer(length_in);
    char* plaintext_out = (char*)buffer.data();
    char* plaintext_out_end = plaintext_out + length_in;
    char* plainchar = plaintext_out;
    *plainchar = 0;

    char fragment;

    while (plainchar < plaintext_out_end) {
        codechar = base64_decode_loop(codechar, code_in_end, fragment);
        if (!codechar) {
            return vectorAutoPtrFromUnsignedCharsBetween(plaintext_out, plainchar);
        }

        *plainchar = (fragment & 0x03f) << 2;

        codechar = base64_decode_loop(codechar, code_in_end, fragment);
        if (!codechar) {
            return vectorAutoPtrFromUnsignedCharsBetween(plaintext_out, plainchar);
        }

        *plainchar++ |= (fragment & 0x030) >> 4;
        if (plainchar >= plaintext_out_end) {
            break;
        }

        *plainchar = (fragment & 0x00f) << 4;

        codechar = base64_decode_loop(codechar, code_in_end, fragment);
        if (!codechar) {
            return vectorAutoPtrFromUnsignedCharsBetween(plaintext_out, plainchar);
        }

        *plainchar++ |= (fragment & 0x03c) >> 2;
        if (plainchar >= plaintext_out_end) {
            break;
        }

        *plainchar = (fragment & 0x003) << 6;

        codechar = base64_decode_loop(codechar, code_in_end, fragment);
        if (!codechar) {
            return vectorAutoPtrFromUnsignedCharsBetween(plaintext_out, plainchar);
        }

        *plainchar++ |= (fragment & 0x03f);
    }

    return UnsignedCharVectorAutoPtr(NULL);
}
