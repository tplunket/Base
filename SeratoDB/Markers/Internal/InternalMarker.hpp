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
#include <Base/Internal/InternalObject.hpp>

NXA_ENTER_NAMESPACE(NxA);
NXA_ENTER_NAMESPACE(Serato);

struct InternalMarker : public InternalObject {
    NXA_GENERATED_INTERNAL_DECLARATIONS_FOR(NxA::Serato, InternalMarker);

    typedef struct {
        byte position[4];
        byte loopPosition[4];
        byte zero;
        byte loopIterations[4];
        byte color[4];
        byte type;
        byte locked;
    } SeratoRawTagV1Struct;

    typedef struct {
        byte position[5];
        byte loopPosition[5];
        byte zero;
        byte loopIterations[5];
        byte color[4];
        byte type;
        byte locked;
    } SeratoEncodedTagV1Struct;

    enum MarkerType {
        eCueMarker = 1,
        eLoopMarker = 3,
        eEmptyMarker = 0,
    };

    static void addRawMarkerV1TagWithFieldsTo(MarkerType type, integer32 position, integer32 loopPos, integer32 loopIterations,
                                              byte red, byte green, byte blue, Blob& data);

    static void addEncodedMarkerV1TagWithFieldsTo(MarkerType type, integer32 position, integer32 loopPos, integer32 loopIterations,
                                                  byte red, byte green, byte blue, Blob& data);

    static void rawV1TagFromEncodedV1TagStruct(SeratoRawTagV1Struct& rawTag, const SeratoEncodedTagV1Struct& encodedTag);
};

NXA_EXIT_NAMESPACE;
NXA_EXIT_NAMESPACE;
