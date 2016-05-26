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

#include <SeratoDB/Track.hpp>
#include <SeratoDB/Crate.hpp>

#include <Base/Base.hpp>

NXA_ENTER_NAMESPACE(NxA);
NXA_ENTER_NAMESPACE(Serato);

NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(Database);

class TrackEntry;

class Database : public Object {
    NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(NxA::Serato, Database);

public:
    #pragma mark Factory Methods
    static Pointer<Database> databaseWithPathsForLocalAndExternalSeratoDirectories(const String& pathForLocalSeratoFolder,
                                                                                   const Array<const String>& pathsForExternalSeratoFolders);

    #pragma mark Class Methods
    static Pointer<String> versionAsStringForDatabaseIn(const String& seratoFolderPath);
    static Pointer<String> seratoFolderPathForFolder(const String& folderPath);
    static Pointer<String> databaseFilePathForSeratoFolder(const String& seratoFolderPath);
    static boolean containsAValidSeratoFolder(const String& folderPath);
    static void createSeratoFolderIfDoesNotExists(const String& seratoFolderPath);

    #pragma mark Instance Methods
    timestamp databaseModificationDateInSecondsSince1970(void) const;
    timestamp rootFolderModificationDateInSecondsSince1970(void) const;

    Crate& rootFolder(void) const;
    const Array<Track>& tracks(void) const;
    Pointer<Array<Track>> removeAndReturnTracks(void);
    const String& volumePathForTrackFilePath(const String& trackFilePath) const;

    void removeTrackEntry(Pointer<TrackEntry>& trackEntry);
    void removeCrate(Pointer<Crate>& crate);

    void addTrack(Track& track);
    void removeTrack(Track& track);

    void saveIfModifiedAndMarkAsModifiedOn(timestamp modificationTimesSince1970) const;
};

NXA_EXIT_NAMESPACE;
NXA_EXIT_NAMESPACE;
