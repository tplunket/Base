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

#pragma once

#include <Base/Base.hpp>

namespace NxA { namespace Serato {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(GridMarker);

    class GridMarker : public Object {
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(NxA::Serato, GridMarker);

    public:
        #pragma mark Factory Methods
        static GridMarker::Pointer markerWithPositionAndBeatsPerMinute(const decimal3& positionInSeconds,
                                                                       const decimal2& beatsPerMinute);
        static GridMarker::Pointer markerWith(const GridMarker& other);

        #pragma mark Class Methods
        static GridMarker::Array::Pointer markersWithMemoryAt(const byte* id3TagStart);
        static void addMarkersTo(const GridMarker::Array& markers, Blob& data);
        static boolean gridMarkersAreValid(const GridMarker::Array& markers);

        #pragma mark Operators
        bool operator==(const GridMarker& other) const;

        #pragma mark Instance Methods
        const decimal3& positionInSeconds(void) const;
        String::Pointer positionInSecondsAsString(void) const;
        const decimal2& beatsPerMinute(void) const;
        String::Pointer beatsPerMinuteAsString(void) const;

        #pragma mark Overidden Object Instance Methods
        virtual NxA::String::Pointer description(void) const;
    };
} }
