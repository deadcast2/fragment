#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <windows.h>
#include <stdio.h>
#include "vendor/memmem.h"
#include "vendor/fastlz.h"
#include "vendor/XAudio2_custom.h"
#include "vendor/XAudio2fx_custom.h"
#include "log.h"

typedef struct AudioParams
{
  BOOL shouldLoop;
} AudioParams;

void InitAudio();
void CleanAudio();
void LoadAudio(const char *name, IXAudio2SourceVoice **source,
  XAUDIO2_BUFFER **audioBuffer, AudioParams params);
void PlayAudio(IXAudio2SourceVoice *source, XAUDIO2_BUFFER *audioBuffer);
void StopAudio(IXAudio2SourceVoice *source);

#endif
