#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <windows.h>
#include <stdio.h>
#include "vendor/memmem.h"
#include "vendor/fastlz.h"
#include "vendor/XAudio2_custom.h"
#include "log.h"

IXAudio2 *xAudio2;
IXAudio2MasteringVoice *xMasterVoice;

struct audioProps
{
  BOOL shouldLoop;
};

void InitAudio();
void CleanAudio();
void LoadAudio(const char *name, IXAudio2SourceVoice **source, struct audioProps props);
void PlayAudio(IXAudio2SourceVoice *source);

#endif
