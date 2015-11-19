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

#include "Internal/Crate.hpp"
#include "SeratoDB/Database.hpp"

// -- Generated internal implementation ommitted because this class does not use the default contructor.

using namespace NxA::Serato::Internal;

#pragma mark Constructors & Destructors

Crate::Crate(const String& fullName) :
             crateName(String::string()),
             fullCrateName(fullName.pointer()),
             tracksWereModified(true),
             cratesWereModified(false),
             childrenCrates(Serato::Crate::Array::array()),
             volumePaths(String::ArrayOfConst::array()),
             trackEntriesPerPath(Serato::TrackEntry::Array::Array::array()),
             otherTagsPerPath(Serato::Tag::ArrayOfConst::Array::array()) { }

#pragma mark Class Methods

NxA::String::Pointer Crate::crateNameIfValidCrateOrEmptyIfNot(const String& name)
{
    auto result = String::string();

    if (name.hasPrefix("[crate]")) {
        result = name.subString(7);
    }

    return result;
}

NxA::String::Pointer Crate::escapedNameFromCrateName(const String& CrateName)
{
    String::Pointer result = String::stringWith(CrateName);

    result->replaceOccurenceOfStringWith("&", "&&");
    result->replaceOccurenceOfStringWith("/", "&fs");
    result->replaceOccurenceOfStringWith("\\", "&bs");

    return result;
}

NxA::String::Pointer Crate::crateNameFromEscapedName(const String& EscapedName)
{
    String::Pointer result = String::stringWith(EscapedName);

    result->replaceOccurenceOfStringWith("&bs", "\\");
    result->replaceOccurenceOfStringWith("&fs", "/");
    result->replaceOccurenceOfStringWith("&&", "&");

    return result;
}

NxA::String::Pointer Crate::crateNameFromFullCrateName(const String& fullCrateName)
{
    auto lastSeperatorIndex = fullCrateName.indexOfLastOccurenceOf("%%");
    if (lastSeperatorIndex == fullCrateName.length()) {
        return String::stringWith(fullCrateName.pointer());
    }

    return fullCrateName.subString(lastSeperatorIndex + 2);
}

NxA::String::Pointer Crate::topParentCrateNameFromFullCrateName(const String& fullCrateName)
{
    auto firstSeperatorIndex = fullCrateName.indexOfFirstOccurenceOf("%%");
    return fullCrateName.subString(0, firstSeperatorIndex);
}

NxA::String::Pointer Crate::smartCratesDirectoryPathInSeratoFolder(const String& seratoFolderPath)
{
    auto joinedPath = File::joinPaths(seratoFolderPath, String::stringWith("SmartCrates"));
    return joinedPath;
}

NxA::String::Pointer Crate::crateFilePathForFullCrateNameInSeratoFolder(const String& fullCrateName,
                                                                        const String& seratoFolderPath)
{
    NXA_ASSERT_TRUE(fullCrateName.length() != 0);

    auto cratesFolderPath = NxA::Serato::Crate::subCratesDirectoryPathInSeratoFolder(seratoFolderPath);
    auto crateFilePath = File::joinPaths(cratesFolderPath, fullCrateName);
    crateFilePath->append(".crate");

    return crateFilePath;
}

NxA::String::Pointer Crate::crateFilePathForFullSmartCrateNameInSeratoFolder(const String& fullCrateName,
                                                                             const String& seratoFolderPath)
{
    NXA_ASSERT_TRUE(fullCrateName.length() != 0);
    
    auto cratesFolderPath = NxA::Serato::Crate::subCratesDirectoryPathInSeratoFolder(seratoFolderPath);
    auto crateFilePath = File::joinPaths(cratesFolderPath, fullCrateName);
    crateFilePath->append(".scrate");

    return crateFilePath;
}

#pragma mark Instance Methods

NxA::count Crate::indexOfVolumePath(const String& volumePath)
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

NxA::count Crate::indexOfVolumePathAndAddIfNotPresent(const String& volumePath)
{
    count pathIndex = this->indexOfVolumePath(volumePath);
    if (pathIndex == this->volumePaths->length()) {
        this->volumePaths->append(volumePath);
        this->trackEntriesPerPath->append(Serato::TrackEntry::Array::array());
        this->otherTagsPerPath->append(Serato::Tag::ArrayOfConst::array());
    }

    return pathIndex;
}

void Crate::markCratesAsModified(void)
{
    this->cratesWereModified = true;

    if (this->parentCrate.isValid()) {
        auto parent = this->parentCrate.pointer();
        parent->internal->markCratesAsModified();
    }
}

void Crate::saveDataToCrateFileInSeratoFolder(const Blob& data, const String& seratoFolderPath)
{
    try {
        Serato::Database::createSeratoFolderIfDoesNotExists(seratoFolderPath);

        auto cratesFolderPath = NxA::Serato::Crate::subCratesDirectoryPathInSeratoFolder(seratoFolderPath);
        if (!File::directoryExistsAt(cratesFolderPath)) {
            File::createDirectoryAt(cratesFolderPath);
        }

        auto crateFilePath = Internal::Crate::crateFilePathForFullCrateNameInSeratoFolder(this->fullCrateName, seratoFolderPath);
        File::writeBlobToFileAt(data, crateFilePath);
    }
    catch (FileError& e) {
        // -- This should log something.
    }
}
