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

namespace NxA {
    #pragma mark Forward declarations
    class SeratoTrackFile;

    #pragma mark Containers
    typedef std::auto_ptr<SeratoTrackFile> SeratoTrackFileAutoPtr;
    typedef std::auto_ptr<TagLib::File> TaglibFileAutoPtr;

    #pragma mark Class Declaration
    class SeratoTrackFile
    {
    private:
        #pragma mark Private Instance Variables
        SeratoCueMarkerVectorAutoPtr p_cueMarkers;
        SeratoLoopMarkerVectorAutoPtr p_loopMarkers;

    protected:
        #pragma mark Protected Instance Variables
        StringAutoPtr p_trackFilePath;
        TaglibFileAutoPtr p_file;
        TagLib::Tag* p_parsedFileTag;
        TagLib::PropertyMap p_properties;
        TagLib::AudioProperties* p_audioProperties;

        #pragma mark Protected Instance Methods
        void p_readMarkersV2FromBase64Data(const char* markerV2Data, size_t sizeInBytes);

        #pragma mark Protected Constructors
        SeratoTrackFile(const char* trackFilePath) : p_trackFilePath(StringAutoPtr(new std::string(trackFilePath))),
                                                     p_cueMarkers(new SeratoCueMarkerVector),
                                                     p_loopMarkers(new SeratoLoopMarkerVector) { };
        
    public:
        #pragma mark Instance Methods
        virtual StringAutoPtr title(void) const;
        virtual StringAutoPtr artist(void) const;
        virtual StringAutoPtr genre(void) const;
        virtual bool hasKey(void) const = 0;
        virtual StringAutoPtr key(void) const = 0;
        virtual StringAutoPtr comments(void) const;
        virtual StringAutoPtr album(void) const;
        virtual StringAutoPtr composer(void) const;
        virtual StringAutoPtr grouping(void) const = 0;
        virtual StringAutoPtr bpm(void) const;

        virtual size_t sizeInBytes(void) const;
        virtual uint32_t lengthInMilliseconds(void) const;
        virtual uint32_t bitRateInKiloBitsPerSecond(void) const;
        virtual uint32_t bitDepthInBitsOrZeroIfNotApplicable(void) const;
        virtual uint32_t sampleRateInSamplesPerSecond(void) const;
        virtual uint32_t trackNumber(void) const;

        // -- TODO: Look into adding discNumber and numberOfAlbumTracks if they are available.

        virtual bool hasRecordLabel(void) const = 0;
        virtual StringAutoPtr recordLabel(void) const = 0;
        virtual bool hasRemixer(void) const = 0;
        virtual StringAutoPtr remixer(void) const = 0;
        virtual StringAutoPtr yearReleased(void) const = 0;

        virtual CharVectorAutoPtr artwork(void) const = 0;
        
        const SeratoCueMarkerVector& cueMarkers(void) const;
        const SeratoLoopMarkerVector& loopMarkers(void) const;
    };
}

#endif /* defined(__SeratoDB__NxASeratoTrackFile__) */
