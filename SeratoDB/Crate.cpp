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

#include "SeratoDB/Database.hpp"
#include "SeratoDB/Crate.hpp"
#include "Internal/Crate.hpp"
#include "Tags/CrateV1Tags.hpp"
#include "Tags/VersionTag.hpp"
#include "Tags/TagFactory.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, Crate, Object);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Constants

static const character* crateFileCurrentVersionString = "1.0/Serato ScratchLive Crate";

#pragma mark Factory Methods

Crate::Pointer Crate::crateWithFullName(const String& fullCrateName)
{
    auto internalObject = Internal::Crate::Pointer(std::make_shared<Internal::Crate>(fullCrateName));
    auto newCrate = Crate::makeSharedWithInternal(NxA::Internal::Object::Pointer::dynamicCastFrom(internalObject));

    newCrate->internal->crateName = Internal::Crate::crateNameFromFullCrateName(fullCrateName);
    Internal::Crate::crateNameFromEscapedName(newCrate->internal->crateName);

    return newCrate;
}

#pragma mark Class Methods

String::Pointer Crate::subCratesDirectoryPathInSeratoFolder(const String& seratoFolderPath)
{
    auto joinedPath = File::joinPaths(seratoFolderPath, String::stringWith("Subcrates"));
    return joinedPath;
}

String::ArrayOfConst::Pointer Crate::readCratesNamesInCrateOrderFile(const String& crateOrderFilePath)
{
    auto cratesInOrder = String::ArrayOfConst::array();

    try {
        if (File::fileExistsAt(crateOrderFilePath)) {
            auto crateOrderFile = File::readFileAt(crateOrderFilePath);
            if (crateOrderFile->size()) {
                auto textAsString = String::stringWithUTF16(crateOrderFile);
                auto lines = textAsString->splitBySeperator('\n');
                for (auto& crateLine : *lines) {
                    auto fullCrateName = Internal::Crate::crateNameIfValidCrateOrEmptyIfNot(crateLine);
                    if (!fullCrateName->isEmpty()) {
                        Internal::Crate::crateNameFromEscapedName(fullCrateName);
                        cratesInOrder->append(Internal::Crate::crateNameFromEscapedName(fullCrateName));
                    }
                }
            }
        }
    }
    catch (FileError::Exception& e) {
        // -- This should log something.
    }

    return cratesInOrder;
}

NxA::boolean Crate::filenameIsAValidCrateName(const String& fileName)
{
    return !fileName.hasPrefix(".") && fileName.hasPostfix(".crate");
}

NxA::String::Pointer Crate::crateNameFromFilename(const String& fileName)
{
    return fileName.subString(0, fileName.length() - 6);
}

NxA::String::ArrayOfConst::Pointer Crate::cratesInSubCratesDirectory(const String& directory)
{
    auto cratePathsFound = File::pathsForFilesInDirectory(directory);
    auto crateNamesFound = String::ArrayOfConst::array();

    for (auto& path : *cratePathsFound) {
        auto fileName = File::removePrefixFromPath(directory, path);
        if (Crate::filenameIsAValidCrateName(fileName)) {
            auto escapedCrateName = Crate::crateNameFromFilename(fileName);
            crateNamesFound->append(Internal::Crate::crateNameFromEscapedName(escapedCrateName));
        }
    }

    return crateNamesFound;
}

boolean Crate::isAnExistingFullCrateName(const String& fullCrateName, const String& seratoFolderPath)
{
    auto crateFilePath = Internal::Crate::crateFilePathForFullCrateNameInSeratoFolder(fullCrateName, seratoFolderPath);
    return File::fileExistsAt(crateFilePath);
}

boolean Crate::isAnExistingFullSmartCrateName(const String& fullCrateName, const String& seratoFolderPath)
{
    auto crateFilePath = Internal::Crate::crateFilePathForFullSmartCrateNameInSeratoFolder(fullCrateName, seratoFolderPath);
    return File::fileExistsAt(crateFilePath);
}

void Crate::parseCratesInSeratoFolderOnVolumeAddToCrateAndSaveUnknownCrateNamesIn(String::ArrayOfConst& cratesInOrder,
                                                                                  const String& seratoFolderPath,
                                                                                  const String& volumePath,
                                                                                  Serato::Crate& parentCrate,
                                                                                  String::ArrayOfConst& unknownCratesNames)
{
    for (auto fullEntryName : cratesInOrder) {
        if (Serato::Crate::isAnExistingFullSmartCrateName(fullEntryName, seratoFolderPath) ||
            !Serato::Crate::isAnExistingFullCrateName(fullEntryName, seratoFolderPath)) {
            unknownCratesNames.append(fullEntryName);
            continue;
        }

        auto newCrate = parentCrate.findOrAddCrateWithRelativeNameAndFullName(fullEntryName, fullEntryName);
        newCrate->readFromFolderInVolume(seratoFolderPath, volumePath);
    }
}

#pragma mark Instance Methods

const String& Crate::crateName(void) const
{
    return internal->crateName;
}

const String& Crate::fullCrateName(void) const
{
    return internal->fullCrateName;
}

void Crate::addFullCrateNameWithPrefixForCratesOnVolumeAndRecurseToChildren(String& destination, const char* prefix, const String& volumePath) const
{
    if (internal->fullCrateName->length()) {
        count volumePathIndex = internal->indexOfVolumePath(volumePath);
        if (volumePathIndex != internal->volumePaths->length()) {
            destination.append(prefix);
            destination.append(internal->fullCrateName);
            destination.append("\n");
        }
    }

    for (auto& crate : *internal->childrenCrates) {
        crate->addFullCrateNameWithPrefixForCratesOnVolumeAndRecurseToChildren(destination, prefix, volumePath);
    }
}

TrackEntry::Array::Pointer Crate::trackEntries(void) const
{
    auto allEntries = TrackEntry::Array::array();

    for (auto& entries : *(internal->trackEntriesPerPath)) {
        allEntries->append(entries);
    }

    return allEntries;
}

const Crate::Array& Crate::crates(void) const
{
    return *(internal->childrenCrates);
}

void Crate::addCrate(Crate& crate)
{
    NXA_ASSERT_FALSE(crate.hasParentCrate());

    crate.internal->parentCrate = Crate::WeakPointer(this->pointer());
    internal->childrenCrates->append(crate);

    internal->markCratesAsModified();
}

void Crate::removeCrate(Crate& crate)
{
    NXA_ASSERT_TRUE(crate.hasParentCrate() && &(crate.parentCrate()) == this);

    crate.internal->parentCrate.release();
    internal->childrenCrates->remove(crate);

    internal->markCratesAsModified();
}

Crate::Pointer Crate::findOrAddCrateWithRelativeNameAndFullName(const String& relativeName, const String& fullCrateName)
{
    auto topCrateName = Internal::Crate::topParentCrateNameFromFullCrateName(relativeName);

    Crate::NullablePointer crateFound;

    for (auto& crate : crates()) {
        if (crate->crateName() == topCrateName) {
            crateFound = crate;
            break;
        }
    }

    if (crateFound.isNull()) {
        if (this->hasParentCrate()) {
            auto newCrateFullName = String::stringWithFormat("%s%%%%%s",
                                                             this->fullCrateName().toUTF8(),
                                                             topCrateName->toUTF8());
            crateFound = Serato::Crate::crateWithFullName(newCrateFullName);
        }
        else {
            crateFound = Serato::Crate::crateWithFullName(topCrateName);
        }

        this->addCrate(crateFound);
    }

    if (topCrateName == relativeName) {
        return crateFound->pointer();
    }

    return crateFound->findOrAddCrateWithRelativeNameAndFullName(relativeName.subString(topCrateName->length() + 2), fullCrateName);
}

void Crate::addTrackEntry(Serato::TrackEntry& trackEntry)
{
    if (trackEntry.hasParentCrate()) {
        NXA_ASSERT_TRUE(&trackEntry.parentCrate() == this);
        return;
    }

    trackEntry.setParentCrate(*this);

    count volumePathIndex = internal->indexOfVolumePathAndAddIfNotPresent(trackEntry.volumePath());
    (*internal->trackEntriesPerPath)[volumePathIndex].append(trackEntry);

    internal->tracksWereModified = true;
}

void Crate::removeTrackEntry(TrackEntry& trackEntry)
{
    count volumePathIndex = internal->indexOfVolumePath(trackEntry.volumePath());
    NXA_ASSERT_TRUE(volumePathIndex != internal->volumePaths->length());

    auto& trackEntries = (*internal->trackEntriesPerPath)[volumePathIndex];

    auto position = trackEntries.find(trackEntry);
    if (position != trackEntries.end()) {
        trackEntries.removeObjectAt(position);
        internal->tracksWereModified = true;
    }

    if (trackEntry.hasParentCrate()) {
        NXA_ASSERT_TRUE(&trackEntry.parentCrate() == this);
        trackEntry.removeFromParentCrate();
    }
}

void Crate::readFromFolderInVolume(const String& seratoFolderPath, const String& volumePath)
{
    auto otherTags = Serato::Tag::ArrayOfConst::array();
    auto trackEntries = Serato::TrackEntry::Array::array();

    try {
        auto filePath = Internal::Crate::crateFilePathForFullCrateNameInSeratoFolder(this->fullCrateName(), seratoFolderPath);

        auto crateFileData = File::readFileAt(filePath);
        auto tags = TagFactory::parseTagsAt(crateFileData->data(), crateFileData->size());
        if (!tags->length()) {
            return;
        }

        for (auto& tag : *(tags)) {
            switch (tag->identifier()) {
                case crateVersionTagIdentifier: {
                    auto& versionTag = dynamic_cast<VersionTag&>(*tag);
                    if (versionTag.value() != crateFileCurrentVersionString) {
                        throw Serato::DatabaseError::exceptionWith("Illegal crate file version for file '%s'.", filePath->toUTF8());
                    }
                    break;
                }
                case trackEntryTagIdentifier: {
                    auto entry = Serato::TrackEntry::entryWithTagOnVolume(dynamic_cast<ObjectTag&>(*tag), volumePath);
                    entry->setParentCrate(*this);
                    trackEntries->append(entry);
                    break;
                }
                default: {
                    otherTags->append(tag);
                    break;
                }
            }
        }
    }
    catch (DatabaseError& e) {
        // -- This should be logged.
    }
    catch (FileError& e) {
        // -- This should be logged.
    }

    internal->volumePaths->append(volumePath);
    internal->trackEntriesPerPath->append(trackEntries);
    internal->otherTagsPerPath->append(otherTags);
}

boolean Crate::hasParentCrate(void) const
{
    return internal->parentCrate.isValid();
}

Crate& Crate::parentCrate(void)
{
    return *(internal->parentCrate.pointer());
}

void Crate::removeFromParentCrate(void)
{
    this->parentCrate().removeCrate(*this);
}

void Crate::resetModificationFlags()
{
    internal->cratesWereModified = false;
    internal->tracksWereModified = false;
}

boolean Crate::childrenCratesWereModified(void) const
{
    return internal->cratesWereModified;
}

void Crate::saveIfOnVolumeAndRecurseToChildren(const String& volumePath, const String& seratoFolderPath) const
{
    count numberOfPaths = internal->volumePaths->length();

    for (count pathIndex = 0; pathIndex < numberOfPaths; ++pathIndex) {
        auto& path = (*internal->volumePaths)[pathIndex];
        if (path == volumePath) {
            auto& trackEntries = (*internal->trackEntriesPerPath)[pathIndex];
            if (!trackEntries.length()) {
                // -- Serato deletes empty crates anyway so we don't even save them.
                break;
            }

            auto outputData = Blob::blob();

            auto versionTag = VersionTag::tagWithIdentifierAndValue(crateVersionTagIdentifier,
                                                                    String::stringWith(crateFileCurrentVersionString));
            versionTag->addTo(outputData);

            for (auto& trackEntry : trackEntries) {
                trackEntry->tagForEntry().addTo(outputData);
            }

            auto& otherTags = (*internal->otherTagsPerPath)[pathIndex];
            for (auto& tag : otherTags) {
                tag->addTo(outputData);
            }

            internal->saveDataToCrateFileInSeratoFolder(outputData, seratoFolderPath);
            break;
        }
    }

    for (auto& crate : *internal->childrenCrates) {
        crate->saveIfOnVolumeAndRecurseToChildren(volumePath, seratoFolderPath);
    }
}

TrackEntry::Array::Pointer Crate::removeAndReturnTrackEntries(void)
{
    auto result = TrackEntry::Array::arrayWith(this->trackEntries());

    for (auto& entry : *result) {
        entry->removeFromParentCrate();
    }

    internal->tracksWereModified = true;

    return result;
}

Crate::Array::Pointer Crate::removeAndReturnChildrenCrates(void)
{
    auto result = Crate::Array::arrayWith(this->crates());

    for (auto& crate : *result) {
        crate->removeFromParentCrate();
    }

    internal->markCratesAsModified();

    return result;
}
