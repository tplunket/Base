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

#include <boost/filesystem.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <errno.h>
#include <dirent.h>

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

            if (file.rdstate() & std::ifstream::failbit) {
                file.close();
                throw FileError::exceptionWith("Error reading file at '%s'.", path.toUTF8());
            }
            
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
    NXA_ASSERT_TRUE(content.size() != 0);

    fstream file(path.toUTF8(), ios::out | ios::binary);
    file.write(reinterpret_cast<const char *>(content.data()), content.size());

    if (file.rdstate() & std::ifstream::failbit) {
        file.close();
        throw FileError::exceptionWith("Error writing to file at '%s'.", path.toUTF8());
    }

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
        if (!result->hasPostfix("\\")) {
            result->append("\\");
        }
    }
    else {
        if (!result->hasPostfix("/")) {
            result->append("/");
        }
    }

    result->append(second);

    return result;
}

String::Pointer File::removePrefixFromPath(const String& prefix,
                                           const String& path)
{
    auto fullPrefix = String::stringWith(prefix);

    const character *seperator;
    if (Platform::platform == Platform::Windows) {
        seperator = "\\";
    }
    else {
        seperator = "/";
    }

    if (!fullPrefix->hasPostfix(seperator)) {
        fullPrefix->append(seperator);
    }


    NXA_ASSERT_TRUE(path.hasPrefix(fullPrefix));

    count lengthToCrop = fullPrefix->length();
    return path.subString(lengthToCrop);
}

boolean File::fileExistsAt(const String& path)
{
    boost::filesystem::path boostPath(path.toUTF8());
    return (boost::filesystem::exists(boostPath) && boost::filesystem::is_regular_file(boostPath));
}

boolean File::directoryExistsAt(const String& path)
{
    boost::filesystem::path boostPath(path.toUTF8());
    return (boost::filesystem::exists(boostPath) && boost::filesystem::is_directory(boostPath));
}

NxA::count File::sizeOfFileAt(const String& path)
{
    boost::filesystem::path boostPath(path.toUTF8());
    return (boost::filesystem::file_size(boostPath));
}

void File::createDirectoryAt(const String& path)
{
    try {
        boost::filesystem::path boostPath(path.toUTF8());
        boost::filesystem::create_directory(boostPath);
    } catch (...) {
        throw FileError::exceptionWith("Error creating directory at '%s'.", path.toUTF8());
    }
}

String::Array::Pointer File::pathsForFilesInDirectory(const String& path)
{
    auto pathsFound = String::Array::array();

    boost::filesystem::path boostPath(path.toUTF8());
    boost::filesystem::directory_iterator end_iterator;

    // cycle through the directory
    for (boost::filesystem::directory_iterator iterator(boostPath); iterator != end_iterator; ++iterator) {
        auto& pathFound = iterator->path();
        if (boost::filesystem::is_regular_file(pathFound)){
            pathsFound->append(String::stringWith(pathFound.c_str()));
        }
    }

    return pathsFound;
}

timestamp File::modificationDateInSecondsSince1970ForFile(const String& path)
{
    boost::filesystem::path boostPath(path.toUTF8());
    return (boost::filesystem::last_write_time(boostPath));
}

void File::setModificationDateInSecondsSince1970ForFile(timestamp modificationDateInSeconds, const String& path)
{
    try {
        boost::filesystem::path boostPath(path.toUTF8());
        if (boost::filesystem::exists(boostPath)) {
            boost::filesystem::last_write_time(boostPath, modificationDateInSeconds);
        }
    } catch (...) {
        throw FileError::exceptionWith("Error setting modification date on '%s'.", path.toUTF8());
    }
}
