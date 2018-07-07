#ifndef _AUDIO_H_
#define _AUDIO_H_

#include "log.h"
#include "xaudio2.h"

IXAudio2 *xAudio2;
IXAudio2MasteringVoice *xMasteringVoice;

void InitAudio();
void CleanAudio();
void LoadTestAudio();

#endif
