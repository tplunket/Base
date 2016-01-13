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

#include "SeratoDB/TrackEntry.hpp"
#include "SeratoDB/Internal/TrackEntry.hpp"
#include "SeratoDB/Crate.hpp"
#include "Tags/PathTag.hpp"
#include "Tags/ObjectTag.hpp"
#include "Tags/CrateV1Tags.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, TrackEntry, Object);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

TrackEntry::Pointer TrackEntry::entryWithTagOnVolume(const ObjectTag& tag, const String& volumePath)
{
    NXA_ASSERT_TRUE(volumePath.length() != 0);

    auto internalObject = Internal::TrackEntry::Pointer(std::make_shared<Internal::TrackEntry>(tag, volumePath));
    auto newTrackEntry = TrackEntry::makeSharedWithInternal(NxA::Internal::Object::Pointer::dynamicCastFrom(internalObject));
    return newTrackEntry;
}

TrackEntry::Pointer TrackEntry::entryWithTrackFileAtOnVolume(const String& path, const String& volumePath)
{
    NXA_ASSERT_TRUE(path.length() != 0);
    NXA_ASSERT_TRUE(volumePath.length() != 0);

    auto entryPath = File::removePrefixFromPath(volumePath, path);

    auto tags = Tag::Array::array();
    tags->append(Serato::Tag::Pointer::dynamicCastFrom(PathTag::tagWithIdentifierAndValue(trackEntryPathTagIdentifier, entryPath)));

    auto trackEntryTag = ObjectTag::tagWithIdentifierAndValue(trackEntryTagIdentifier, tags);
    return TrackEntry::entryWithTagOnVolume(trackEntryTag, volumePath);
}

#pragma mark Instance Methods

String::Pointer TrackEntry::trackFilePath(void) const
{
    auto& trackObjectTag = dynamic_cast<const ObjectTag&>(*internal->trackEntryTag);
    if (trackObjectTag.hasSubTagForIdentifier(trackEntryPathTagIdentifier)) {
        auto& pathTag = dynamic_cast<const PathTag&>(trackObjectTag.subTagForIdentifier(trackEntryPathTagIdentifier));
        auto& pathFromVolumePath = pathTag.value();

        auto trackFilePath = File::joinPaths(internal->volumePath, pathFromVolumePath);
        return trackFilePath;
    }

    return String::string();
}

const String& TrackEntry::volumePath(void) const
{
    return *(internal->volumePath);
}

boolean TrackEntry::hasParentCrate(void) const
{
    return internal->parentCrate.isValid();
}

Crate& TrackEntry::parentCrate(void)
{
    return *(internal->parentCrate.pointer());
}

const Tag& TrackEntry::tagForEntry(void) const
{
    return internal->trackEntryTag;
}
