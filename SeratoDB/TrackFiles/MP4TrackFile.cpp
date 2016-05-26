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

#include "SeratoDB/TrackFiles/MP4TrackFile.hpp"
#include "SeratoDB/TrackFiles/Internal/InternalMP4TrackFile.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, MP4TrackFile, TrackFile);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

Pointer<MP4TrackFile> MP4TrackFile::fileWithFileAt(const String& path, TrackFile::Flags flags)
{
    NXA_ASSERT_TRUE(path.length() > 0);

    auto internalObject = Pointer<InternalMP4TrackFile>(std::make_shared<InternalMP4TrackFile>(path));
    auto newFile = MP4TrackFile::makeSharedWithInternal(Pointer<NxA::InternalObject>::dynamicCastFrom(internalObject));

    newFile->internal->markersWereIgnored = (flags & TrackFile::Flags::IgnoreMarkers);
    newFile->internal->loadAndParseFile();

    return newFile;
}

#pragma mark Instance Methods

boolean MP4TrackFile::hasItemNamed(const String& name)
{
    return internal->nameOfItems->contains(name);
}

void MP4TrackFile::removeItemNamed(const String& name)
{
    // -- For MP4 files we don't check if the item is there or not because we might to remove an item that
    // -- does not get loaded properly and therefore is not there but will also no get saved if the
    // -- metadata is marked as modified, thus achieving the same result for now. This will be fixed
    internal->nameOfItemsToRemove->append(name);
    internal->metadataWasModified = true;
}

#pragma mark Overriden TrackFile Instance Methods

boolean MP4TrackFile::hasKey(void) const
{
    return true;
}

boolean MP4TrackFile::hasRecordLabel(void) const
{
    return true;
}

void MP4TrackFile::setBpm(const String& bpm)
{
    integer integerBpm = bpm.integerValue();

    if (integerBpm != internal->bpm->integerValue()) {
        internal->bpm = String::stringWith(bpm);
        internal->metadataWasModified = true;
    }
}

void MP4TrackFile::setReleaseDate(const String& date)
{
    if (date.length()) {
        auto components = date.splitBySeperator('-');
        if (components->length()) {
            auto dateWithOnlyYear = String::stringWithFormat("%s-01-01", components->firstObject().toUTF8());
            this->TrackFile::setReleaseDate(dateWithOnlyYear);
            return;
        }
    }

    this->TrackFile::setReleaseDate(date);
}
