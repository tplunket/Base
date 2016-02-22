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

#include <Base/Base.hpp>
#include "Markers/Marker.hpp"

NXA_ENTER_NAMESPACE(NxA);
NXA_ENTER_NAMESPACE(Serato);

NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(CueMarker);

class CueMarker : public Marker {
    NXA_GENERATED_DECLARATIONS_WITHOUT_OPERATOR_EQUAL_IN_NAMESPACE_FOR_CLASS(NxA::Serato, CueMarker);

public:
    #pragma mark Factory Methods
    static CueMarker::Pointer markerWithMemoryAt(const byte* id3TagStart);
    static CueMarker::Pointer markerV1WithIndexAndRawMemoryAt(uinteger16 index, const byte* tagStart);
    static CueMarker::Pointer markerV1WithIndexAndEncodedMemoryAt(uinteger16 index, const byte* tagStart);
    static CueMarker::Pointer markerWithLabelPositionIndexAndColor(const String& label,
                                                                   uinteger32 positionInMilliseconds,
                                                                   uinteger16 index,
                                                                   byte colorRedComponent,
                                                                   byte colorGreenComponent,
                                                                   byte colorBlueComponent);
    static CueMarker::Pointer markerWith(const CueMarker&other);

    #pragma mark Class Methods
    static NxA::String::Pointer stringRepresentationForTimeInMilliseconds(uinteger32 timeInMilliseconds);

    #pragma mark Operators
    virtual bool operator==(const CueMarker& other) const;

    #pragma mark Instance Methods
    uinteger32 positionInMilliseconds(void) const;
    uinteger16 index(void) const;
    const String& label(void) const;
    byte colorRedComponent(void) const;
    byte colorGreenComponent(void) const;
    byte colorBlueComponent(void) const;

    void addMarkerV2TagTo(Blob& data) const;

    void addRawMarkerV1TagTo(Blob& data) const;
    void addEncodedMarkerV1TagTo(Blob& data) const;
    static void addEmptyRawMarkerV1TagTo(Blob& data);
    static void addEmptyEncodedMarkerV1TagTo(Blob& data);

    #pragma mark Overidden Object Instance Methods
    virtual NxA::String::Pointer description(void) const override;
};

NXA_EXIT_NAMESPACE;
NXA_EXIT_NAMESPACE;
