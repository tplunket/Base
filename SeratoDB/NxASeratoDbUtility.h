//
//  NxASeratoDbUtility.h
//  SeratoDB
//
//  Created by Didier Malenfant on 2/4/15.
//  Copyright (c) 2015 Next Audio Labs, LLC. All rights reserved.
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

    #pragma mark Utility Functions
    const std::string* convertUTF16ToStdString(const char16_t* characters, int numberOfCharacters);

    StringVector* splitStringIntoOneStringForEachLine(const std::string* text);

    uint32_t bigEndianUInt32ValueAt(const void* ptr);
    uint16_t bigEndianUInt16ValueAt(const void* ptr);
}

#endif /* defined(__SeratoDB__NxASeratoDbUtility__) */
