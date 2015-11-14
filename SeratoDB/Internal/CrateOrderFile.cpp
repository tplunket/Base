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

#include "Internal/CrateOrderFile.hpp"

#include <string>
#include <errno.h>
#include <dirent.h>

// -- Generated internal implementation omitted because this class does not use the default contructor.

using namespace NxA;
using namespace NxA::Serato::Internal;

#pragma mark Constructors & Destructors

CrateOrderFile::CrateOrderFile(const String& path,
                               Serato::Crate& root) :
                               crateOrderFilePath(path.pointer()),
                               rootCrate(root.pointer()),
                               unknownCratesNames(String::ArrayOfConst::array()) { }

#pragma mark Class Methods

String::Pointer CrateOrderFile::crateNameIfValidCrateOrEmptyIfNot(const String& name)
{
    auto result = String::string();

    if (name.hasPrefix("[crate]")) {
        result = name.subString(7);
    }

    return result;
}

boolean CrateOrderFile::filenameIsAValidCrateName(const String& fileName)
{
    return !fileName.hasPrefix(".") && fileName.hasPostfix(".crate");
}

String::Pointer CrateOrderFile::crateNameFromFilename(const String& fileName)
{
    return fileName.subString(0, fileName.length() - 6);
}

String::ArrayOfConst::Pointer CrateOrderFile::cratesInSubCratesDirectory(const String& directory)
{
    auto crateNamesFound = String::ArrayOfConst::array();

    DIR *pdir;
    struct dirent *pent;

    pdir = opendir(directory.toUTF8()); //"." refers to the current dir
    if (pdir){
        errno = 0;
        while ((pent = readdir(pdir))){
            auto fileName = String::stringWith(pent->d_name);
            if (CrateOrderFile::filenameIsAValidCrateName(fileName)) {
                crateNamesFound->append(CrateOrderFile::crateNameFromFilename(fileName));
            }
        }
    }

    if (!errno){
        closedir(pdir);
    }

    return crateNamesFound;
}

String::ArrayOfConst::Pointer CrateOrderFile::readCratesNamesInCrateOrderFile(const String& crateOrderFilePath)
{
    auto cratesInOrder = String::ArrayOfConst::array();

    auto crateOrderFile = File::readFileAt(crateOrderFilePath);
    if (crateOrderFile->size()) {
        auto textAsString = String::stringWithUTF16(crateOrderFile);
        auto lines = textAsString->splitBySeperator('\n');
        for (auto& crateLine : *lines) {
            auto fullCrateName = CrateOrderFile::crateNameIfValidCrateOrEmptyIfNot(crateLine);
            if (fullCrateName->isEmpty()) {
                continue;
            }

            cratesInOrder->append(fullCrateName);
        }
    }

    return cratesInOrder;
}

void CrateOrderFile::addCratesNamesAtTheStartOfUnlessAlreadyThere(String::ArrayOfConst& cratesToAddTo,
                                                                  const String::ArrayOfConst& cratesToAdd)
{
    count insertionIndex = 0;
    for (auto& crateName : cratesToAdd) {
        boolean alreadyHaveThisCrate = cratesToAddTo.contains(crateName);
        if (alreadyHaveThisCrate) {
            continue;
        }

        cratesToAddTo.insertAt(*crateName, cratesToAddTo.begin() + insertionIndex);

        ++insertionIndex;
    }
}

#pragma mark Instance Methods

void CrateOrderFile::addChildrenCratesOfCrateNamedUsingNameList(Serato::Crate& parentCrate,
                                                                const String& name,
                                                                String::ArrayOfConst::iterator& it,
                                                                const String::ArrayOfConst::iterator& end,
                                                                const String& seratoFolderPath,
                                                                const String& rootFolderPath)
{
    while (it != end) {
        auto fullCrateName = *it;
        if (name.length() && !fullCrateName->hasPrefix(name)) {
            break;
        }

        if (Serato::Crate::isASmartCrateName(fullCrateName, seratoFolderPath)) {
            this->unknownCratesNames->append(fullCrateName);
            ++it;
            continue;
        }

        auto newCrate = parentCrate.findOrAddCrateWithName(fullCrateName);

        if (Serato::Crate::isAValidCrateName(fullCrateName, seratoFolderPath)) {
            newCrate->readFromFolderInVolume(seratoFolderPath, rootFolderPath);
        }

        ++it;

        auto crateNameWithSeperator = String::stringWith(fullCrateName);
        crateNameWithSeperator->append("%%");

        CrateOrderFile::addChildrenCratesOfCrateNamedUsingNameList(newCrate,
                                                                   crateNameWithSeperator,
                                                                   it,
                                                                   end,
                                                                   seratoFolderPath,
                                                                   rootFolderPath);

        newCrate->resetModificationFlags();
    }
}
