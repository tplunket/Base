//
//  NxASeratoDbUtility.h
//  SeratoDB
//
//  Created by Didier Malenfant on 2/4/15.
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

#ifndef __SeratoDB__NxASeratoDbUtility__
#define __SeratoDB__NxASeratoDbUtility__

#include <string>
#include <vector>

namespace NxA {
    #pragma mark Containers
    typedef std::auto_ptr<const std::string> StringAutoPtr;
    typedef std::vector<StringAutoPtr> StringVector;
    typedef std::auto_ptr<StringVector> StringVectorAutoPtr;
    typedef std::vector<unsigned char> UnsignedCharVector;
    typedef std::auto_ptr<UnsignedCharVector> UnsignedCharVectorAutoPtr;

    #pragma mark Utility Functions
    StringAutoPtr convertUTF16ToStdString(const char16_t* characters, int numberOfCharacters);

    StringVectorAutoPtr splitStringIntoOneStringForEachLine(const std::string* text);

    uint32_t bigEndianUInt32ValueAt(const void* ptr);
    uint16_t bigEndianUInt16ValueAt(const void* ptr);
}

#endif /* defined(__SeratoDB__NxASeratoDbUtility__) */
