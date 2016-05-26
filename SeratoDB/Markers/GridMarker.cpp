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

#include "SeratoDB/Markers/GridMarker.hpp"
#include "SeratoDB/Markers/CueMarker.hpp"
#include "SeratoDB/Markers/Internal/InternalGridMarker.hpp"

#include <math.h>

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, GridMarker, Marker);

NXA_ENTER_NAMESPACE(NxA);
NXA_ENTER_NAMESPACE(Serato);

#pragma mark Structures

typedef struct {
    byte positionInSeconds[4];
    byte beatsPerMinute[4];
} GridMarkerStruct;

NXA_EXIT_NAMESPACE;
NXA_EXIT_NAMESPACE;

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

Pointer<GridMarker> GridMarker::markerWithPositionAndBeatsPerMinute(const decimal3& positionInSeconds, const decimal2& beatsPerMinute)
{
    auto newMarker = GridMarker::makeShared();
    newMarker->internal->positionInSeconds = positionInSeconds;
    newMarker->internal->beatsPerMinute = beatsPerMinute;

    return newMarker;
}

Pointer<GridMarker> GridMarker::markerWith(const GridMarker& other)
{
    return GridMarker::markerWithPositionAndBeatsPerMinute(other.positionInSeconds(), other.beatsPerMinute());
}

#pragma mark Class Methods

boolean GridMarker::sizeIsCorrectForMarkerData(count size)
{
    return (size != 0) && !((size - 4) % sizeof(GridMarkerStruct));
}

Pointer<Array<GridMarker>> GridMarker::markersWithMemoryAt(const byte* id3TagStart)
{
    auto result = Array<GridMarker>::array();

    auto numberOfMarkers = Platform::bigEndianUInteger32ValueAt(id3TagStart);
    if (!numberOfMarkers) {
        return result;
    }
    else if (numberOfMarkers > 256) {
        numberOfMarkers = 256;
    }

    decimal3 nextPosition;

    const GridMarkerStruct* gridMarker = (const GridMarkerStruct* )(id3TagStart + 4);
    for (count markerIndex = 0; markerIndex < numberOfMarkers; ++markerIndex) {
        decimal3 position;
        if (markerIndex) {
            position = nextPosition;
        }
        else {
            position.setUnbiased(::roundf(Platform::bigEndianFloatValueAt(gridMarker->positionInSeconds) * 1000.0f));
        }

        decimal2 bpm;

        const GridMarkerStruct* nextGridMarker = gridMarker + 1;
        if ((markerIndex + 1) < numberOfMarkers) {
            nextPosition = Platform::bigEndianFloatValueAt(nextGridMarker->positionInSeconds);

            if (nextPosition == position) {
                gridMarker = nextGridMarker;
                continue;
            }

            decimal3 numberOfBeats(Platform::bigEndianUInteger32ValueAt(gridMarker->beatsPerMinute));
            decimal3 bpmAsDecimal3 = (numberOfBeats * decimal3("60.0")) / (nextPosition - position);
            bpm.setUnbiased((bpmAsDecimal3.getUnbiased() + 3) / 10);
        }
        else {
            bpm.setUnbiased(::roundf(Platform::bigEndianFloatValueAt(gridMarker->beatsPerMinute) * 100.0f));
        }

        auto newMarker = GridMarker::markerWithPositionAndBeatsPerMinute(position, bpm);
        result->append(newMarker);

        gridMarker = nextGridMarker;
    }
    
    return result;
}

void GridMarker::addMarkersTo(const Array<GridMarker>& markers, NxA::Blob& data)
{
    uinteger32 numberOfMarkers = static_cast<uinteger32>(markers.length());

    uinteger32 numberOfMarkersBuffer;
    Platform::writeBigEndianUInteger32ValueAt(numberOfMarkers, reinterpret_cast<byte*>(&numberOfMarkersBuffer));
    auto numberOfMarkersData = Blob::blobWithMemoryAndSize(reinterpret_cast<byte*>(&numberOfMarkersBuffer), sizeof(numberOfMarkersBuffer));
    data.append(numberOfMarkersData);

    count lastMarkerIndex = numberOfMarkers - 1;

    for (count index = 0; index < numberOfMarkers; ++index) {
        auto& marker = markers[index];

        GridMarkerStruct markerData;

        Platform::writeBigEndianFloatValueAt(marker.positionInSeconds().getAsDouble(), markerData.positionInSeconds);
        if (index == lastMarkerIndex) {
            Platform::writeBigEndianFloatValueAt(marker.beatsPerMinute().getAsDouble(), markerData.beatsPerMinute);
        }
        else {
            auto& nextMarker = markers[index + 1];
            decimal3 bpmDecimal3;
            bpmDecimal3.setUnbiased(marker.beatsPerMinute().getUnbiased() * 10);
            decimal3 numberOfBeats = (bpmDecimal3 * (nextMarker.positionInSeconds() - marker.positionInSeconds())) / decimal3("60");

            count actualNumberOfBeats = InternalGridMarker::actualNumberOfBeatsIfSupported(numberOfBeats);
            if (!actualNumberOfBeats) {
                data.removeAll();
                return;
            }

            Platform::writeBigEndianUInteger32ValueAt(static_cast<uinteger32>(actualNumberOfBeats), markerData.beatsPerMinute);
        }

        auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<const byte*>(&markerData), sizeof(GridMarkerStruct));
        data.append(headerData);
    }

    // -- This marks the end of tags.
    data.append('\0');
}

boolean GridMarker::gridMarkersAreValid(const Array<GridMarker>& markers)
{
    // -- Serato doesn't support very flexible grid markers, the ones we end up writing
    // -- might not be exactly the ones we wanted to write so we test them
    // -- before setting them and if at least one is invalid, we write none.
    count numberOfMarkers = markers.length();
    count lastMarkerIndex = numberOfMarkers - 1;

    for (count index = 0; index < numberOfMarkers; ++index) {
        auto& marker = markers[index];

        if (index != lastMarkerIndex) {
            auto& nextMarker = markers[index + 1];
            decimal3 bpmDecimal3;
            bpmDecimal3.setUnbiased(marker.beatsPerMinute().getUnbiased() * 10);

            auto markerPosition = marker.positionInSeconds();
            auto nextMarkerPosition = nextMarker.positionInSeconds();
            if (nextMarkerPosition == markerPosition) {
                return false;
            }

            decimal3 numberOfBeats = (bpmDecimal3 * (nextMarkerPosition - markerPosition)) / decimal3("60");

            count actualNumberOfBeats = InternalGridMarker::actualNumberOfBeatsIfSupported(numberOfBeats);
            if (actualNumberOfBeats == 0) {
                return false;
            }
        }
    }

    return true;
}

#pragma mark Operators

bool GridMarker::operator==(const GridMarker& other) const
{
    if (this == &other) {
        return true;
    }

    return (this->positionInSeconds() == other.positionInSeconds()) &&
           (this->beatsPerMinute() == other.beatsPerMinute());
}

#pragma mark Instance Methods

const decimal3& GridMarker::positionInSeconds(void) const
{
    return internal->positionInSeconds;
}

Pointer<String> GridMarker::positionInSecondsAsString(void) const
{
    integer64 integerPart = this->positionInSeconds().getUnbiased();
    integer64 decimalPart = integerPart % 1000;

    return String::stringWithFormat("%d.%03d", integerPart / 1000, decimalPart);
}

const decimal2& GridMarker::beatsPerMinute(void) const
{
    return internal->beatsPerMinute;
}

Pointer<String> GridMarker::beatsPerMinuteAsString(void) const
{
    integer64 integerPart = this->beatsPerMinute().getUnbiased();
    integer64 decimalPart = integerPart % 100;

    return String::stringWithFormat("%d.%02d", integerPart / 100, decimalPart);
}

#pragma mark Overriden Object Instance Methods

Pointer<String> GridMarker::description(void) const
{
    return NxA::String::stringWithFormat("Grid Marker at %s with bpm %s.",
                                         CueMarker::stringRepresentationForTimeInMilliseconds(this->positionInSeconds().getAsDouble() * 1000.0f)->toUTF8(),
                                         this->beatsPerMinuteAsString()->toUTF8());
}
