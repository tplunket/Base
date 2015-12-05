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

#include <SeratoDB/Track.hpp>
#include <SeratoDB/Crate.hpp>

#include <Base/Base.hpp>

namespace NxA { namespace Serato {
    NXA_EXCEPTION_NAMED_WITH_PARENT(DatabaseError, NxA::Exception);

    NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(Database);

    class TrackEntry;

    class Database : public Object {
        NXA_GENERATED_OPERATOR_EQUAL_DECLARATION_IN_NAMESPACE_FOR_CLASS(NxA::Serato, Database);
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(NxA::Serato, Database);

    public:
        #pragma mark Factory Methods
        static Database::Pointer databaseWithPathsForLocalAndExternalSeratoDirectories(const String& pathForLocalSeratoFolder,
                                                                                       const String::ArrayOfConst& pathsForExternalSeratoFolders);

        #pragma mark Class Methods
        static String::Pointer versionAsStringForDatabaseIn(const String& seratoFolderPath);
        static String::Pointer seratoFolderPathForFolder(const String& folderPath);
        static String::Pointer databaseFilePathForSeratoFolder(const String& seratoFolderPath);
        static boolean containsAValidSeratoFolder(const String& folderPath);
        static void createSeratoFolderIfDoesNotExists(const String& seratoFolderPath);

        #pragma mark Instance Methods
        timestamp databaseModificationDateInSecondsSince1970(void) const;
        timestamp rootCrateModificationDateInSecondsSince1970(void) const;

        Crate& rootCrate(void) const;
        const Track::Array& tracks(void) const;
        Track::Array::Pointer removeAndReturnTracks(void);
        const String& volumePathForTrackFilePath(const String& trackFilePath) const;

        void removeTrackEntry(TrackEntry::Pointer& trackEntry);
        void removeCrate(Crate::Pointer& crate);

        void addTrack(Track& track);
        void removeTrack(Track& track);

        void saveIfModifiedAndMarkAsModifiedOn(timestamp modificationTimesSince1970) const;
    };
} }
