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

#include "TrackFiles/OGGTrackFile.hpp"
#include "TrackFiles/Internal/InternalOGGTrackFile.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, OGGTrackFile, TrackFile);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

OGGTrackFile::Pointer OGGTrackFile::fileWithFileAt(const String& path, TrackFile::Flags flags)
{
    NXA_ASSERT_TRUE(path.length() > 0);

    auto internalObject = InternalOGGTrackFile::Pointer(std::make_shared<InternalOGGTrackFile>(path));
    auto newFile = OGGTrackFile::makeSharedWithInternal(InternalObject::Pointer::dynamicCastFrom(internalObject));

    newFile->internal->markersWereIgnored = (flags & TrackFile::Flags::IgnoreMarkers);
    newFile->internal->loadAndParseFile();

    return newFile;
}

#pragma mark Instance Methods

boolean OGGTrackFile::hasFieldNamed(const String& name)
{
    return internal->nameOfFields->contains(name);
}

void OGGTrackFile::removeFieldNamed(const String& name)
{
    if (!this->hasFieldNamed(name)) {
        return;
    }
    
    internal->nameOfFieldsToRemove->append(name);
    internal->metadataWasModified = true;
}

#pragma mark Overriden TrackFile Instance Methods

boolean OGGTrackFile::hasRecordLabel(void) const
{
    return true;
}

boolean OGGTrackFile::hasRemixer(void) const
{
    return true;
}
