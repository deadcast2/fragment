#include "audio.h"

void InitAudio()
{
  if(FAILED(XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
  {
    Log("Failed to create XAudio2 instance\n");
    return;
  }

  if(FAILED(xAudio2->lpVtbl->CreateMasteringVoice(xAudio2, &xMasterVoice,
    XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, NULL, NULL,
    AudioCategory_GameEffects)))
  {
    Log("Failed to create XAudio2 mastering voice\n");
    return;
  }
}

void LoadAudio(const char *name, IXAudio2SourceVoice **source,
  XAUDIO2_BUFFER **audioBuffer, struct audioProps props)
{
  HANDLE resource = FindResource(NULL, name, "WAV");
  if(resource == NULL) return Log("Resource not found\n");
  HGLOBAL loadedResource = LoadResource(NULL, resource);
  if(loadedResource == NULL) return Log("Could not load resource\n");
  LPVOID resourceData = LockResource(loadedResource);
  DWORD resourceSize = SizeofResource(NULL, resource);

  int uncompressedSize = 0;
  memmove(&uncompressedSize, resourceData, sizeof(int));
  memmove(resourceData, resourceData + sizeof(int), resourceSize - sizeof(int));
  BYTE *decompressedData = HeapAlloc(GetProcessHeap(), 0, uncompressedSize);
  fastlz_decompress(resourceData, resourceSize - sizeof(int),
    decompressedData, uncompressedSize);

  ADPCMWAVEFORMAT *adpcm = (ADPCMWAVEFORMAT*)HeapAlloc(
    GetProcessHeap(), 0, sizeof(ADPCMWAVEFORMAT) + sizeof(ADPCMCOEFSET) * 7);
  adpcm->wSamplesPerBlock = 512;
  adpcm->wNumCoef = 7;
  adpcm->aCoef[0].iCoef1 = 256;
  adpcm->aCoef[0].iCoef1 = 0;
  adpcm->aCoef[1].iCoef1 = 512;
  adpcm->aCoef[1].iCoef1 = -256;
  adpcm->aCoef[2].iCoef1 = 0;
  adpcm->aCoef[2].iCoef1 = 0;
  adpcm->aCoef[3].iCoef1 = 192;
  adpcm->aCoef[3].iCoef1 = 64;
  adpcm->aCoef[4].iCoef1 = 240;
  adpcm->aCoef[4].iCoef1 = 0;
  adpcm->aCoef[5].iCoef1 = 460;
  adpcm->aCoef[5].iCoef1 = -208;
  adpcm->aCoef[6].iCoef1 = 392;
  adpcm->aCoef[6].iCoef1 = -232;
  DWORD wfxSize = 0;
  PVOID fmt = memmem(decompressedData, uncompressedSize, "fmt ", 4);
  if(fmt == NULL) return Log("fmt chunk not found\n");
  CopyMemory(&wfxSize, fmt + sizeof(DWORD), sizeof(DWORD));
  CopyMemory(&adpcm->wfx, fmt + sizeof(DWORD) * 2, wfxSize);

  XAUDIO2_BUFFER *buffer = HeapAlloc(GetProcessHeap(), 0, sizeof(XAUDIO2_BUFFER));
  DWORD dataSize = 0;
  PVOID data = memmem(decompressedData, uncompressedSize, "data", 4);
  if(data == NULL) return Log("data chunk not found\n");
  CopyMemory(&dataSize, data + sizeof(DWORD), sizeof(DWORD));
  BYTE *dataBuffer = HeapAlloc(GetProcessHeap(), 0, dataSize);
  CopyMemory(dataBuffer, data + sizeof(DWORD) * 2, dataSize);
  buffer->AudioBytes = dataSize;
  buffer->pAudioData = dataBuffer;
  buffer->Flags = XAUDIO2_END_OF_STREAM;
  if (props.shouldLoop) buffer->LoopCount = XAUDIO2_LOOP_INFINITE;
  *audioBuffer = buffer;

  if (FAILED(xAudio2->lpVtbl->CreateSourceVoice(xAudio2, source,
    (WAVEFORMATEX*)adpcm, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL)))
    return;

  HeapFree(GetProcessHeap(), 0, adpcm);
  HeapFree(GetProcessHeap(), 0, decompressedData);
}

void PlayAudio(IXAudio2SourceVoice *source, XAUDIO2_BUFFER *audioBuffer)
{
  if (!source) return;
  StopAudio(source);
  source->lpVtbl->SubmitSourceBuffer(source, audioBuffer, NULL);
  source->lpVtbl->Start(source, 0, XAUDIO2_COMMIT_NOW);
}

void StopAudio(IXAudio2SourceVoice *source)
{
  if (!source) return;
  source->lpVtbl->Stop(source, 0, XAUDIO2_COMMIT_NOW);
  source->lpVtbl->FlushSourceBuffers(source);
}

void CleanAudio()
{
  xMasterVoice->lpVtbl->DestroyVoice(xMasterVoice);
  xAudio2->lpVtbl->Release(xAudio2);
}
