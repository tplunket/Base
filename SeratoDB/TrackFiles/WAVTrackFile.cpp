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

#include "SeratoDB/TrackFiles/WAVTrackFile.hpp"
#include "SeratoDB/TrackFiles/Internal/InternalWAVTrackFile.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, WAVTrackFile, ID3TrackFile);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

NxA::Pointer<WAVTrackFile> WAVTrackFile::fileWithFileAt(const String& path, TrackFile::Flags flags)
{
    NXA_ASSERT_TRUE(path.length() > 0);

    auto internalObject = NxA::Pointer<InternalWAVTrackFile>(std::make_shared<InternalWAVTrackFile>(path));
    auto newFile = WAVTrackFile::makeSharedWithInternal(NxA::Pointer<InternalObject>::dynamicCastFrom(internalObject));

    newFile->internal->markersWereIgnored = (flags & TrackFile::Flags::IgnoreMarkers);
    newFile->internal->loadAndParseFile();

    return newFile;
}

#pragma mark Overriden TrackFile Instance Methods

boolean WAVTrackFile::hasBitDepth(void) const
{
    return true;
}
