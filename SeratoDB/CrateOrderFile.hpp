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

#include <SeratoDB/TrackEntry.hpp>

#include <Base/Base.hpp>

namespace NxA { namespace Serato {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(CrateOrderFile);

    class Crate;

    class CrateOrderFile : public Object {
        NXA_GENERATED_OPERATOR_EQUAL_DECLARATION_IN_NAMESPACE_FOR_CLASS(NxA::Serato, CrateOrderFile);
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(NxA::Serato, CrateOrderFile);

    public:
        #pragma mark Factory Methods
        static CrateOrderFile::Pointer fileWithSeratoFolderInRootFolder(const String& seratoFolderPath,
                                                                        const String& rootFolderPath);
        static CrateOrderFile::Pointer fileWithSeratoFolderInRootFolderWithRootCrate(const String& seratoFolderPath,
                                                                                     const String& rootFolderPath,
                                                                                     Crate& rootCrate);

        #pragma mark Class Methods
        static String::Pointer pathForFileInSeratoFolder(const String& seratoFolderPath);

        #pragma mark Instance Methods
        Crate& rootCrate(void);

        timestamp modificationDateInSecondsSince1970(void) const;

        void saveIfModified(void) const;
    };
} }
