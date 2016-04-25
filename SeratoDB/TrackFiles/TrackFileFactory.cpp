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

#include "SeratoDB/TrackFiles/TrackFileFactory.hpp"
#include "SeratoDB/TrackFiles/AIFFTrackFile.hpp"
#include "SeratoDB/TrackFiles/MPEGTrackFile.hpp"
#include "SeratoDB/TrackFiles/MP4TrackFile.hpp"
#include "SeratoDB/TrackFiles/WavTrackFile.hpp"
#include "SeratoDB/TrackFiles/OGGTrackFile.hpp"
#include "SeratoDB/TrackFiles/FLACTrackFile.hpp"

#include "mp4file.h"

using namespace NxA::Serato;

#pragma mark Class Methods

TrackFileFactory::AudioFileType TrackFileFactory::audioFileTypeForPath(const character *trackFilePath, count trackFilePathLength)
{
    NXA_ASSERT_TRUE(trackFilePath > 0);
    if (trackFilePathLength < 4) {
        return AudioFileType::Unknown;
    }

    const character *endOfTrackFilePath = (trackFilePath + trackFilePathLength) - 1;
    auto lastCharacter = tolower(*endOfTrackFilePath);
    auto nextToLast = tolower(*(endOfTrackFilePath - 1));
    auto secondToLast = tolower(*(endOfTrackFilePath - 2));
    auto thirdToLast = tolower(*(endOfTrackFilePath - 3));

    switch (lastCharacter) {
        case 'f':{
            if (trackFilePathLength >= 5 && nextToLast == 'f' && secondToLast == 'i' && thirdToLast == 'a' && *(endOfTrackFilePath - 4) == '.') {
                return AudioFileType::AIFF;
            }

            if (nextToLast == 'i' && secondToLast == 'a' && thirdToLast == '.') {
                return AudioFileType::AIFF;
            }
            break;
        }
        case '3':
        case '4': {
            if (nextToLast == 'p' && secondToLast == 'm' && thirdToLast == '.') {
                if (lastCharacter == '3') {
                    return AudioFileType::MP3;
                }

                return AudioFileType::MP4;
            }
            break;
        }
        case 'a': {
            if (nextToLast == '4' && secondToLast == 'm' && thirdToLast == '.') {
                return AudioFileType::MP4;
            }
            break;
        }
        case 'v': {
            if (nextToLast == '4' && secondToLast == 'm' && thirdToLast == '.') {
                return AudioFileType::MP4;
            }

            if (nextToLast == 'a' && secondToLast == 'w' && thirdToLast == '.') {
                return AudioFileType::WAV;
            }
            break;
        }
        case 'c': {
            if (trackFilePathLength >= 5 && nextToLast == 'a' && secondToLast == 'l' && thirdToLast == 'f' && *(endOfTrackFilePath - 4) == '.') {
                return AudioFileType::FLAC;
            }
            break;
        }
        case 'g': {
            if (nextToLast == 'g' && secondToLast == 'o' && thirdToLast == '.') {
                return AudioFileType::WAV;
            }
            break;
        }
    }

    return AudioFileType::Unknown;
}

TrackFileFactory::AudioFileType TrackFileFactory::audioFileTypeForPath(const String& trackFilePath)
{
    return TrackFileFactory::audioFileTypeForPath(trackFilePath.toUTF8());
}

TrackFile::Pointer TrackFileFactory::trackFileForPath(const String& trackFilePath, TrackFile::Flags flags)
{
    NXA_ASSERT_TRUE(trackFilePath.length() > 0);

    if (!File::fileExistsAt(trackFilePath)) {
        throw TrackFileNotFound::exceptionWith("Missing file '%s'.", trackFilePath.toUTF8());
    }

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
