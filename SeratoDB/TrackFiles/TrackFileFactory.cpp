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

#include "TrackFiles/TrackFileFactory.hpp"
#include "TrackFiles/AIFFTrackFile.hpp"
#include "TrackFiles/MPEGTrackFile.hpp"
#include "TrackFiles/MP4TrackFile.hpp"
#include "TrackFiles/WavTrackFile.hpp"
#include "TrackFiles/OGGTrackFile.hpp"
#include "TrackFiles/FLACTrackFile.hpp"

#include "mp4file.h"

using namespace NxA::Serato;

#pragma mark Class Methods

TrackFileFactory::AudioFileType TrackFileFactory::audioFileTypeForPath(const String& trackFilePath)
{
    auto extension = File::extensionForFilePath(trackFilePath)->lowerCaseString();
    if ((*extension == ".aiff") || (*extension == ".aif")) {
        return AudioFileType::AIFF;
    }
    else if (*extension == ".mp3") {
        return AudioFileType::MP3;
    }
    else if ((*extension == ".m4a") || (*extension == ".mp4") || (*extension == ".m4v")) {
        return AudioFileType::MP4;
    }
    else if (*extension == ".flac") {
        return AudioFileType::FLAC;
    }
    else if (*extension == ".ogg") {
        return AudioFileType::OGG;
    }
    else if (*extension == ".wav") {
        return AudioFileType::WAV;
    }
    else {
        return AudioFileType::Unknown;
    }
}

TrackFile::Pointer TrackFileFactory::trackFileForPath(const String& trackFilePath, TrackFile::Flags flags)
{
    switch (TrackFileFactory::audioFileTypeForPath(trackFilePath)) {
        case AudioFileType::AIFF: {
            return TrackFile::Pointer::dynamicCastFrom(AIFFTrackFile::fileWithFileAt(trackFilePath, flags));
        }
        case AudioFileType::MP3: {
            return TrackFile::Pointer::dynamicCastFrom(MPEGTrackFile::fileWithFileAt(trackFilePath, flags));
        }
        case AudioFileType::MP4: {
            return TrackFile::Pointer::dynamicCastFrom(MP4TrackFile::fileWithFileAt(trackFilePath, flags));
        }
        case AudioFileType::FLAC: {
            return TrackFile::Pointer::dynamicCastFrom(FLACTrackFile::fileWithFileAt(trackFilePath, flags));
        }
        case AudioFileType::OGG: {
            return TrackFile::Pointer::dynamicCastFrom(OGGTrackFile::fileWithFileAt(trackFilePath, flags));
        }
        case AudioFileType::WAV: {
            return TrackFile::Pointer::dynamicCastFrom(WAVTrackFile::fileWithFileAt(trackFilePath, flags));
        }
        default: {
            throw TrackFileError::exceptionWith("Unknown file extension for file '%s'.", trackFilePath.toUTF8());
        }
    }
}
