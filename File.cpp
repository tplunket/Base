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

#include "Base/Blob.hpp"
#include "Base/String.hpp"
#include "Base/MutableBlob.hpp"
#include "Base/MutableString.hpp"
#include "Base/MutableArray.hpp"
#include "Base/Platform.hpp"
#include "Base/Assert.hpp"
#include "Base/File.hpp"

#include <boost/filesystem.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <errno.h>
#include <dirent.h>

using namespace NxA;

#pragma mark mark Class Methods

Blob File::readFileAt(const String& path)
{
    NXA_ASSERT_TRUE(path.length() > 0);

    byte* fileData = nullptr;

    count fileSize = File::sizeOfFileAt(path);
    if (!fileSize) {
        return { };
    }

    fileData = new byte[fileSize];
    NXA_SCOPE_EXIT(fileData) {
        delete[] fileData;
    } NXA_SCOPE_EXIT_END

    if (!fileData) {
        throw FileError::exceptionWith("Error reading file at '%s'.", path.asUTF8());
    }

    std::fstream file(path.asUTF8(), std::ios::in | std::ios::binary);
    file.read(reinterpret_cast<character*>(fileData), fileSize);
    NXA_SCOPE_EXIT(&file) {
        file.close();
    } NXA_SCOPE_EXIT_END

    if (file.rdstate() & std::ifstream::failbit) {
        throw FileError::exceptionWith("Error reading file at '%s'.", path.asUTF8());
    }

    return Blob::blobWithMemoryAndSize(fileData, fileSize);
}

void File::writeBlobToFileAt(const Blob& content, const String& path)
{
    NXA_ASSERT_TRUE(content.size() != 0);

    std::fstream file(path.asUTF8(), std::ios::out | std::ios::binary);
    file.write(reinterpret_cast<const character*>(content.data()), content.size());

    if (file.rdstate() & std::ifstream::failbit) {
        file.close();
        throw FileError::exceptionWith("Error writing to file at '%s'.", path.asUTF8());
    }

    file.close();
}

void File::deleteFileAt(const String& path)
{
    NXA_ASSERT_TRUE(path.length() > 0);

    ::remove(path.asUTF8());
}

String File::joinPaths(const String& first, const String& second)
{
    MutableString result(first);

    if (Platform::CurrentPlatform == Platform::Kind::Windows) {
        if (!result.hasPostfix("\\")) {
            result.append("\\");
        }
    }
    else {
        if (!result.hasPostfix("/")) {
            result.append("/");
        }
    }

    result.append(second);

    return { std::move(result) };
}

String File::removePrefixFromPath(const String& prefix, const String& path)
{
    MutableString fullPrefix(prefix);

    const character* separator;
    if (Platform::CurrentPlatform == Platform::Kind::Windows) {
        separator = "\\";
    }
    else {
        separator = "/";
    }

    if (!fullPrefix.hasPostfix(separator)) {
        fullPrefix.append(separator);
    }

    if (!path.hasPrefix(fullPrefix.asUTF8())) {
        NXA_ALOG("Path '%s' does not have prefix '%s'.", path.asUTF8(), prefix.asUTF8());
    }

    count lengthToCrop = fullPrefix.length();
    return path.subString(lengthToCrop);
}

String File::extensionForFilePath(const String& path)
{
    boost::filesystem::path boostPath(path.asUTF8());
    return String::stringWithUTF8(boost::filesystem::extension(boostPath).c_str());
}

boolean File::fileExistsAt(const String& path)
{
    if(!path.length()) {
        return false;
    }

    try {
        boost::filesystem::path boostPath(path.asUTF8());
        return (boost::filesystem::exists(boostPath) && boost::filesystem::is_regular_file(boostPath));
    } catch (const boost::filesystem::filesystem_error &e) {
        return false;
    }
}

boolean File::directoryExistsAt(const String& path)
{
    if(!path.length()) {
        return false;
    }

    boost::filesystem::path boostPath(path.asUTF8());
    return (boost::filesystem::exists(boostPath) && boost::filesystem::is_directory(boostPath));
}

count File::sizeOfFileAt(const String& path)
{
    if(!path.length()) {
        return 0;
    }

    boost::filesystem::path boostPath(path.asUTF8());
    return (boost::filesystem::file_size(boostPath));
}

void File::createDirectoryAt(const String& path)
{
    NXA_ASSERT_TRUE(path.length() > 0);

    try {
        boost::filesystem::path boostPath(path.asUTF8());
        boost::filesystem::create_directory(boostPath);
    } catch (...) {
        throw FileError::exceptionWith("Error creating directory at '%s'.", path.asUTF8());
    }
}

Array<String> File::pathsForFilesInDirectory(const String& path)
{
    NXA_ASSERT_TRUE(path.length() > 0);

    MutableArray<String> pathsFound;

    if (File::directoryExistsAt(path)) {
        boost::filesystem::path boostPath(path.asUTF8());
        boost::filesystem::directory_iterator end_iterator;

        // cycle through the directory
        for (boost::filesystem::directory_iterator iterator(boostPath); iterator != end_iterator; ++iterator) {
            auto& pathFound = iterator->path();
            if (boost::filesystem::is_regular_file(pathFound)) {
                pathsFound.append(String::stringWithUTF8(pathFound.c_str()));
            }
        }
    }

    return { std::move(pathsFound) };
}

String File::TemporaryDirectory()
{
    boost::system::error_code error;
    auto path = boost::filesystem::temp_directory_path(error);

    if (boost::system::errc::success != error.value()) {
        throw FileError::exceptionWith("Error retrieving temporary path.");
    }

    return String(path.string());
}

timestamp File::modificationDateInSecondsSince1970ForFile(const String& path)
{
    NXA_ASSERT_TRUE(path.length() > 0);

    boost::filesystem::path boostPath(path.asUTF8());
    return (boost::filesystem::last_write_time(boostPath));
}

void File::setModificationDateInSecondsSince1970ForFile(timestamp modificationDateInSeconds, const String& path)
{
    NXA_ASSERT_TRUE(path.length() > 0);

    try {
        boost::filesystem::path boostPath(path.asUTF8());
        if (boost::filesystem::exists(boostPath)) {
            boost::filesystem::last_write_time(boostPath, modificationDateInSeconds);
        }
    } catch (...) {
        throw FileError::exceptionWith("Error setting modification date on '%s'.", path.asUTF8());
    }
}
