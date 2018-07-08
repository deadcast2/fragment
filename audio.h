#ifndef _AUDIO_H_
#define _AUDIO_H_

#include "log.h"
#include "XAudio2_custom.h"

#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'

IXAudio2 *xAudio2;
IXAudio2MasteringVoice *xMasterVoice;
IXAudio2SourceVoice *xSourceVoice;

void InitAudio();
void CleanAudio();
HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD *dwChunkSize,
  DWORD *dwChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *buffer, DWORD buffersize,
  DWORD bufferoffset);
void LoadTestAudio();

#endif
