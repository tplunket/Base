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

#include "SeratoDB/Crate.hpp"
#include "Internal/Crate.hpp"
#include "SeratoDB/Database.hpp"
#include "Tags/CrateV1Tags.hpp"
#include "Tags/VersionTag.hpp"
#include "Tags/TagFactory.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, Crate, Object);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Constants

static const character* crateFileCurrentVersionString = "1.0/Serato ScratchLive Crate";

#pragma mark Factory Methods

Crate::Pointer Crate::crateWithName(const String& crateFullName)
{
    auto internalObject = Internal::Crate::Pointer(std::make_shared<Internal::Crate>(crateFullName));
    auto newCrate = Crate::makeSharedWithInternal(NxA::Internal::Object::Pointer::dynamicCastFrom(internalObject));

    auto lastSeperatorIndex = crateFullName.indexOfLastOccurenceOf("%%");
    if (lastSeperatorIndex != crateFullName.length()) {
        newCrate->internal->crateName = crateFullName.subString(lastSeperatorIndex + 2);
    }
    else {
        newCrate->internal->crateName = crateFullName.pointer();
    }

    return newCrate;
}

#pragma mark Class Methods

String::Pointer Crate::subCratesDirectoryPathInSeratoFolder(const String& seratoFolderPath)
{
    auto joinedPath = File::joinPaths(seratoFolderPath, String::stringWith("Subcrates"));
    return joinedPath;
}

boolean Crate::isAValidCrateName(const String& crateFullName, const String& seratoFolderPath)
{
    auto crateFilePath = Internal::Crate::crateFilePathForCrateNameInSeratoFolder(crateFullName, seratoFolderPath);
    return File::fileExistsAt(crateFilePath);
}

boolean Crate::isASmartCrateName(const String& crateFullName, const String& seratoFolderPath)
{
    auto crateFilePath = Internal::Crate::crateFilePathForSmartCrateNameInSeratoFolder(crateFullName, seratoFolderPath);
    return File::fileExistsAt(crateFilePath);
}

#pragma mark Instance Methods

const String& Crate::crateName(void) const
{
    return internal->crateName;
}

const String& Crate::crateFullName(void) const
{
    return internal->crateFullName;
}

void Crate::addFullCrateNameWithPrefixAndRecurseToChildren(String& destination, const char* prefix) const
{
    if (internal->crateFullName->length()) {
        destination.append(prefix);
        destination.append(internal->crateFullName);
        destination.append("\n");
    }

    for (auto& crate : *internal->childrenCrates) {
        crate->addFullCrateNameWithPrefixAndRecurseToChildren(destination, prefix);
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

    if (!crate.trackEntries()->length() && !crate.crates().length()) {
        // -- Serato deletes empty crates anyway wo we don't even add them.
        return;
    }

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

Crate::Pointer Crate::findOrAddCrateWithName(const String& crateName)
{
    for (auto& crate : crates()) {
        if (crate->crateFullName() == crateName) {
            return crate;
        }
    }

    auto newCrate = Serato::Crate::crateWithName(crateName);
    this->addCrate(newCrate);
    return newCrate;
}

void Crate::addTrackEntry(Serato::TrackEntry& trackEntry)
{
    if (trackEntry.hasParentCrate()) {
        NXA_ASSERT_TRUE(&trackEntry.parentCrate() == this);
        return;
    }

    trackEntry.setParentCrate(*this);

    count volumePathIndex = internal->indexOfVolumePath(trackEntry.volumePath());
    (*internal->trackEntriesPerPath)[volumePathIndex].append(trackEntry);

    internal->tracksWereModified = true;
}

void Crate::removeTrackEntry(TrackEntry& trackEntry)
{
    count volumePathIndex = internal->indexOfVolumePath(trackEntry.volumePath());
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
    auto filePath = Internal::Crate::crateFilePathForCrateNameInSeratoFolder(this->crateFullName(), seratoFolderPath);

    auto crateFileData = File::readFileAt(filePath);
    auto tags = TagFactory::parseTagsAt(crateFileData->data(), crateFileData->size());
    if (!tags->length()) {
        return;
    }

    auto otherTags = Serato::Tag::ArrayOfConst::array();
    auto trackEntries = Serato::TrackEntry::Array::array();

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
            auto outputData = Blob::blob();

            auto versionTag = VersionTag::tagWithIdentifierAndValue(crateVersionTagIdentifier,
                                                                    String::stringWith(crateFileCurrentVersionString));
            versionTag->addTo(outputData);

            auto& trackEntries = (*internal->trackEntriesPerPath)[pathIndex];
            for (auto& trackEntry : trackEntries) {
                trackEntry->tagForEntry().addTo(outputData);
            }

            auto& otherTags = (*internal->otherTagsPerPath)[pathIndex];
            for (auto& tag : otherTags) {
                tag->addTo(outputData);
            }

            Database::createSeratoFolderIfDoesNotExists(seratoFolderPath);

            auto cratesFolderPath = NxA::Serato::Crate::subCratesDirectoryPathInSeratoFolder(seratoFolderPath);
            if (!File::directoryExistsAt(cratesFolderPath)) {
                File::createDirectoryAt(cratesFolderPath);
            }

            auto crateFilePath = Internal::Crate::crateFilePathForCrateNameInSeratoFolder(this->crateFullName(), seratoFolderPath);
            File::writeBlobToFileAt(outputData, crateFilePath);

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
