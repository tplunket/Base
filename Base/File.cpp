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

#include "Base/File.hpp"
#include "Base/Platform.hpp"

#include <sys/stat.h>
#include <iostream>
#include <fstream>

using namespace NxA;
using namespace std;

#pragma mark mark Class Methods

Blob::Pointer File::readFileAt(String::ConstPointer const& path)
{
    count fileSize = File::sizeOfFileAt(path);
    if (fileSize) {
        char* fileData = new char[fileSize];
        if (fileData) {
            fstream file(path->toUTF8(), ios::in | ios::binary);
            file.read(fileData, fileSize);
            file.close();

            auto result = Blob::blobWithMemoryAndSizeInBytes(fileData, fileSize);
            delete[] fileData;

            return result;
        }
    }

    return Blob::blob();
}

void File::writeBlobToFileAt(Blob::ConstPointer const& content, String::ConstPointer const& path)
{
    fstream file(path->toUTF8(), ios::out | ios::binary);
    file.write(reinterpret_cast<const char *>(content->data()), content->size());
    file.close();
}

void File::deleteFileAt(String::ConstPointer const& path)
{
    ::remove(path->toUTF8());
}

String::Pointer File::joinPaths(String::ConstPointer const& first,
                                String::ConstPointer const& second)
{
    auto result = String::stringWith(first);

    if (Platform::platform == Platform::Windows) {
        result->append("\\");
    }
    else {
        result->append("/");
    }

    result->append(second);

    return result;
}

String::Pointer File::removePrefixFromPath(String::ConstPointer const& prefix,
                                           String::ConstPointer const& path)
{
    count lengthToCrop = prefix->length();
    return path->subString(lengthToCrop);
}

bool File::fileExistsAt(String::ConstPointer const& path)
{
    struct stat buf;
    return (::stat(path->toUTF8(), &buf) != -1);
}

NxA::count File::sizeOfFileAt(String::ConstPointer const& path)
{
    struct stat buf;
    if (::stat(path->toUTF8(), &buf) == -1) {
        return 0;
    }

    return buf.st_size;
}

timestamp File::modificationDateInSecondsSince1970ForFile(String::ConstPointer const& path)
{
    struct stat buf;
    if (::stat(path->toUTF8(), &buf) == -1) {
        return 0;
    }

    return buf.st_mtimespec.tv_sec;
}
