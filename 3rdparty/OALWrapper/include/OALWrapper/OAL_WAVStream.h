#ifndef _OAL_WAVSTREAM_H
#define _OAL_WAVSTREAM_H

#include "OAL_Stream.h"
#include "OAL_Helper.h"

#include <SDL_audio.h>

//------------------------------------------------------------------------

class cOAL_WAVStream : public cOAL_Stream
{
public:
	cOAL_WAVStream();
	~cOAL_WAVStream();
	
	void Seek(float afWhere, bool abForceRebuffer);
protected:
	bool CreateFromFile(const std::wstring &asFileName);
	bool CreateFromBuffer(const void *apBuffer, size_t aSize);
	void Destroy();

	bool Stream(cOAL_Buffer* apDestBuffer);
	double GetTime();

	SDL_AudioSpec audioSpec;
	unsigned char* audioBuffer;
	unsigned int audioSize;
	unsigned int currentPos;

	int mlCurrent_section;
	bool mbIsValidHandle;
};

//------------------------------------------------------------------------

#endif
