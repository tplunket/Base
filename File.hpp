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

#pragma once

#include <Base/Types.hpp>
#include <Base/Exception.hpp>
#include <Base/Uncopyable.hpp>
#include <Base/Array.hpp>

namespace NxA {

class String;
class Blob;

NXA_EXCEPTION_NAMED_WITH_PARENT(FileError, Exception);

class File : private Uncopyable {
public:
    #pragma mark Constructors & Destructors
    File() = delete;

    #pragma mark Class Methods
    static Blob readFileAt(const String&);
    static void writeBlobToFileAt(const Blob&, const String&);
    static void deleteFileAt(const String&);

    static String joinPaths(const String&, String);
    static String removePrefixFromPath(const String&, const String&);
    static String extensionForFilePath(const String&);

    static boolean fileExistsAt(const String&);
    static boolean directoryExistsAt(const String&);
    static count sizeOfFileAt(const String&);

    static void createDirectoryAt(const String&);
    static Array<String> pathsForFilesInDirectory(const String&);

    static String TemporaryDirectory();
    
    static timestamp modificationDateInSecondsSince1970ForFile(const String&);
    static void setModificationDateInSecondsSince1970ForFile(timestamp, const String&);
};

}
