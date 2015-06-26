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

Blob::Pointer File::readFileAt(const String& path)
{
    count fileSize = File::sizeOfFileAt(path);
    if (fileSize) {
        byte* fileData = new byte[fileSize];
        if (fileData) {
            fstream file(path.toUTF8(), ios::in | ios::binary);
            file.read(reinterpret_cast<char*>(fileData), fileSize);
            file.close();

            auto result = Blob::blobWithMemoryAndSize(fileData, fileSize);
            delete[] fileData;

            return result;
        }
    }

    return Blob::blob();
}

void File::writeBlobToFileAt(const Blob& content, const String& path)
{
    fstream file(path.toUTF8(), ios::out | ios::binary);
    file.write(reinterpret_cast<const char *>(content.data()), content.size());
    file.close();
}

void File::deleteFileAt(const String& path)
{
    ::remove(path.toUTF8());
}

String::Pointer File::joinPaths(const String& first,
                                const String& second)
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

String::Pointer File::removePrefixFromPath(const String& prefix,
                                           const String& path)
{
    count lengthToCrop = prefix.length();
    return path.subString(lengthToCrop);
}

bool File::fileExistsAt(const String& path)
{
    struct stat buf;
    return (::stat(path.toUTF8(), &buf) != -1);
}

NxA::count File::sizeOfFileAt(const String& path)
{
    struct stat buf;
    if (::stat(path.toUTF8(), &buf) == -1) {
        return 0;
    }

    return buf.st_size;
}

timestamp File::modificationDateInSecondsSince1970ForFile(const String& path)
{
    struct stat buf;
    if (::stat(path.toUTF8(), &buf) == -1) {
        return 0;
    }

    return buf.st_mtimespec.tv_sec;
}
