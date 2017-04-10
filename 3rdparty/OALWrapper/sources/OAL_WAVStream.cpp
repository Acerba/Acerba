#include "OALWrapper/OAL_WAVStream.h"
#include "OALWrapper/OAL_Buffer.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

//---------------------------------------------------------------------

cOAL_WAVStream::cOAL_WAVStream() : mbIsValidHandle(false), currentPos(0)
{
}

//---------------------------------------------------------------------

cOAL_WAVStream::~cOAL_WAVStream()
{
	Destroy();
}

//---------------------------------------------------------------------

///////////////////////////////////////////////////////////
//	void Stream ( const ALuint alDestBuffer )
//	-	Streams data from the Ogg file to the buffer
///////////////////////////////////////////////////////////

//---------------------------------------------------------------------

inline static float WAVTotalTime(const SDL_AudioSpec& audioSpec, unsigned int samples)
{
	float numSamples = samples / (audioSpec.channels * (SDL_AUDIO_BITSIZE(audioSpec.format) / 8));
	return numSamples / audioSpec.freq;
}

inline static float WAVCurrentTime(const SDL_AudioSpec& audioSpec, unsigned int size, unsigned samples)
{
	float numSamples = (size-samples) / (audioSpec.channels * (SDL_AUDIO_BITSIZE(audioSpec.format) / 8));

	return WAVTotalTime(audioSpec, size) - (numSamples / audioSpec.freq);
}

bool cOAL_WAVStream::Stream(cOAL_Buffer* apDestBuffer)
{
	DEF_FUNC_NAME("cOAL_WAVStream::Stream()");
	
	double fStartTime = GetTime();

	long lDataSize = mlSamples * mlChannels;

	if (currentPos > audioSize)
	{
		lDataSize = 0;
	}
	else if (currentPos + lDataSize > audioSize)
	{
		lDataSize = audioSize - currentPos;
	}
	
	if(lDataSize) 
		mbStatus = apDestBuffer->Feed(audioBuffer + currentPos, lDataSize, fStartTime);
	else 
		mbEOF = true;
	
	currentPos += lDataSize;

	return (lDataSize != 0);
}

//---------------------------------------------------------------------

///////////////////////////////////////////////////////////
// void Seek(float afWhere)
//
///////////////////////////////////////////////////////////

//---------------------------------------------------------------------

void cOAL_WAVStream::Seek(float afWhere, bool abForceRebuffer)
{
	mbEOF = false;
	
	if(afWhere < 0)
		afWhere = 0;
	if(afWhere > 1)
		afWhere = 1;
	
	//afWhere = afWhere*mlSamples;
	afWhere = afWhere*audioSize;
	currentPos = afWhere;

	//// Move the pointer to the desired offset
	//ov_pcm_seek_page_lap(&movStreamHandle, (long int) afWhere);
	
	if(abForceRebuffer)
		mbNeedsRebuffering = true;
}

//---------------------------------------------------------------------

double cOAL_WAVStream::GetTime()
{
	return WAVCurrentTime(audioSpec, audioSize, currentPos);
}

//---------------------------------------------------------------------

///////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////

//---------------------------------------------------------------------

bool cOAL_WAVStream::CreateFromFile(const wstring &asFilename)
{
	DEF_FUNC_NAME("cOAL_WAVStream::CreateFromFile()");
	
	if(mbStatus==false)
		return false;
	
	//int lOpenResult;
	FILE *pStreamFile = OpenFileW(asFilename, L"rb");
	
	if (pStreamFile == NULL)
		return false;
	
	msFilename = asFilename;


	SDL_RWops* stream = SDL_RWFromFP(pStreamFile, SDL_TRUE);
	unsigned int size = SDL_RWsize(stream);
	
	unsigned char* buffer = new unsigned char[size + 1];
	
	SDL_RWread(stream, buffer, size, 1);

	buffer[size] = '\0';

	bool result = CreateFromBuffer(buffer, size);

	delete[] buffer;
	 
	return result;

	// If not an Ogg file, set status and exit
	
	//lOpenResult = ov_open_callbacks(pStreamFile, &movStreamHandle,
	//								NULL, 0, OV_CALLBACKS_DEFAULT);
	//if(lOpenResult<0)	
	//{
	//	fclose(pStreamFile);
	//	mbIsValidHandle = false;
	//	mbStatus = false;
	//	return false;
	//}
	//mbIsValidHandle = true;
	//
	//// Get file info
	//vorbis_info *viFileInfo = ov_info ( &movStreamHandle, -1 );
	//
	//mlChannels = viFileInfo->channels;
	//mlFrequency = viFileInfo->rate;
	//mlSamples = (long) ov_pcm_total ( &movStreamHandle, -1 );
	//mFormat = (mlChannels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
	//
	//mfTotalTime = ov_time_total( &movStreamHandle, -1 );
}

//---------------------------------------------------------------------

bool cOAL_WAVStream::CreateFromBuffer(const void *apBuffer, size_t aSize)
{
	DEF_FUNC_NAME("cOAL_WAVStream::CreateFromBuffer()");

	if(mbStatus==false)
		return false;
	
	if (apBuffer == NULL)
		return false;
	
	SDL_RWops* stream = SDL_RWFromConstMem(apBuffer, aSize);

	if (SDL_LoadWAV_RW(stream, 1, &audioSpec, &audioBuffer, &audioSize ) == NULL)
	{
		return false;
	}

	mbIsValidHandle = true;
	

	mlChannels = audioSpec.channels;
	mlFrequency = audioSpec.freq;
	mlSamples = audioSpec.samples;

	mFormat = (mlChannels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;

	mfTotalTime = WAVTotalTime(audioSpec, audioSize);

	return true;

	//// If not an Ogg file, set status and exit
	//tMemoryReader *pBufferStream = new tMemoryReader(apBuffer, aSize);
	//
	//int lOpenResult = ov_open_callbacks(pBufferStream, &movStreamHandle,
	//								NULL, 0, OV_MEMORY_CALLBACKS);
	//if(lOpenResult<0)
	//{
	//	delete pBufferStream;
	//	mbIsValidHandle = false;
	//	mbStatus = false;
	//	return false;
	//}
	//mbIsValidHandle = true;
	//
	//// Get file info
	//vorbis_info *viFileInfo = ov_info ( &movStreamHandle, -1 );
	//
	//mlChannels = viFileInfo->channels;
	//mlFrequency = viFileInfo->rate;
	//mlSamples = (long) ov_pcm_total ( &movStreamHandle, -1 );
	//mFormat = (mlChannels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
	//
	//mfTotalTime = ov_time_total( &movStreamHandle, -1 );
}

//---------------------------------------------------------------------

///////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////

//---------------------------------------------------------------------

void cOAL_WAVStream::Destroy()
{
	DEF_FUNC_NAME("cOAL_WAVStream::Unload()");
	
	// If we loaded a stream, clear the handle to the Ogg Vorbis file
	if (mbIsValidHandle)
	{
		//ov_clear(&movStreamHandle);
		SDL_FreeWAV(audioBuffer);
		audioBuffer = nullptr;
	}
}

//---------------------------------------------------------------------

