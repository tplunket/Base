#ifndef ONEDB_ONEDBH
#define ONEDB_ONEDBH

class OneDBFile;
typedef OneDBFile* OneDBFileHandle;

#ifdef WINDOWS
typedef wchar_t			UTF16;
typedef __int64			Int64;
#include <Windows.h>
#else
#ifdef __APPLE_CC__
typedef unsigned short	UTF16;
typedef long long int	Int64;
#include <dlfcn.h>
#else
#error Unsupported platform.
#endif
#endif

#include <vector>

// UTF16 and UTF8 versions for open file
typedef OneDBFileHandle (__cdecl* OneDBOpenFileW) (const UTF16*);
typedef OneDBFileHandle (__cdecl* OneDBOpenFileA) (const char*);

typedef void (__cdecl* OneDBCloseFile) (OneDBFileHandle);

typedef Int64 (__cdecl* OneDBGetLengthInSamples) (OneDBFileHandle);
typedef Int64 (__cdecl* OneDBGetDurationInSamples) (OneDBFileHandle);
typedef int (__cdecl* OneDBGetChannels) (OneDBFileHandle);
typedef int (__cdecl* OneDBGetSamplerate) (OneDBFileHandle);
typedef Int64 (__cdecl* OneDBGetCuePosition) (OneDBFileHandle);
typedef Int64 (__cdecl* OneDBGetHotCuePosition) (OneDBFileHandle, int);
typedef void (__cdecl* OneDBSetLengthInSamples) (OneDBFileHandle, Int64);
typedef void (__cdecl* OneDBSetDurationInSamples) (OneDBFileHandle, Int64);
typedef void (__cdecl* OneDBSetChannels) (OneDBFileHandle, int);
typedef void (__cdecl* OneDBSetSamplerate) (OneDBFileHandle, int);
typedef void (__cdecl* OneDBSetCuePosition) (OneDBFileHandle, Int64);
typedef void (__cdecl* OneDBSetHotCuePosition) (OneDBFileHandle, int, Int64);
typedef bool (__cdecl* OneDBGetIsCueSet) (OneDBFileHandle);
typedef bool (__cdecl* OneDBGetIsHotCueSet) (OneDBFileHandle, int);
typedef void (__cdecl* OneDBClearCuePosition) (OneDBFileHandle);
typedef void (__cdecl* OneDBClearHotCuePosition) (OneDBFileHandle, int);

typedef void (__cdecl* OneDBGetFileGuidW) (OneDBFileHandle, UTF16*, int);
typedef void (__cdecl* OneDBGetArtistW) (OneDBFileHandle, UTF16*, int);
typedef void (__cdecl* OneDBGetTitleW) (OneDBFileHandle, UTF16*, int);
typedef void (__cdecl* OneDBGetGenreW) (OneDBFileHandle, UTF16*, int);
typedef void (__cdecl* OneDBGetYearW) (OneDBFileHandle, UTF16*, int);
typedef void (__cdecl* OneDBGetTrackNumberW) (OneDBFileHandle, UTF16*, int);

typedef void (__cdecl* OneDBGetFileGuidA) (OneDBFileHandle, char*, int);
typedef void (__cdecl* OneDBGetArtistA) (OneDBFileHandle, char*, int);
typedef void (__cdecl* OneDBGetTitleA) (OneDBFileHandle, char*, int);
typedef void (__cdecl* OneDBGetGenreA) (OneDBFileHandle, char*, int);
typedef void (__cdecl* OneDBGetYearA) (OneDBFileHandle, char*, int);
typedef void (__cdecl* OneDBGetTrackNumberA) (OneDBFileHandle, char*, int);

typedef void (__cdecl* OneDBSetFileGuidW) (OneDBFileHandle, const UTF16*);
typedef void (__cdecl* OneDBSetArtistW) (OneDBFileHandle, const UTF16*);
typedef void (__cdecl* OneDBSetTitleW) (OneDBFileHandle, const UTF16*);
typedef void (__cdecl* OneDBSetGenreW) (OneDBFileHandle, const UTF16*);
typedef void (__cdecl* OneDBSetYearW) (OneDBFileHandle, const UTF16*);
typedef void (__cdecl* OneDBSetTrackNumberW) (OneDBFileHandle, const UTF16*);

typedef void (__cdecl* OneDBSetFileGuidA) (OneDBFileHandle, const char*);
typedef void (__cdecl* OneDBSetArtistA) (OneDBFileHandle, const char*);
typedef void (__cdecl* OneDBSetTitleA) (OneDBFileHandle, const char*);
typedef void (__cdecl* OneDBSetGenreA) (OneDBFileHandle, const char*);
typedef void (__cdecl* OneDBSetYearA) (OneDBFileHandle, const char*);
typedef void (__cdecl* OneDBSetTrackNumberA) (OneDBFileHandle, const char*);

typedef float (__cdecl* OneDBGetBpm) (OneDBFileHandle);
typedef void (__cdecl* OneDBSetBpm) (OneDBFileHandle, float);
typedef int (__cdecl* OneDBGetBeatCount) (OneDBFileHandle);
typedef void (__cdecl* OneDBGetBeats) (OneDBFileHandle, Int64*, int);
typedef void (__cdecl* OneDBSetBeats) (OneDBFileHandle, Int64*, int);

typedef int (__cdecl* OneDBGetWaveformLevelCount) (OneDBFileHandle);
typedef void (__cdecl* OneDBGetWaveformLevels) (OneDBFileHandle, int*, int);
typedef int (__cdecl* OneDBGetWaveformLevelValueCount) (OneDBFileHandle, int);
typedef void (__cdecl* OneDBGetWaveformLevel) (OneDBFileHandle, int, unsigned short*, unsigned char*, unsigned char*, int);
typedef void (__cdecl* OneDBSetWaveformLevel) (OneDBFileHandle, int, unsigned short*, unsigned char*, unsigned char*, int);

class OneDBWaveformLevel
{
private:
	int stepM;
	std::vector<unsigned short> colorsM;
	std::vector<unsigned char> maximumAmplitudeM;
	std::vector<unsigned char> minimumAmplitudeM;
public:
	int getStep() const
	{
		return stepM;
	};
	void setStep(int stepP)
	{
		stepM = stepP;
	};
	const std::vector<unsigned short>& getColors() const
	{
		return colorsM;
	};
	std::vector<unsigned short>& getColors()
	{
		return colorsM;
	};

	const std::vector<unsigned char>& getMaximumAmplitudes() const
	{
		return maximumAmplitudeM;
	};
	std::vector<unsigned char>& getMaximumAmplitudes()
	{
		return maximumAmplitudeM;
	};

	const std::vector<unsigned char>& getMinimumAmplitudes() const
	{
		return minimumAmplitudeM;
	};
	std::vector<unsigned char>& getMinimumAmplitudes()
	{
		return minimumAmplitudeM;
	};
};

class OneDB
{
public:
	OneDB();
	~OneDB();

	OneDBFileHandle open(const UTF16* filenameP);
	OneDBFileHandle open(const char* filenameP);
	void close(OneDBFileHandle fileP);

	Int64 getLengthInSamples(OneDBFileHandle fileP) const;
	Int64 getDurationInSamples(OneDBFileHandle fileP) const;
	int getChannels(OneDBFileHandle fileP) const;
	int getSamplerate(OneDBFileHandle fileP) const;
	Int64 getCuePosition(OneDBFileHandle fileP) const;
    Int64 getHotCuePosition(OneDBFileHandle fileP, const int& indexP) const;
	void setLengthInSamples(OneDBFileHandle fileP, Int64 lengthSamplesP);
	void setDurationInSamples(OneDBFileHandle fileP, Int64 lengthSamplesP);
	void setChannels(OneDBFileHandle fileP, int channelsP);
	void setSamplerate(OneDBFileHandle fileP, int samplerateP);
    void setCuePosition(OneDBFileHandle fileP, const Int64& cuePositionP);
    void setHotCuePosition(OneDBFileHandle fileP, const int& indexP, const Int64& hotcuePositionP);
	bool getIsCueSet(OneDBFileHandle fileP) const;
    bool getIsHotCueSet(OneDBFileHandle fileP, const int& indexP) const;
	void clearCuePosition(OneDBFileHandle fileP);
    void clearHotCuePosition(OneDBFileHandle fileP, const int& indexP);

	// UTF16 versions of the string getters and setters
	void getFileGuid(OneDBFileHandle fileP, UTF16* bufferP, int bufferLengthP) const;
	void getArtist(OneDBFileHandle fileP, UTF16* bufferP, int bufferLengthP) const;
	void getTitle(OneDBFileHandle fileP, UTF16* bufferP, int bufferLengthP) const;
	void getGenre(OneDBFileHandle fileP, UTF16* bufferP, int bufferLengthP) const;
	void getYear(OneDBFileHandle fileP, UTF16* bufferP, int bufferLengthP) const;
	void getTrackNumber(OneDBFileHandle fileP, UTF16* bufferP, int bufferLengthP) const;
	void setFileGuid(OneDBFileHandle fileP, const UTF16* bufferP);
	void setArtist(OneDBFileHandle fileP, const UTF16* artistP);
	void setTitle(OneDBFileHandle fileP, const UTF16* titleP);
	void setGenre(OneDBFileHandle fileP, const UTF16* genreP);
	void setYear(OneDBFileHandle fileP, const UTF16* yearP);
	void setTrackNumber(OneDBFileHandle fileP, const UTF16* trackNumberP);

	// UTF8 versions of the string getters and setters
	void getFileGuid(OneDBFileHandle fileP, char* bufferP, int bufferSizeBytesP) const;
	void getArtist(OneDBFileHandle fileP, char* bufferP, int bufferSizeBytesP) const;
	void getTitle(OneDBFileHandle fileP, char* bufferP, int bufferSizeBytesP) const;
	void getGenre(OneDBFileHandle fileP, char* bufferP, int bufferSizeBytesP) const;
	void getYear(OneDBFileHandle fileP, char* bufferP, int bufferSizeBytesP) const;
	void getTrackNumber(OneDBFileHandle fileP, char* bufferP, int bufferSizeBytesP) const;
	void setFileGuid(OneDBFileHandle fileP, const char* bufferP);
	void setArtist(OneDBFileHandle fileP, const char* artistP);
	void setTitle(OneDBFileHandle fileP, const char* titleP);
	void setGenre(OneDBFileHandle fileP, const char* genreP);
	void setYear(OneDBFileHandle fileP, const char* yearP);
	void setTrackNumber(OneDBFileHandle fileP, const char* trackNumberP);

	// beat related functions
	float getBpm(OneDBFileHandle fileP) const;
	void setBpm(OneDBFileHandle fileP, const float& bpmP);
	void getBeats(OneDBFileHandle fileP, std::vector<Int64>& beatsP);
	void setBeats(OneDBFileHandle fileP, std::vector<Int64>& beatsP);

	// waveform related functions
	void getWaveformLevels(OneDBFileHandle fileP, std::vector<OneDBWaveformLevel*>& levelsP);
    void setWaveformLevels(OneDBFileHandle fileP, std::vector<OneDBWaveformLevel*>& levelsP);

private:
#ifdef WINDOWS
	HINSTANCE dllInstanceM;
#else
    void* dllInstanceM;
#endif
	OneDBOpenFileW openFileFuncWM;
	OneDBOpenFileA openFileFuncAM;
	OneDBCloseFile closeFileFuncM;

	OneDBGetLengthInSamples getLengthInSamplesM;
	OneDBGetDurationInSamples getDurationInSamplesM;
	OneDBGetChannels getChannelsM;
	OneDBGetSamplerate getSamplerateM;
	OneDBGetCuePosition getCuePositionM;
	OneDBGetHotCuePosition getHotCuePositionM;
	OneDBSetLengthInSamples setLengthInSamplesM;
	OneDBSetDurationInSamples setDurationInSamplesM;
	OneDBSetChannels setChannelsM;
	OneDBSetSamplerate setSamplerateM;
	OneDBSetCuePosition setCuePositionM;
	OneDBSetHotCuePosition setHotCuePositionM;
	OneDBGetIsCueSet getIsCueSetM;
	OneDBGetIsHotCueSet getIsHotCueSetM;
	OneDBClearCuePosition clearCuePositionM;
	OneDBClearHotCuePosition clearHotCuePositionM;

	OneDBGetFileGuidW getFileGuidWM;
	OneDBGetArtistW getArtistWM;
	OneDBGetTitleW getTitleWM;
	OneDBGetGenreW getGenreWM;
	OneDBGetYearW getYearWM;
	OneDBGetTrackNumberW getTrackNumberWM;

	OneDBGetFileGuidA getFileGuidAM;
	OneDBGetArtistA getArtistAM;
	OneDBGetTitleA getTitleAM;
	OneDBGetGenreA getGenreAM;
	OneDBGetYearA getYearAM;
	OneDBGetTrackNumberA getTrackNumberAM;

	OneDBSetFileGuidW setFileGuidWM;
	OneDBSetArtistW setArtistWM;
	OneDBSetTitleW setTitleWM;
	OneDBSetGenreW setGenreWM;
	OneDBSetYearW setYearWM;
	OneDBSetTrackNumberW setTrackNumberWM;

	OneDBSetFileGuidA setFileGuidAM;
	OneDBSetArtistA setArtistAM;
	OneDBSetTitleA setTitleAM;
	OneDBSetGenreA setGenreAM;
	OneDBSetYearA setYearAM;
	OneDBSetTrackNumberA setTrackNumberAM;

	OneDBGetBpm getBpmM;
	OneDBSetBpm setBpmM;
	OneDBGetBeatCount getBeatCountM;
	OneDBGetBeats getBeatsM;
	OneDBSetBeats setBeatsM;

	OneDBGetWaveformLevelCount getWaveformLevelCountM;
	OneDBGetWaveformLevels getWaveformLevelsM;
	OneDBGetWaveformLevelValueCount getWaveformLevelValueCountM;
	OneDBGetWaveformLevel getWaveformLevelM;
	OneDBSetWaveformLevel setWaveformLevelM;
};

#endif
