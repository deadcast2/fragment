#ifndef _AUDIO_H_
#define _AUDIO_H_

#include "log.h"
#include "XAudio2_custom.h"

IXAudio2 *xAudio2;
IXAudio2MasteringVoice *xMasterVoice;
IXAudio2SourceVoice *xSourceVoice;

void InitAudio();
void CleanAudio();
void LoadTestAudio();

#endif
