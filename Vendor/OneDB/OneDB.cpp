#include "OneDB.h"

#ifdef WINDOWS
#define LoadSym(func, sym) func((FARPROC)(GetProcAddress(HMODULE(dllInstanceM), sym)))
#else
#ifdef __APPLE_CC__
#define LoadSym(func, sym) (func)dlsym(dllInstanceM, sym)
#else
#error Unsupported platform.
#endif
#endif

OneDB::OneDB()
: dllInstanceM(NULL)
{
#ifdef WINDOWS
#ifdef X64
#ifdef ONEDB_DEBUG
    dllInstanceM = LoadLibrary(L"OneDBDebug64.dll");
#else
    dllInstanceM = LoadLibrary(L"OneDBRelease64.dll");
#endif
#else
#ifdef ONEDB_DEBUG
    dllInstanceM = LoadLibrary(L"OneDBDebug32.dll");
#else
    dllInstanceM = LoadLibrary(L"OneDBRelease32.dll");
#endif
#endif

    dllInstanceM = LoadLibrary(L"OneDBRelease32.dll");
#else
#ifdef __APPLE_CC__
    // reset dll errors
    dlerror();

#ifdef ONEDB_DEBUG
    const char* dllNameP = "libOneDBDebugUniversal.dylib";
#else
    const char* dllNameP = "libOneDBReleaseUniversal.dylib";
#endif

    // load shared lib
    dllInstanceM = dlopen(dllNameP, RTLD_LAZY | RTLD_LOCAL);
#else
#error Unsupported platform.
#endif
#endif

#ifdef TARGET_OS_MAC
	const char *dlsym_error = dlerror();
    if (dlsym_error)
    {
        printf("dlopen error: %s", dlsym_error);
        return;
    }
#endif

    if(!dllInstanceM)
    {
        printf("failed to load dynamic library");
        return;
    }

    // load symbols
    openFileFuncWM = LoadSym(OneDBOpenFileW, "openFileW");
	openFileFuncAM = LoadSym(OneDBOpenFileA, "openFileA");
	closeFileFuncM = LoadSym(OneDBCloseFile, "closeFile");

	getLengthInSamplesM = LoadSym(OneDBGetLengthInSamples, "getLengthInSamples");
	getDurationInSamplesM = LoadSym(OneDBGetDurationInSamples, "getDurationInSamples");
	getChannelsM = LoadSym(OneDBGetChannels, "getChannels");
	getSamplerateM = LoadSym(OneDBGetSamplerate, "getSamplerate");
	getCuePositionM = LoadSym(OneDBGetCuePosition, "getCuePosition");
	getHotCuePositionM = LoadSym(OneDBGetHotCuePosition, "getHotCuePosition");
	setLengthInSamplesM = LoadSym(OneDBSetLengthInSamples, "setLengthInSamples");
	setDurationInSamplesM = LoadSym(OneDBSetDurationInSamples, "setDurationInSamples");
	setChannelsM = LoadSym(OneDBSetChannels, "setChannels");
	setSamplerateM = LoadSym(OneDBSetSamplerate, "setSamplerate");
	setCuePositionM = LoadSym(OneDBSetCuePosition, "setCuePosition");
	setHotCuePositionM = LoadSym(OneDBSetHotCuePosition, "setHotCuePosition");
	getIsCueSetM = LoadSym(OneDBGetIsCueSet, "getIsCueSet");
	getIsHotCueSetM = LoadSym(OneDBGetIsHotCueSet, "getIsHotCueSet");
	clearCuePositionM = LoadSym(OneDBClearCuePosition, "clearCuePosition");
	clearHotCuePositionM = LoadSym(OneDBClearHotCuePosition, "clearHotCuePosition");

	getFileGuidWM = LoadSym(OneDBGetFileGuidW, "getFileGuidW");
	getArtistWM = LoadSym(OneDBGetArtistW, "getArtistW");
	getTitleWM = LoadSym(OneDBGetTitleW, "getTitleW");
	getGenreWM = LoadSym(OneDBGetGenreW, "getGenreW");
	getYearWM = LoadSym(OneDBGetYearW, "getYearW");
	getTrackNumberWM = LoadSym(OneDBGetTrackNumberW, "getTrackNumberW");

	getFileGuidAM = LoadSym(OneDBGetFileGuidA, "getFileGuidA");
	getArtistAM = LoadSym(OneDBGetArtistA, "getArtistA");
	getTitleAM = LoadSym(OneDBGetTitleA, "getTitleA");
	getGenreAM = LoadSym(OneDBGetGenreA, "getGenreA");
	getYearAM = LoadSym(OneDBGetYearA, "getYearA");
	getTrackNumberAM = LoadSym(OneDBGetTrackNumberA, "getTrackNumberA");

	setFileGuidWM = LoadSym(OneDBSetFileGuidW, "setFileGuidW");
	setArtistWM = LoadSym(OneDBSetArtistW, "setArtistW");
	setTitleWM = LoadSym(OneDBSetTitleW, "setTitleW");
	setGenreWM = LoadSym(OneDBSetGenreW, "setGenreW");
	setYearWM = LoadSym(OneDBSetYearW, "setYearW");
	setTrackNumberWM = LoadSym(OneDBSetTrackNumberW, "setTrackNumberW");

	setFileGuidAM = LoadSym(OneDBSetFileGuidA, "setFileGuidA");
	setArtistAM = LoadSym(OneDBSetArtistA, "setArtistA");
	setTitleAM = LoadSym(OneDBSetTitleA, "setTitleA");
	setGenreAM = LoadSym(OneDBSetGenreA, "setGenreA");
	setYearAM = LoadSym(OneDBSetYearA, "setYearA");
	setTrackNumberAM = LoadSym(OneDBSetTrackNumberA, "setTrackNumberA");

	getBpmM = LoadSym(OneDBGetBpm, "getBpm");
	setBpmM = LoadSym(OneDBSetBpm, "setBpm");
	getBeatCountM = LoadSym(OneDBGetBeatCount, "getBeatCount");
	getBeatsM = LoadSym(OneDBGetBeats, "getBeats");
	setBeatsM = LoadSym(OneDBSetBeats, "setBeats");

	getWaveformLevelCountM = LoadSym(OneDBGetWaveformLevelCount, "getWaveformLevelCount");
	getWaveformLevelsM = LoadSym(OneDBGetWaveformLevels, "getWaveformLevels");
	getWaveformLevelValueCountM = LoadSym(OneDBGetWaveformLevelValueCount, "getWaveformLevelValueCount");
	getWaveformLevelM = LoadSym(OneDBGetWaveformLevel, "getWaveformLevel");
	setWaveformLevelM = LoadSym(OneDBSetWaveformLevel, "setWaveformLevel");
}

OneDB::~OneDB()
{
#ifdef WINDOWS
    if(dllInstanceM)
    {
	    FreeLibrary(dllInstanceM);
    }
#else
#ifdef __APPLE_CC__
    if(dllInstanceM)
    {
        dlclose(dllInstanceM);
    }
#else
#error Unsupported platform.
#endif
#endif
}

OneDBFileHandle OneDB::open(const UTF16* filenameP)
{
	return openFileFuncWM(filenameP);
}

OneDBFileHandle OneDB::open(const char* filenameP)
{
	return openFileFuncAM(filenameP);
}

void OneDB::close(OneDBFileHandle fileP)
{
	closeFileFuncM(fileP);
}

Int64 OneDB::getLengthInSamples(OneDBFileHandle fileP) const
{
	return getLengthInSamplesM(fileP);
}

Int64 OneDB::getDurationInSamples(OneDBFileHandle fileP) const
{
	return getDurationInSamplesM(fileP);
}

int OneDB::getChannels(OneDBFileHandle fileP) const
{
	return getChannelsM(fileP);
}

int OneDB::getSamplerate(OneDBFileHandle fileP) const
{
	return getSamplerateM(fileP);
}

Int64 OneDB::getCuePosition(OneDBFileHandle fileP) const
{
	return getCuePositionM(fileP);
}

Int64 OneDB::getHotCuePosition(OneDBFileHandle fileP, const int& indexP) const
{
	return getHotCuePositionM(fileP, indexP);
}

void OneDB::setLengthInSamples(OneDBFileHandle fileP, Int64 lengthSamplesP)
{
	setLengthInSamplesM(fileP, lengthSamplesP);
}

void OneDB::setDurationInSamples(OneDBFileHandle fileP, Int64 lengthSamplesP)
{
	setDurationInSamplesM(fileP, lengthSamplesP);
}

void OneDB::setChannels(OneDBFileHandle fileP, int channelsP)
{
	setChannelsM(fileP, channelsP);
}

void OneDB::setSamplerate(OneDBFileHandle fileP, int samplerateP)
{
	setSamplerateM(fileP, samplerateP);
}

void OneDB::setCuePosition(OneDBFileHandle fileP, const Int64& cuePositionP)
{
	setCuePositionM(fileP, cuePositionP);
}

void OneDB::setHotCuePosition(OneDBFileHandle fileP, const int& indexP, const Int64& hotcuePositionP)
{
	setHotCuePositionM(fileP, indexP, hotcuePositionP);
}

bool OneDB::getIsCueSet(OneDBFileHandle fileP) const
{
	return getIsCueSetM(fileP);
}

bool OneDB::getIsHotCueSet(OneDBFileHandle fileP, const int& indexP) const
{
	return getIsHotCueSetM(fileP, indexP);
}

void OneDB::clearCuePosition(OneDBFileHandle fileP)
{
	clearCuePositionM(fileP);
}

void OneDB::clearHotCuePosition(OneDBFileHandle fileP, const int& indexP)
{
	clearHotCuePositionM(fileP, indexP);
}

// UTF16 versions of the string getters and setters
void OneDB::getFileGuid(OneDBFileHandle fileP, UTF16* bufferP, int bufferLengthP) const
{
	memset(reinterpret_cast<char*>(bufferP), 0, bufferLengthP * 2);
	getFileGuidWM(fileP, bufferP, bufferLengthP);
}

void OneDB::getArtist(OneDBFileHandle fileP, UTF16* bufferP, int bufferLengthP) const
{
	memset(reinterpret_cast<char*>(bufferP), 0, bufferLengthP * 2);
	getArtistWM(fileP, bufferP, bufferLengthP);
}

void OneDB::getTitle(OneDBFileHandle fileP, UTF16* bufferP, int bufferLengthP) const
{
	memset(reinterpret_cast<char*>(bufferP), 0, bufferLengthP * 2);
	getTitleWM(fileP, bufferP, bufferLengthP);
}

void OneDB::getGenre(OneDBFileHandle fileP, UTF16* bufferP, int bufferLengthP) const
{
	memset(reinterpret_cast<char*>(bufferP), 0, bufferLengthP * 2);
	getGenreWM(fileP, bufferP, bufferLengthP);
}

void OneDB::getYear(OneDBFileHandle fileP, UTF16* bufferP, int bufferLengthP) const
{
	memset(reinterpret_cast<char*>(bufferP), 0, bufferLengthP * 2);
	getYearWM(fileP, bufferP, bufferLengthP);
}

void OneDB::getTrackNumber(OneDBFileHandle fileP, UTF16* bufferP, int bufferLengthP) const
{
	memset(reinterpret_cast<char*>(bufferP), 0, bufferLengthP * 2);
	getTrackNumberWM(fileP, bufferP, bufferLengthP);
}

void OneDB::setFileGuid(OneDBFileHandle fileP, const UTF16* bufferP)
{
	setFileGuidWM(fileP, bufferP);
}

void OneDB::setArtist(OneDBFileHandle fileP, const UTF16* artistP)
{
	setArtistWM(fileP, artistP);
}

void OneDB::setTitle(OneDBFileHandle fileP, const UTF16* titleP)
{
	setTitleWM(fileP, titleP);
}

void OneDB::setGenre(OneDBFileHandle fileP, const UTF16* genreP)
{
	setGenreWM(fileP, genreP);
}

void OneDB::setYear(OneDBFileHandle fileP, const UTF16* yearP)
{
	setYearWM(fileP, yearP);
}

void OneDB::setTrackNumber(OneDBFileHandle fileP, const UTF16* trackNumberP)
{
	setTrackNumberWM(fileP, trackNumberP);
}

// UTF8 versions of the string getters and setters
void OneDB::getFileGuid(OneDBFileHandle fileP, char* bufferP, int bufferSizeBytesP) const
{
	memset(bufferP, 0, bufferSizeBytesP);
	getFileGuidAM(fileP, bufferP, bufferSizeBytesP);
}

void OneDB::getArtist(OneDBFileHandle fileP, char* bufferP, int bufferSizeBytesP) const
{
	memset(bufferP, 0, bufferSizeBytesP);
	getArtistAM(fileP, bufferP, bufferSizeBytesP);
}

void OneDB::getTitle(OneDBFileHandle fileP, char* bufferP, int bufferSizeBytesP) const
{
	memset(bufferP, 0, bufferSizeBytesP);
	getTitleAM(fileP, bufferP, bufferSizeBytesP);
}

void OneDB::getGenre(OneDBFileHandle fileP, char* bufferP, int bufferSizeBytesP) const
{
	memset(bufferP, 0, bufferSizeBytesP);
	getGenreAM(fileP, bufferP, bufferSizeBytesP);
}

void OneDB::getYear(OneDBFileHandle fileP, char* bufferP, int bufferSizeBytesP) const
{
	memset(bufferP, 0, bufferSizeBytesP);
	getYearAM(fileP, bufferP, bufferSizeBytesP);
}

void OneDB::getTrackNumber(OneDBFileHandle fileP, char* bufferP, int bufferSizeBytesP) const
{
	memset(bufferP, 0, bufferSizeBytesP);
	getTrackNumberAM(fileP, bufferP, bufferSizeBytesP);
}

void OneDB::setFileGuid(OneDBFileHandle fileP, const char* bufferP)
{
	setFileGuidAM(fileP, bufferP);
}

void OneDB::setArtist(OneDBFileHandle fileP, const char* artistP)
{
	setArtistAM(fileP, artistP);
}

void OneDB::setTitle(OneDBFileHandle fileP, const char* titleP)
{
	setTitleAM(fileP, titleP);
}

void OneDB::setGenre(OneDBFileHandle fileP, const char* genreP)
{
	setGenreAM(fileP, genreP);
}

void OneDB::setYear(OneDBFileHandle fileP, const char* yearP)
{
	setYearAM(fileP, yearP);
}

void OneDB::setTrackNumber(OneDBFileHandle fileP, const char* trackNumberP)
{
	setTrackNumberAM(fileP, trackNumberP);
}

float OneDB::getBpm(OneDBFileHandle fileP) const
{
	return getBpmM(fileP);
}

void OneDB::setBpm(OneDBFileHandle fileP, const float& bpmP)
{
	setBpmM(fileP, bpmP);
}

void OneDB::getBeats(OneDBFileHandle fileP, std::vector<Int64>& beatsP)
{
	int beatCount = getBeatCountM(fileP);
	if(beatCount == 0)
		return;
	beatsP.resize(beatCount);
	getBeatsM(fileP, beatsP.data(), beatCount);
}

void OneDB::setBeats(OneDBFileHandle fileP, std::vector<Int64>& beatsP)
{
	setBeatsM(fileP, beatsP.data(), (int)beatsP.size());
}

void OneDB::getWaveformLevels(OneDBFileHandle fileP, std::vector<OneDBWaveformLevel*>& levelsP)
{
	int levelCount = getWaveformLevelCountM(fileP);
	if(levelCount == 0)
		return;
	std::vector<int> levels;
	levels.resize(levelCount);
	getWaveformLevelsM(fileP, levels.data(), (int)levels.size());

	for(std::vector<int>::iterator level = levels.begin(); level != levels.end(); ++level)
	{
		OneDBWaveformLevel* newLevel = new OneDBWaveformLevel();
		newLevel->setStep(*level);
		newLevel->getColors().resize(getWaveformLevelValueCountM(fileP, *level));
		newLevel->getMaximumAmplitudes().resize(newLevel->getColors().size());
		newLevel->getMinimumAmplitudes().resize(newLevel->getColors().size());
		getWaveformLevelM(
                          fileP,
                          *level,
                          newLevel->getColors().data(),
                          newLevel->getMaximumAmplitudes().data(),
                          newLevel->getMinimumAmplitudes().data(),
                          (int)newLevel->getColors().size());
		levelsP.push_back(newLevel);
	}
}

void OneDB::setWaveformLevels(OneDBFileHandle fileP, std::vector<OneDBWaveformLevel*>& levelsP)
{
    for(std::vector<OneDBWaveformLevel*>::iterator level = levelsP.begin(); level != levelsP.end(); ++level)
    {
        setWaveformLevelM(
                          fileP,
                          (*level)->getStep(),
                          (*level)->getColors().data(),
                          (*level)->getMaximumAmplitudes().data(),
                          (*level)->getMinimumAmplitudes().data(),
                          (int)(*level)->getColors().size());
    }
}
