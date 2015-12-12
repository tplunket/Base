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

#include "SeratoDB/TrackEntry.hpp"
#include "SeratoDB/Crate.hpp"
#include "Tags/ObjectTag.hpp"
#include "Tags/PathTag.hpp"
#include "Tags/CrateV1Tags.hpp"
#include "Base/Test.hpp"

using namespace testing;
using namespace NxA;
using namespace NxA::Serato;

NXA_CONTAINS_TEST_SUITE_NAMED(SeratoDB_TrackEntry_Tests);

TEST(SeratoDB_TrackEntry, EntryWithTagOnVolume_EntryWithInitialValues_ReturnsAValidEntry)
{
    // -- Given.
    auto tags = Tag::Array::array();
    tags->append(PathTag::tagWithIdentifierAndValue(trackEntryPathTagIdentifier, String::stringWith("Users/didier/Music/Gigs/02 Money On My Mind (MK Remix).m4a")));
    auto tag = ObjectTag::tagWithIdentifierAndValue(trackEntryTagIdentifier, tags);
    auto volumePath = String::stringWith("/");

    // -- When.
    auto entry = TrackEntry::entryWithTagOnVolume(tag, volumePath);

    // -- Then.
    ASSERT_STREQ("/Users/didier/Music/Gigs/02 Money On My Mind (MK Remix).m4a", entry->trackFilePath()->toUTF8());
}

TEST(SeratoDB_TrackEntry, EntryWithTrackFileAtOnVolume_EntryWithInitialValues_ReturnsAValidEntry)
{
    // -- Given.
    auto filePath = String::stringWith("/Users/didier/Music/Gigs/02 Money On My Mind (MK Remix).m4a");
    auto volumePath = String::stringWith("/");

    // -- When.
    auto entry = TrackEntry::entryWithTrackFileAtOnVolume(filePath, volumePath);

    // -- Then.
    ASSERT_STREQ("/Users/didier/Music/Gigs/02 Money On My Mind (MK Remix).m4a", entry->trackFilePath()->toUTF8());
}

TEST(SeratoDB_TrackEntry, HasParentCrate_EntryWithAParentCrate_ReturnsTrue)
{
    // -- Given.
    auto crate = Crate::crateWithName(String::stringWith("MyCrate1"));
    auto entry = TrackEntry::entryWithTrackFileAtOnVolume(String::stringWith("/Users/didier/Music/Gigs/02 Money On My Mind (MK Remix).m4a"), String::stringWith("/"));
    crate->addTrackEntry(entry);

    // -- When.
    // -- Then.
    ASSERT_TRUE(entry->hasParentCrate());
}

TEST(SeratoDB_TrackEntry, HasParentCrate_EntryWithNoParentCrate_ReturnsFalse)
{
    // -- Given.
    auto entry = TrackEntry::entryWithTrackFileAtOnVolume(String::stringWith("/Users/didier/Music/Gigs/02 Money On My Mind (MK Remix).m4a"), String::stringWith("/"));

    // -- When.
    // -- Then.
    ASSERT_FALSE(entry->hasParentCrate());
}

TEST(SeratoDB_TrackEntry, HasParentCrate_EntryWithParentCrateRemoved_ReturnsFalse)
{
    // -- Given.
    auto crate = Crate::crateWithName(String::stringWith("MyCrate1"));
    auto entry = TrackEntry::entryWithTrackFileAtOnVolume(String::stringWith("/Users/didier/Music/Gigs/02 Money On My Mind (MK Remix).m4a"), String::stringWith("/"));
    crate->addTrackEntry(entry);
    crate->removeTrackEntry(entry);

    // -- When.
    // -- Then.
    ASSERT_FALSE(entry->hasParentCrate());
}

TEST(SeratoDB_TrackEntry, ParentCrate_EntryWithAParentCrate_ReturnsParentCrate)
{
    // -- Given.
    auto crate = Crate::crateWithName(String::stringWith("MyCrate1"));
    auto entry = TrackEntry::entryWithTrackFileAtOnVolume(String::stringWith("/Users/didier/Music/Gigs/02 Money On My Mind (MK Remix).m4a"), String::stringWith("/"));
    crate->addTrackEntry(entry);

    // -- When.
    // -- Then.
    ASSERT_EQ(&(*crate), &entry->parentCrate());
}

TEST(SeratoDB_TrackEntry, ParentCrate_EntryWithoutAParentCrate_ThrowsException)
{
    // -- Given.
    auto entry = TrackEntry::entryWithTrackFileAtOnVolume(String::stringWith("/Test/MyFile.mp4"), String::stringWith("/"));

    // -- When.
    // -- Then.
    ASSERT_THROW(entry->parentCrate(), NxA::Exception);
}

TEST(SeratoDB_TrackEntry, TagForEntry_AnEntryWithValues_ReturnsTheCorrectTag)
{
    // -- Given.
    auto filePath = String::stringWith("/Users/didier/Music/Gigs/02 Money On My Mind (MK Remix).m4a");
    auto volumePath = String::stringWith("/");
    auto entry = TrackEntry::entryWithTrackFileAtOnVolume(filePath, volumePath);

    // -- When.
    auto& tag = entry->tagForEntry();

    // -- Then.
    auto& objectTag = dynamic_cast<const ObjectTag&>(tag);
    ASSERT_EQ(trackEntryTagIdentifier, objectTag.identifier());
    ASSERT_EQ(1, objectTag.numberOfSubTags());
    auto& pathTag = dynamic_cast<const PathTag&>(objectTag.subTagForIdentifier(trackEntryPathTagIdentifier));
    ASSERT_EQ(trackEntryPathTagIdentifier, pathTag.identifier());
    ASSERT_EQ(filePath->toUTF8()[0], '/');
    ASSERT_STREQ(filePath->toUTF8() + 1, pathTag.value().toUTF8());
}
