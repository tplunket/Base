//
//  NxASeratoTrackFile.h
//  SeratoDB
//
//  Created by Didier Malenfant on 2/6/15.
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

#ifndef __SeratoDB__NxASeratoTrackFile__
#define __SeratoDB__NxASeratoTrackFile__

#include <SeratoDB/NxASeratoCueMarker.h>
#include <SeratoDB/NxASeratoLoopMarker.h>
#include <SeratoDB/NxASeratoDbUtility.h>

#include <taglib/tag.h>
#include <taglib/tfile.h>
#include <taglib/tpropertymap.h>
#include <taglib/audioproperties.h>

#include <string>

namespace NxA {
    #pragma mark Forward declarations
    class SeratoTrackFile;

    #pragma mark Containers
    typedef std::unique_ptr<SeratoTrackFile> SeratoTrackFilePtr;
    typedef std::unique_ptr<TagLib::File> TaglibFilePtr;

    #pragma mark Class Declaration
    class SeratoTrackFile
    {
    private:
        #pragma mark Private Instance Variables
        SeratoCueMarkerVectorPtr p_cueMarkers;
        SeratoLoopMarkerVectorPtr p_loopMarkers;

    protected:
        #pragma mark Protected Constructors
        explicit SeratoTrackFile(const char* trackFilePath) :
                                 p_trackFilePath(StringPtr(new std::string(trackFilePath))),
                                 p_cueMarkers(new SeratoCueMarkerVector),
                                 p_loopMarkers(new SeratoLoopMarkerVector) { };

        #pragma mark Protected Instance Variables
        StringPtr p_trackFilePath;
        TaglibFilePtr p_file;
        TagLib::Tag* p_parsedFileTag;
        TagLib::PropertyMap p_properties;
        TagLib::AudioProperties* p_audioProperties;

        #pragma mark Protected Instance Methods
        void p_readMarkersV2FromBase64Data(const char* markerV2Data, size_t sizeInBytes);

    public:
        #pragma mark Public Constructor/Destructor
        virtual ~SeratoTrackFile() { }

        #pragma mark Instance Methods
        virtual std::string title(void) const;
        virtual std::string artist(void) const;
        virtual std::string genre(void) const;
        virtual bool hasKey(void) const = 0;
        virtual std::string key(void) const = 0;
        virtual std::string comments(void) const;
        virtual std::string album(void) const;
        virtual std::string composer(void) const;
        virtual std::string grouping(void) const = 0;
        virtual std::string bpm(void) const;

        virtual size_t sizeInBytes(void) const;
        virtual uint32_t lengthInMilliseconds(void) const;
        virtual uint32_t bitRateInKiloBitsPerSecond(void) const;
        virtual uint32_t bitDepthInBitsOrZeroIfNotApplicable(void) const;
        virtual uint32_t sampleRateInSamplesPerSecond(void) const;
        virtual uint32_t trackNumber(void) const;

        // -- TODO: Look into adding discNumber and numberOfAlbumTracks if they are available.

        virtual bool hasRecordLabel(void) const = 0;
        virtual std::string recordLabel(void) const = 0;
        virtual bool hasRemixer(void) const = 0;
        virtual std::string remixer(void) const = 0;
        virtual std::string yearReleased(void) const = 0;

        virtual CharVectorPtr artwork(void) const = 0;
        
        const SeratoCueMarkerVector& cueMarkers(void) const;
        const SeratoLoopMarkerVector& loopMarkers(void) const;
    };
}

#endif /* defined(__SeratoDB__NxASeratoTrackFile__) */
