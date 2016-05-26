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

#include "SeratoDB/Internal/InternalTrack.hpp"
#include "SeratoDB/TrackFiles/TrackFileFactory.hpp"
#include "SeratoDB/Tags/ObjectTag.hpp"
#include "SeratoDB/Tags/TextTag.hpp"
#include "SeratoDB/Tags/BooleanTag.hpp"
#include "SeratoDB/Tags/PathTag.hpp"
#include "SeratoDB/Tags/UInteger32Tag.hpp"
#include "SeratoDB/Tags/DatabaseV2Tags.hpp"

// -- Generated internal implementation ommitted because this class does not use the default contructor.

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Constants

static Pointer<String> emptyString = String::string();

#pragma mark Constructors & Destructors

InternalTrack::InternalTrack(ObjectTag& tag,
                             const String& newVolumePath) :
    trackTag(tag.pointer()),
    volumePath(newVolumePath.pointer()),
    needsToUpdateDatabaseFile(false)
{
}

#pragma mark Instance Methods

const String& InternalTrack::stringForSubTagForIdentifier(uinteger32 identifier) const
{
    auto& trackObjectTag = *this->trackTag;
    if (trackObjectTag.hasSubTagForIdentifier(identifier)) {
        auto& textTag = dynamic_cast<const TextTag&>(trackObjectTag.subTagForIdentifier(identifier));
        return textTag.value();
    }

    return emptyString;
}

const String& InternalTrack::pathForSubTagForIdentifier(uinteger32 identifier) const
{
    auto& trackObjectTag = *this->trackTag;
    if (trackObjectTag.hasSubTagForIdentifier(identifier)) {
        auto& pathTag = dynamic_cast<const PathTag&>(trackObjectTag.subTagForIdentifier(identifier));
        return pathTag.value();
    }

    return emptyString;
}

uinteger32 InternalTrack::uint32ForSubTagForIdentifier(uinteger32 identifier) const
{
    auto& trackObjectTag = *this->trackTag;
    if (trackObjectTag.hasSubTagForIdentifier(identifier)) {
        auto& uintTag = dynamic_cast<const UInteger32Tag&>(trackObjectTag.subTagForIdentifier(identifier));
        return uintTag.value();
    }

    return 0;
}

boolean InternalTrack::booleanForSubTagForIdentifier(uinteger32 identifier) const
{
    auto& trackObjectTag = *this->trackTag;
    if (trackObjectTag.hasSubTagForIdentifier(identifier)) {
        auto& booleanTag = dynamic_cast<BooleanTag&>(trackObjectTag.subTagForIdentifier(identifier));
        return booleanTag.value();
    }

    return false;
}

void InternalTrack::setStringForSubTagForIdentifier(const String& value,
                                                    uinteger32 identifier)
{
    auto& trackObjectTag = *this->trackTag;
    if (!trackObjectTag.hasSubTagForIdentifier(identifier)) {
        auto textTag = TextTag::tagWithIdentifierAndValue(identifier, value);
        trackObjectTag.setSubTag(*textTag);
    }
    else {
        auto& textTag = dynamic_cast<TextTag&>(trackObjectTag.subTagForIdentifier(identifier));
        textTag.setValue(value);
    }

    this->needsToUpdateDatabaseFile = true;
}

void InternalTrack::setPathForSubTagForIdentifier(const String& value,
                                                  uinteger32 identifier)
{
    auto& trackObjectTag = dynamic_cast<ObjectTag&>(*this->trackTag);
    if (!trackObjectTag.hasSubTagForIdentifier(identifier)) {
        auto pathTag = PathTag::tagWithIdentifierAndValue(identifier, value);
        trackObjectTag.setSubTag(*pathTag);
    }
    else {
        auto& pathTag = dynamic_cast<PathTag&>(trackObjectTag.subTagForIdentifier(identifier));
        pathTag.setValue(value);
    }

    this->needsToUpdateDatabaseFile = true;
}

void InternalTrack::setUInt32ForSubTagForIdentifier(uinteger32 value,
                                                    uinteger32 identifier)
{
    auto& trackObjectTag = *this->trackTag;
    if (!trackObjectTag.hasSubTagForIdentifier(identifier)) {
        auto uinteger32Tag = UInteger32Tag::tagWithIdentifierAndValue(identifier, value);
        trackObjectTag.setSubTag(*uinteger32Tag);
    }
    else {
        auto& uinteger32Tag = dynamic_cast<UInteger32Tag&>(trackObjectTag.subTagForIdentifier(identifier));
        uinteger32Tag.setValue(value);
    }

    this->needsToUpdateDatabaseFile = true;
}

void InternalTrack::setBooleanForSubTagForIdentifier(boolean value,
                                                     uinteger32 identifier)
{
    auto& trackObjectTag = *this->trackTag;
    if (!trackObjectTag.hasSubTagForIdentifier(identifier)) {
        auto booleanTag = BooleanTag::tagWithIdentifierAndValue(identifier, value);
        trackObjectTag.setSubTag(*booleanTag);
    }
    else {
        auto& booleanTag = dynamic_cast<BooleanTag&>(trackObjectTag.subTagForIdentifier(identifier));
        booleanTag.setValue(value);
    }

    this->needsToUpdateDatabaseFile = true;
}
