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

#include "Internal/Database.hpp"
#include "Tags/DatabaseV2Tags.hpp"
#include "Tags/TagFactory.hpp"
#include "Tags/VersionTag.hpp"

// -- Generated internal implementation ommitted because this class does not use the default contructor.

using namespace NxA::Serato::Internal;

#pragma mark Constants

const char* Database::databaseFileCurrentVersionString = "2.0/Serato Scratch LIVE Database";

#pragma mark Constructors & Destructors

Database::Database(const String& path, const String& volume,
                   Serato::CrateOrderFile& usingCrateOrderFile) :
                   databaseFilePath(path.pointer()),
                   databaseVolume(volume.pointer()),
                   tracks(Serato::Track::Array::array()),
                   otherTags(Serato::Tag::ArrayOfConst::array()),
                   crateFilesToDelete(String::Array::array()),
                   crateOrderFile(usingCrateOrderFile.pointer()),
                   databaseIsValid(false) { }

#pragma mark Class Methods

#if NXA_PRINT_DEBUG_INFO
void Database::debugListCrate(Serato::Crate& crate,
                              const String& spacing)
{
    auto& crates = crate.crates();
    for (auto& subCrate : crates) {
        auto& crateName = subCrate->crateName();
        printf("%sCrate '%s'\n", spacing.toUTF8(), crateName.toUTF8());

        auto& crateTracks = subCrate->trackEntries();
        for (auto& trackEntry : crateTracks) {
            printf("%s   Track '%s'\n", spacing.toUTF8(), trackEntry->trackFilePath()->toUTF8());
        }

        String::Pointer newSpacing = String::stringWith(spacing);
        newSpacing->append("   ");

        Database::debugListCrate(subCrate, newSpacing);
    }
}
#endif

#pragma mark Instance Methods

void Database::parseDatabaseFile(void)
{
    auto databaseFile = File::readFileAt(databaseFilePath);

    auto tags = TagFactory::parseTagsAt(databaseFile->data(), databaseFile->size());
    for (auto& tag : *tags) {
        switch (tag->identifier()) {
            case trackObjectTagIdentifier: {
                storeTrackTag(dynamic_cast<Serato::ObjectTag&>(*tag));
                break;
            }
            case databaseVersionTagIdentifier: {
                auto& versionText = dynamic_cast<Serato::VersionTag&>(*tag).value();
                if (versionText != databaseFileCurrentVersionString) {
                    tracks->emptyAll();
                    otherTags->emptyAll();
                    return;
                }
                break;
            }
            default: {
                storeOtherTag(tag);
                break;
            }
        }
    }

#if NXA_PRINT_DEBUG_INFO
    debugListCrate(crateOrderFile->rootCrate(), String::string());
#endif

    databaseIsValid = true;
}

void Database::storeTrackTag(Serato::ObjectTag& tag)
{
    auto track = Serato::Track::trackWithTagOnVolume(tag, databaseVolume);
    this->tracks->append(track);
}

void Database::storeOtherTag(const Serato::Tag& tag)
{
    this->otherTags->append(tag);
}
