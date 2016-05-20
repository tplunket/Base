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

#include "SeratoDB/Database.hpp"
#include "SeratoDB/Exceptions.hpp"
#include "SeratoDB/Crate.hpp"
#include "Internal/InternalCrate.hpp"
#include "Internal/InternalTrackEntry.hpp"
#include "Tags/CrateV1Tags.hpp"
#include "Tags/VersionTag.hpp"
#include "Tags/TagFactory.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, Crate, Object);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Constants

constexpr const character* crateFileCurrentVersionString = "1.0/Serato ScratchLive Crate";

#pragma mark Factory Methods

Crate::Pointer Crate::crateWithName(const String& crateName)
{
    auto internalObject = InternalCrate::Pointer(std::make_shared<InternalCrate>(crateName));
    auto newCrate = Crate::makeSharedWithInternal(InternalObject::Pointer::dynamicCastFrom(internalObject));

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
                    auto crateName = InternalCrate::crateNameIfValidCrateOrEmptyIfNot(crateLine);
                    if (!crateName->isEmpty()) {
                        cratesInOrder->append(crateName);
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

NxA::String::ArrayOfConst::Pointer Crate::cratesInSubCratesDirectory(const String& directory)
{
    auto cratePathsFound = File::pathsForFilesInDirectory(directory);
    auto crateNamesFound = String::ArrayOfConst::array();

    for (auto& path : *cratePathsFound) {
        auto fileName = File::removePrefixFromPath(directory, path);
        if (Crate::filenameIsAValidCrateName(fileName)) {
            auto escapedCrateName = InternalCrate::fullCrateNameFromFilename(fileName);
            crateNamesFound->append(InternalCrate::crateNameFromEscapedName(escapedCrateName));
        }
    }

    return crateNamesFound;
}

boolean Crate::isAnExistingFullCrateName(const String& fullCrateName, const String& seratoFolderPath)
{
    auto crateFilePath = InternalCrate::crateFilePathForFullCrateNameInSeratoFolder(fullCrateName, seratoFolderPath);
    return File::fileExistsAt(crateFilePath);
}

boolean Crate::isAnExistingFullSmartCrateName(const String& fullCrateName, const String& seratoFolderPath)
{
    auto crateFilePath = InternalCrate::crateFilePathForFullSmartCrateNameInSeratoFolder(fullCrateName, seratoFolderPath);
    return File::fileExistsAt(crateFilePath);
}

void Crate::parseCratesInSeratoFolderOnVolumeAddToCrateAndSaveSmartCrateNamesIn(String::ArrayOfConst& cratesInOrder,
                                                                                const String& seratoFolderPath,
                                                                                const String& volumePath,
                                                                                Serato::Crate& parentCrate,
                                                                                String::ArrayOfConst& smartCrateNames)
{
    for (auto fullEntryName : cratesInOrder) {
        if (!Serato::Crate::isAnExistingFullCrateName(fullEntryName, seratoFolderPath)) {
            if (Serato::Crate::isAnExistingFullSmartCrateName(fullEntryName, seratoFolderPath)) {
                smartCrateNames.append(fullEntryName);
            }

            continue;
        }

        auto newCrate = parentCrate.findOrAddCrateWithRelativeNameAndFullName(fullEntryName, fullEntryName);
        newCrate->readFromFolderInVolume(seratoFolderPath, volumePath);
    }
}

#pragma mark Instance Methods

const String& Crate::name(void) const
{
    return internal->name;
}

NxA::String::Pointer Crate::fullCrateName(void) const
{
    auto escapedName = InternalCrate::escapedNameFromCrateName(internal->name);
    if (internal->parentCrate.isValid()) {
        auto parentCratePointer = internal->parentCrate.pointer();
        auto parentFullCrateName = parentCratePointer->fullCrateName();
        if (parentFullCrateName->length()) {
            return NxA::String::stringWithFormat("%s%%%%%s", parentFullCrateName->toUTF8(), escapedName->toUTF8());
        }
    }

    return NxA::String::stringWith(escapedName);
}

void Crate::addFullCrateNameWithPrefixForCratesOnVolumeAndRecurseToChildren(String& destination, const char* prefix, const String& volumePath) const
{
    auto fullCrateName = this->fullCrateName();
    NXA_ASSERT_TRUE(fullCrateName->length() != 0);

    if (!internal->childrenCrates->length() ||
        (internal->indexOfVolumePath(volumePath) != internal->volumePaths->length())) {
        return;
    }

    destination.append(prefix);
    destination.append(*fullCrateName);
    destination.append("\n");

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

void Crate::removeCrate(Crate::Pointer& crate)
{
    NXA_ASSERT_TRUE(crate->hasParentCrate() && &(crate->parentCrate()) == this);

    crate->internal->parentCrate.release();
    internal->childrenCrates->remove(crate);

    internal->markCratesAsModified();
}

Crate::Pointer Crate::findOrAddCrateWithRelativeNameAndFullName(const String& relativeName, const String& fullCrateName)
{
    auto topCrateName = InternalCrate::topParentCrateNameFromFullCrateName(relativeName);

    Crate::NullablePointer crateFound;

    for (auto& crate : crates()) {
        if (crate->name() == topCrateName) {
            crateFound = crate;
            break;
        }
    }

    if (crateFound.isNull()) {
        crateFound = Serato::Crate::crateWithName(InternalCrate::crateNameFromEscapedName(topCrateName));

        this->addCrate(crateFound);
    }

    if (topCrateName == relativeName) {
        return crateFound->pointer();
    }

    return crateFound->findOrAddCrateWithRelativeNameAndFullName(relativeName.subString(topCrateName->length() + 2), fullCrateName);
}

bool Crate::crateOrChildrenCrateContainsTracks(void) const
{
    if (this->trackEntries()->length()) {
        return true;
    }

    for (auto& crate : *internal->childrenCrates) {
        if (crate->crateOrChildrenCrateContainsTracks()) {
            return true;
        }
    }

    return false;
}

void Crate::addTrackEntry(Serato::TrackEntry& trackEntry)
{
    if (trackEntry.hasParentCrate()) {
        NXA_ASSERT_TRUE(&trackEntry.parentCrate() == this);
        return;
    }

    trackEntry.internal->parentCrate = Crate::WeakPointer(this->pointer());

    count volumePathIndex = internal->indexOfVolumePathAndAddIfNotPresent(trackEntry.volumePath());
    (*internal->trackEntriesPerPath)[volumePathIndex].append(trackEntry);

    internal->markCratesAsModified();
}

void Crate::removeTrackEntry(TrackEntry::Pointer& trackEntry)
{
    NXA_ASSERT_TRUE(trackEntry->hasParentCrate() && &(trackEntry->parentCrate()) == this);

    trackEntry->internal->parentCrate.release();

    count volumePathIndex = internal->indexOfVolumePath(trackEntry->volumePath());
    NXA_ASSERT_TRUE(volumePathIndex != internal->volumePaths->length());

    auto& trackEntries = (*internal->trackEntriesPerPath)[volumePathIndex];

    auto position = trackEntries.find(trackEntry);
    if (position != trackEntries.end()) {
        trackEntries.removeObjectAt(position);
        internal->markCratesAsModified();
    }
}

void Crate::readFromFolderInVolume(const String& seratoFolderPath, const String& volumePath)
{
    auto otherTags = Serato::Tag::ArrayOfConst::array();
    auto trackEntries = Serato::TrackEntry::Array::array();

    try {
        auto filePath = InternalCrate::crateFilePathForFullCrateNameInSeratoFolder(this->fullCrateName(), seratoFolderPath);

        auto crateFileData = File::readFileAt(filePath);
        auto tags = TagFactory::parseTagsAt(crateFileData->data(), crateFileData->size(), filePath);
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
                    entry->internal->parentCrate = Crate::WeakPointer(this->pointer());
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

boolean Crate::isEmpty(void) const
{
    if (internal->childrenCrates->length()) {
        return false;
    }

    for (auto& entries : *(internal->trackEntriesPerPath)) {
        if (entries->length()) {
            return false;
        }
    }

    return true;
}

void Crate::resetModificationFlags()
{
    internal->cratesWereModified = false;
}

boolean Crate::childrenCratesWereModified(void) const
{
    return internal->cratesWereModified;
}

void Crate::saveIfOnVolumeAndRecurseToChildren(const String& volumePath, const String& seratoFolderPath) const
{
    count pathIndex = -1;

    boolean crateIsEmpty = this->isEmpty();
    if (!crateIsEmpty) {
        for (count pathIndexLookup = 0; pathIndexLookup < internal->volumePaths->length(); ++pathIndexLookup) {
            if ((*internal->volumePaths)[pathIndexLookup] == volumePath) {
                pathIndex = pathIndexLookup;
                break;
            }
        }

        if (pathIndex == -1) {
            return;
        }
    }

    auto outputData = Blob::blob();

    auto versionTag = VersionTag::tagWithIdentifierAndValue(crateVersionTagIdentifier,
                                                            String::stringWith(crateFileCurrentVersionString));
    versionTag->addTo(outputData);

    if (!crateIsEmpty) {
        auto& trackEntries = (*internal->trackEntriesPerPath)[pathIndex];
        for (auto& trackEntry : trackEntries) {
            trackEntry->tagForEntry().addTo(outputData);
        }

        auto& otherTags = (*internal->otherTagsPerPath)[pathIndex];
        for (auto& tag : otherTags) {
            tag->addTo(outputData);
        }
    }

    internal->saveDataToCrateFileInSeratoFolder(outputData, seratoFolderPath, this->fullCrateName());

    for (auto& crate : *internal->childrenCrates) {
        crate->saveIfOnVolumeAndRecurseToChildren(volumePath, seratoFolderPath);
    }
}

TrackEntry::Array::Pointer Crate::removeAndReturnTrackEntries(void)
{
    auto entries = TrackEntry::Array::arrayWith(this->trackEntries());
    for (auto& entry : *entries) {
        auto entryPointer = entry->pointer();
        this->removeTrackEntry(entryPointer);
    }

    internal->markCratesAsModified();

    return entries;
}

Crate::Array::Pointer Crate::removeAndReturnChildrenCrates(void)
{
    auto crates = Crate::Array::arrayWith(this->crates());
    for (auto& crate : *crates) {
        auto cratePointer = crate->pointer();
        this->removeCrate(cratePointer);
    }

    internal->markCratesAsModified();

    return crates;
}

#pragma mark Overridden Object Instance Methods

String::Pointer Crate::description(void) const
{
    return String::stringWithFormat("Crate with name '%s' at %08lx", this->fullCrateName()->toUTF8(), (long)this);
}
