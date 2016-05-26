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

#pragma once

#include "SeratoDB/Markers/Marker.hpp"

#include <Base/Base.hpp>

NXA_ENTER_NAMESPACE(NxA);
NXA_ENTER_NAMESPACE(Serato);

NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(GridMarker);

class GridMarker : public Marker {
    NXA_GENERATED_DECLARATIONS_WITHOUT_OPERATOR_EQUAL_IN_NAMESPACE_FOR_CLASS(NxA::Serato, GridMarker);

public:
    #pragma mark Factory Methods
    static Pointer<GridMarker> markerWithPositionAndBeatsPerMinute(const decimal3& positionInSeconds,
                                                                   const decimal2& beatsPerMinute);
    static Pointer<GridMarker> markerWith(const GridMarker& other);

    #pragma mark Class Methods
    static boolean sizeIsCorrectForMarkerData(count size);
    static Pointer<GridMarker::Array> markersWithMemoryAt(const byte* id3TagStart);
    static void addMarkersTo(const GridMarker::Array& markers, Blob& data);
    static boolean gridMarkersAreValid(const GridMarker::Array& markers);

    #pragma mark Operators
    virtual bool operator==(const GridMarker& other) const;

    #pragma mark Instance Methods
    const decimal3& positionInSeconds(void) const;
    Pointer<String> positionInSecondsAsString(void) const;
    const decimal2& beatsPerMinute(void) const;
    Pointer<String> beatsPerMinuteAsString(void) const;

    #pragma mark Overidden Object Instance Methods
    virtual NxA::Pointer<String> description(void) const override;
};

NXA_EXIT_NAMESPACE;
NXA_EXIT_NAMESPACE;
