#include "audio.h"
#include <stdio.h>

void InitAudio()
{
  if(SUCCEEDED(XAudio2Create(&xAudio2)))
    Log("xaudio2 created\n");

  if(SUCCEEDED(xAudio2->lpVtbl->CreateMasteringVoice(xAudio2,
    &xMasteringVoice, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE,
    0, 0, NULL)))
    Log("mastering voice created\n");
}

void LoadTestAudio()
{
  HRSRC hResInfo = FindResource(NULL, "IDR_WAVE1", "WAV");
  if(hResInfo == NULL) Log("resource not found\n");
  HGLOBAL hResData = LoadResource(NULL, hResInfo);
  if(hResData == NULL) Log("resource not loaded\n");
  DWORD dwChunkSize;
  DWORD dwChunkPosition;
  DWORD filetype;
  if(FindChunk(hResData, FOURCCRIFF, &dwChunkSize, &dwChunkPosition) == S_OK)
    Log("found chunk\n");
  if(ReadChunkData(hResData, &filetype, sizeof(DWORD), dwChunkPosition) == S_OK)
    Log("read chunk data");
  if(filetype == FOURCCWAVE) Log("wav audio detected\n");
  char buffer[128];
  sprintf(buffer, "filetype: %lu\n", filetype);
  Log(buffer);
}

void CleanAudio()
{
  // todo
}
