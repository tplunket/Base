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
    NXA_EXCEPTION_NAMED_WITH_PARENT(LoopMarkerError, NxA::Exception);

    NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(LoopMarker);

    class LoopMarker : public Object {
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(NxA::Serato, LoopMarker);
        
    public:
        #pragma mark Factory Methods
        static LoopMarker::Pointer markerWithMemoryAt(const byte* id3TagStart);
        static LoopMarker::Pointer markerWithLabelStartEndPositionsIndexAndColor(const String& label,
                                                                                 uinteger32 startPositionInMilliseconds,
                                                                                 uinteger32 endPositionInMilliseconds,
                                                                                 uinteger16 index,
                                                                                 byte colorRedComponent,
                                                                                 byte colorGreenComponent,
                                                                                 byte colorBlueComponent);
        static LoopMarker::Pointer markerWith(const LoopMarker& other);

        #pragma mark Operators
        bool operator==(const LoopMarker& other) const;

        #pragma mark Instance Methods
        uinteger32 startPositionInMilliseconds(void) const;
        uinteger32 endPositionInMilliseconds(void) const;
        uinteger16 index(void) const;
        const String& label(void) const;
        byte colorRedComponent(void) const;
        byte colorGreenComponent(void) const;
        byte colorBlueComponent(void) const;

        void addId3TagTo(Blob& data) const;

        #pragma mark Overidden Object Instance Methods
        virtual NxA::String::Pointer description(void);
    };
} }
