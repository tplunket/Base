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
             crateFullName(fullName.pointer()),
             tracksWereModified(true),
             cratesWereModified(false),
             childrenCrates(Serato::Crate::Array::array()),
             volumePaths(String::ArrayOfConst::array()),
             trackEntriesPerPath(Serato::TrackEntry::Array::Array::array()),
             otherTagsPerPath(Serato::Tag::ArrayOfConst::Array::array()) { }

#pragma mark Class Methods

NxA::String::Pointer Crate::smartCratesDirectoryPathInSeratoFolder(const String& seratoFolderPath)
{
    auto joinedPath = File::joinPaths(seratoFolderPath, String::stringWith("SmartCrates"));
    return joinedPath;
}

NxA::String::Pointer Crate::crateFilePathForCrateNameInSeratoFolder(const String& crateName,
                                                                    const String& seratoFolderPath)
{
    NXA_ASSERT_TRUE(crateName.length() != 0);

    auto cratesFolderPath = NxA::Serato::Crate::subCratesDirectoryPathInSeratoFolder(seratoFolderPath);
    auto crateFilePartialPath = File::joinPaths(cratesFolderPath, crateName);
    crateFilePartialPath->append(".crate");

    return crateFilePartialPath;
}

NxA::String::Pointer Crate::crateFilePathForSmartCrateNameInSeratoFolder(const String& crateName,
                                                                         const String& seratoFolderPath)
{
    NXA_ASSERT_TRUE(crateName.length() != 0);
    
    auto cratesFolderPath = NxA::Serato::Crate::subCratesDirectoryPathInSeratoFolder(seratoFolderPath);
    auto crateFilePartialPath = File::joinPaths(cratesFolderPath, crateName);
    crateFilePartialPath->append(".scrate");

    return crateFilePartialPath;
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

    printf("Index of NEW path '%s' is %ld\n", volumePath.toUTF8(), numberOfPaths);

    this->volumePaths->append(volumePath);
    this->trackEntriesPerPath->append(Serato::TrackEntry::Array::array());
    this->otherTagsPerPath->append(Serato::Tag::ArrayOfConst::array());

    return numberOfPaths;
}

void Crate::markCratesAsModified(void)
{
    this->cratesWereModified = true;

    if (this->parentCrate.isValid()) {
        auto parent = this->parentCrate.pointer();
        parent->internal->markCratesAsModified();
    }
}
