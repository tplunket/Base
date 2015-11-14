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

#include "SeratoDb/CrateOrderFile.hpp"
#include "SeratoDb/Crate.hpp"
#include "SeratoDb/Database.hpp"

#include <Base/Base.hpp>
#include <Base/Internal/Object.hpp>

namespace NxA { namespace Serato { namespace Internal {
    struct CrateOrderFile : public NxA::Internal::Object {
        NXA_GENERATED_INTERNAL_DECLARATIONS_WITHOUT_CONSTRUCTORS_FOR(NxA::Serato, CrateOrderFile);

        #pragma mark Constructors & Desctructors
        CrateOrderFile(const String& path, Serato::Crate& root);

        #pragma mark Class Methods
        static String::Pointer crateNameIfValidCrateOrEmptyIfNot(const String& name);
        static boolean filenameIsAValidCrateName(const String& fileName);
        static String::Pointer crateNameFromFilename(const String& fileName);
        static String::ArrayOfConst::Pointer cratesInSubCratesDirectory(const String& directory);
        static String::ArrayOfConst::Pointer readCratesNamesInCrateOrderFile(const String& crateOrderFilePath);
        static void addCratesNamesAtTheStartOfUnlessAlreadyThere(String::ArrayOfConst& cratesToAddTo,
                                                                 const String::ArrayOfConst& cratesToAdd);

        #pragma mark Instance Variables
        String::PointerToConst crateOrderFilePath;

        Serato::Crate::Pointer rootCrate;

        String::ArrayOfConst::Pointer unknownCratesNames;

        #pragma mark Instance Methods
        void addChildrenCratesOfCrateNamedUsingNameList(Serato::Crate& parentCrate,
                                                        const String& name,
                                                        String::ArrayOfConst::iterator& it,
                                                        const String::ArrayOfConst::iterator& end,
                                                        const String& seratoFolderPath,
                                                        const String& rootFolderPath);
    };
} } }
