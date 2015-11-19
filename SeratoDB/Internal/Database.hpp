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

#include "SeratoDB/Database.hpp"
#include "SeratoDB/Track.hpp"
#include "SeratoDB/Crate.hpp"
#include "Tags/Tag.hpp"
#include "Tags/ObjectTag.hpp"

#include <Base/Base.hpp>
#include <Base/Internal/Object.hpp>

namespace NxA { namespace Serato { namespace Internal {
    struct Database : public NxA::Internal::Object {
        NXA_GENERATED_INTERNAL_DECLARATIONS_WITHOUT_CONSTRUCTORS_FOR(NxA::Serato, Database);

        #pragma mark Constructors & Destructors
        Database(const String& pathForLocalSeratoFolder,
                 const String::ArrayOfConst& pathsForExternalSeratoFolders);

        #pragma mark Class Methods
        static String::Pointer pathForCrateOrderFileInSeratoFolder(const String& seratoFolderPath);

        static void addCratesFoundInSeratoFolderOnVolumeToRootCrate(const String& seratoFolderPath,
                                                                    const String& volumePath,
                                                                    Serato::Crate& rootCrate,
                                                                    String::ArrayOfConst& unknownCratesNames);

        static void saveContentOfRootCrateIfModifiedAndOnVolumeAndUnknownCrateNamesToSeratoFolder(const Serato::Crate& rootCrate,
                                                                                                  const String& volumePath,
                                                                                                  const String::ArrayOfConst& unknownCratesNames,
                                                                                                  const String& seratoFolderPath);

        static void setDatabaseFilesInSeratoFolderAsModifedOnDateInSecondsSince1970(const String& folderPath, timestamp dateModified);

        static void addCratesNamesAtTheStartOfUnlessAlreadyThere(String::ArrayOfConst& cratesToAddTo,
                                                                 const String::ArrayOfConst& cratesToAdd);

#if NXA_PRINT_DEBUG_INFO
        static void debugListCrate(Serato::Crate& crate,
                                   const String& spacing);
#endif

        #pragma mark Constants
        static const char* databaseFileCurrentVersionString;

        #pragma mark Instance Variables
        Serato::Crate::Pointer rootCrate;
        Serato::Track::Array::Pointer tracks;

        String::ArrayOfConst::Pointer pathsForSeratoDirectories;
        String::ArrayOfConst::Pointer volumePathsPerPath;
        Serato::Tag::ArrayOfConst::Array::Pointer otherTagsPerPath;
        String::ArrayOfConst::Array::Pointer otherCrateNamesPerPath;

        boolean databaseIsValid;

        #pragma mark Instance Methods
        Serato::Tag::ArrayOfConst::Pointer parseDatabaseFileAtLocatedOnVolumeAndReturnOtherTags(const String& databasePath,
                                                                                                const String& volumePath);
        void parseAnyDatabaseFilesIn(const String& pathForLocalSeratoFolder,
                                     const String::ArrayOfConst& pathsForExternalSeratoFolders);

        void storeTrackTagLocatedOnVolume(Serato::ObjectTag& tag, const String& volumePath);
    };
} } }
