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
        CharVectorPtrVector p_otherTags;

    protected:
        #pragma mark Protected Constructors
        explicit SeratoTrackFile(const char* trackFilePath) :
                                 p_trackFilePath(std::make_unique<std::string>(trackFilePath)),
                                 p_cueMarkers(std::make_unique<SeratoCueMarkerVector>()),
                                 p_loopMarkers(std::make_unique<SeratoLoopMarkerVector>()) { };

        #pragma mark Protected Instance Variables
        ConstStringPtr p_trackFilePath;
        TaglibFilePtr p_file;
        TagLib::Tag* p_parsedFileTag;
        TagLib::PropertyMap p_properties;
        TagLib::AudioProperties* p_audioProperties;

        #pragma mark Protected Instance Methods
        void p_readMarkersV2FromBase64Data(const char* markerV2Data, size_t sizeInBytes);
        CharVectorPtr p_base64DataFromMarkersV2(void);
        virtual void p_writeMarkersV2(void) = 0;

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

        virtual void setTitle(const char* title);
        virtual void setArtist(const char* artist);
        virtual void setGenre(const char* genre);
        virtual void setKey(const char* key) = 0;
        virtual void setComments(const char* comments);
        virtual void setAlbum(const char* album);
        virtual void setComposer(const char* composer);
        virtual void setGrouping(const char* grouping) = 0;
        virtual void setBpm(const char* bpm);
        virtual void setTrackNumber(const uint32_t& trackNumber);
        virtual void setRecordLabel(const char* recordLabel) = 0;
        virtual void setRemixer(const char* remixer) = 0;
        virtual void setYearReleased(const char* year) = 0;

        virtual void setArtwork(CharVectorPtr artwork) = 0;

        void setCueMarkers(SeratoCueMarkerVectorPtr markers);
        void setLoopMarkers(SeratoLoopMarkerVectorPtr markers);

        void saveChanges(void);
    };
}

#endif /* defined(__SeratoDB__NxASeratoTrackFile__) */
