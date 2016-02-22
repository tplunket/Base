//
//  Copyright (c) 2015-2016 Next Audio Labs, LLC. All rights reserved.
//
//  This file contains confidential and proprietary information of Serato
//  Inc. LLP ("Serato"). No use is permitted without express written
//  permission of Serato. If you are not a party to a Confidentiality/
//  Non-Disclosure Agreement with Serato, please immediately delete this
//  file as well as all copies in your possession. For further information,
//  please refer to the modified MIT license provided with this library,
//  or email licensing@serato.com.
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

#include "Internal/InternalCrate.hpp"
#include "SeratoDB/Database.hpp"

// -- Generated internal implementation ommitted because this class does not use the default contructor.

using namespace NxA::Serato;

#pragma mark Constructors & Destructors

InternalCrate::InternalCrate(const String& crateName) :
    name(crateName.pointer()),
    tracksWereModified(true),
    cratesWereModified(false),
    childrenCrates(Crate::Array::array()),
    volumePaths(String::ArrayOfConst::array()),
    trackEntriesPerPath(TrackEntry::Array::Array::array()),
    otherTagsPerPath(Tag::ArrayOfConst::Array::array())
{
}

#pragma mark Class Methods

NxA::String::Pointer InternalCrate::crateNameIfValidCrateOrEmptyIfNot(const String& name)
{
    if (name.hasPrefix("[crate]")) {
        return name.subString(7);
    }
    else {
        return String::string();
    }
}

NxA::String::Pointer InternalCrate::escapedNameFromCrateName(const String& crateName)
{
    String::Pointer result = String::stringWith(crateName);

    result->replaceOccurenceOfStringWith("?", "??");
    result->replaceOccurenceOfStringWith("%", "?pc");
    result->replaceOccurenceOfStringWith("/", "?fs");
    result->replaceOccurenceOfStringWith("\\", "?bs");

    return result;
}

NxA::String::Pointer InternalCrate::crateNameFromEscapedName(const String& escapedName)
{
    String::Pointer result = String::stringWith(escapedName);

    result->replaceOccurenceOfStringWith("?bs", "\\");
    result->replaceOccurenceOfStringWith("?fs", "/");
    result->replaceOccurenceOfStringWith("?pc", "%");
    result->replaceOccurenceOfStringWith("??", "?");

    return result;
}

NxA::String::Pointer InternalCrate::topParentCrateNameFromFullCrateName(const String& fullCrateName)
{
    auto firstSeperatorIndex = fullCrateName.indexOfFirstOccurenceOf("%%");
    return fullCrateName.subString(0, firstSeperatorIndex);
}

NxA::String::Pointer InternalCrate::smartCratesDirectoryPathInSeratoFolder(const String& seratoFolderPath)
{
    auto joinedPath = File::joinPaths(seratoFolderPath, String::stringWith("SmartCrates"));
    return joinedPath;
}

NxA::String::Pointer InternalCrate::crateFilePathForFullCrateNameInSeratoFolder(const String& fullCrateName,
                                                                                const String& seratoFolderPath)
{
    NXA_ASSERT_TRUE(fullCrateName.length() != 0);

    auto cratesFolderPath = Crate::subCratesDirectoryPathInSeratoFolder(seratoFolderPath);
    auto crateFilePath = File::joinPaths(cratesFolderPath, fullCrateName);
    crateFilePath->append(".crate");

    return crateFilePath;
}

NxA::String::Pointer InternalCrate::crateFilePathForFullSmartCrateNameInSeratoFolder(const String& fullCrateName,
                                                                                     const String& seratoFolderPath)
{
    NXA_ASSERT_TRUE(fullCrateName.length() != 0);
    
    auto cratesFolderPath = smartCratesDirectoryPathInSeratoFolder(seratoFolderPath);
    auto crateFilePath = File::joinPaths(cratesFolderPath, fullCrateName);
    crateFilePath->append(".scrate");

    return crateFilePath;
}

NxA::String::Pointer InternalCrate::fullCrateNameFromFilename(const String& fileName)
{
    return fileName.subString(0, fileName.length() - 6);
}

#pragma mark Instance Methods

NxA::count InternalCrate::indexOfVolumePath(const String& volumePath)
{
    NXA_ASSERT_TRUE(volumePath.length() != 0);
    
    count numberOfPaths = this->volumePaths->length();
    for (count pathIndex = 0; pathIndex < numberOfPaths; ++pathIndex) {
        auto& path = (*this->volumePaths)[pathIndex];
        if (volumePath == path) {
            return pathIndex;
        }
    }

    return numberOfPaths;
}

NxA::count InternalCrate::indexOfVolumePathAndAddIfNotPresent(const String& volumePath)
{
    count pathIndex = this->indexOfVolumePath(volumePath);
    if (pathIndex == this->volumePaths->length()) {
        this->volumePaths->append(volumePath);
        this->trackEntriesPerPath->append(TrackEntry::Array::array());
        this->otherTagsPerPath->append(Tag::ArrayOfConst::array());
    }

    return pathIndex;
}

void InternalCrate::markCratesAsModified(void)
{
    this->cratesWereModified = true;

    if (this->parentCrate.isValid()) {
        auto parent = this->parentCrate.pointer();
        parent->internal->markCratesAsModified();
    }
}

void InternalCrate::saveDataToCrateFileInSeratoFolder(const Blob& data,
                                                      const String& seratoFolderPath,
                                                      const String& fullCrateName)
{
    try {
        Database::createSeratoFolderIfDoesNotExists(seratoFolderPath);

        auto cratesFolderPath = Crate::subCratesDirectoryPathInSeratoFolder(seratoFolderPath);
        if (!File::directoryExistsAt(cratesFolderPath)) {
            File::createDirectoryAt(cratesFolderPath);
        }

        auto crateFilePath = InternalCrate::crateFilePathForFullCrateNameInSeratoFolder(fullCrateName, seratoFolderPath);
        File::writeBlobToFileAt(data, crateFilePath);
    }
    catch (FileError& e) {
        // -- This should log something.
    }
}
