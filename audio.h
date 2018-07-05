#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <dsound.h>
#include <mmreg.h>
#include "log.h"

LPDIRECTSOUND8 dSound;
LPDIRECTSOUNDBUFFER dSoundBuffer;

struct WaveHeaderType
{
  char chunkId[4];
	unsigned long chunkSize;
	char format[4];
	char subChunkId[4];
	unsigned long subChunkSize;
	unsigned short audioFormat;
	unsigned short numChannels;
	unsigned long sampleRate;
	unsigned long bytesPerSecond;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
	char dataChunkId[4];
	unsigned long dataSize;
};

void InitAudio(HWND hWnd);
void CleanAudio();
void LoadTestAudio();

#endif
